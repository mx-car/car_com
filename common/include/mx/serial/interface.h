#ifndef MX_SERIAL_SERIALINTERFACE_H
#define MX_SERIAL_SERIALINTERFACE_H

#include <stdint.h>
#include <iostream>
#include <memory>
#include <vector>
#include <functional>
#include <thread>
#include <mutex>
#include <mx/serial/message.h>

class TimeoutSerial;

namespace mx {
  namespace serial {

class SerialInterface {
    Message msg_tx_;
    Objects obj_tx_;
    Message msg_rx_;
    Objects obj_rx_;
    std::mutex mutex_obj_tx_;

    std::function<void (Message &,  Objects &)> callback;
    void serial_monitor ( const std::string& devname, unsigned int baud_rate );
    std::thread serial_monitor_thread_;
    TimeoutSerial *serial_timeout_;
public:
    SerialInterface();
    ~SerialInterface();
    void init ( const Parameters &param, std::function<void (Message &,  Objects &)> callback_fnc);
    void close();
    bool loop;
    void addObject(const Object &object);

};


}
}

#endif // MX_SERIAL_SERIALINTERFACE_H
