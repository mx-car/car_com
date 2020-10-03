
#if defined(__amd64__)
#include "car/com/pc/TimeoutSerial.h"
#include "car/com/pc/interface.h"


using namespace car::com::pc;

SerialInterface::SerialInterface()
    : serial_timeout_ ( NULL ) {
};
SerialInterface::~SerialInterface() {
    if(serial_timeout_) free(serial_timeout_);
};

void SerialInterface::close() {
    loop = false;
    for ( int i = 0; i < 4; i++ ) {
        if (serial_timeout_ && serial_timeout_->isOpen() ) {
            sleep ( 1 );
            std::cout << "closing serial connection" << std::endl;
        }
    }
    serial_monitor_thread_.join();
}

void SerialInterface::serial_monitor ( const std::string& devname, unsigned int baud_rate ) {
    loop = true;
    while ( loop ) {
        try {
            std::cout << "try: " << devname << "@" << baud_rate << "bit/sec" << std::endl;
            serial_timeout_ = new TimeoutSerial ( devname,  baud_rate );
            serial_timeout_->setTimeout ( boost::posix_time::seconds ( 3 ) );
            int i = 0;
            car::com::objects::Object object;
            car::com::objects::Pose pose;
            while ( loop ) {
                startMessageReceive:
                msg_rx_.reset();
                obj_rx_.clear();

                if (serial_timeout_->readEscaped ( ( char* ) &msg_rx_, sizeof ( MessageHeader ) ) == -1) goto startMessageReceive;
                if ( msg_rx_.size < Message::MAX_BUFFER_SIZE ) {
                    if (serial_timeout_->readEscaped ( ( char* ) &msg_rx_.buffer[0], msg_rx_.size ) == -1) goto startMessageReceive;
                    while ( msg_rx_.pop_object ( object ).isValid() ) {
                        switch ( object.type ) {
                        case car::com::objects::TYPE_SYNC_REQUEST:
                            std::cout << "Sync request" << std::endl;
                            addObject ( car::com::objects::Object ( car::com::objects::TYPE_SYNC ) );
                            break;
                        default:
                            obj_rx_[object.type] = object;
                        }
                    }

                    if ( callback ) {
                        callback ( msg_rx_, obj_rx_ );
                    }
                    if ( obj_tx_.size() ) {
                        std::lock_guard<std::mutex> lock ( mutex_obj_tx_ );
                        msg_tx_.time().now();
                        for ( Objects::iterator it=obj_tx_.begin(); it!=obj_tx_.end(); ++it ) {
                            car::com::objects::Object &object = it->second;
                            if ( object.type != it->first ) std::cerr << "object type mismatch!!" << std::endl;
                            msg_tx_.push_object ( object );
                        }

                        serial_timeout_->startTransmission();
                        serial_timeout_->writeEscaped ( ( char* ) &msg_tx_, sizeof ( MessageHeader ) );
                        serial_timeout_->writeEscaped ( &msg_tx_.buffer[0], msg_tx_.size );
                        serial_timeout_->endTransmission();

                        msg_tx_.reset();
                        obj_tx_.clear();
                    }
                }
            }
        } catch ( boost::system::system_error& e ) {
            std::cout << "Error on reading serial link: " << e.what() << std::endl;
        } catch (std::exception& e) {
            // This catches the timeout
            std::cout << "Error on reading serial link: " << e.what() << std::endl;
        }
        if ( serial_timeout_ ) {
            try {
                serial_timeout_->close();
                delete serial_timeout_;
                serial_timeout_ = NULL;
                sleep ( 1 );
            } catch ( boost::system::system_error& e ) {
                std::cout<<"Error on closing serial link: " << e.what() << std::endl;
            }
        }
    }
    try {
	if(serial_timeout_ && serial_timeout_->isOpen()){
	  serial_timeout_->close();
	}
    } catch ( boost::system::system_error& e ) {
        std::cout<<"Error on closing serial link: " << e.what() << std::endl;
    }
}

void SerialInterface::addObject ( const car::com::objects::Object &object ) {

    std::lock_guard<std::mutex> lock ( mutex_obj_tx_ );
    obj_tx_[object.type] = object;
}

void SerialInterface::init ( const Parameters &param, std::function<void ( Message &,  Objects & ) > callback_fnc ) {
    callback = callback_fnc;
    serial_monitor_thread_ = std::thread ( std::bind ( &SerialInterface::serial_monitor, this, param.port,  param.baudrate ) );
}

#endif //defined(__amd64__)
