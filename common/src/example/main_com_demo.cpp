#include <csignal>
#include <iostream>
#include <boost/program_options.hpp>
#include <car/com/pc/interface.h>

namespace
{
volatile std::sig_atomic_t gSignalStatus;
}
void signal_handler ( int signal )
{
    gSignalStatus = signal;
}

struct Parameters {
    static int loop;
    car::com::pc::Parameters serial;
    int rps;
    int steering;
};

car::com::pc::SerialInterface serial_arduino;


void callback ( car::com::Message &header,  car::com::Objects & objects )
{

    std::cout << header << " with " << objects.size()  << " objects" << std::endl;
    for ( car::com::Objects::iterator it=objects.begin(); it!=objects.end(); ++it ) {
        car::com::objects::Object &object = it->second;
        switch ( it->first ) {
        case car::com::objects::TYPE_SYNC_REQUEST:
            std::cout << "Sync request" << std::endl;
            break;
        case car::com::objects::TYPE_TEXT: {
            car::com::objects::Text text;
            object.get ( text );
            std::cout << "Text: " << text.txt << std::endl;
        }
        break;
        case car::com::objects::TYPE_RACE_CAR: {
            car::com::objects::RaceCar state;
            object.get ( state );
            std::cout << "CarState: " << state.getToStringReadable() << std::endl;
        }
        break;
        default:
            std::cout << "Type id: " << object.type << ", of size: " << object.size << std::endl;
        }
    }
}


int main ( int argc, char* argv[] )
{
    namespace po = boost::program_options;

    Parameters params;
    po::options_description desc ( "Allowed Parameters" );
    desc.add_options()
    ( "help", "get this help message" )
    ( "port,m", po::value<std::string> ( &params.serial.port )->default_value ( "/dev/ttyACM0" ), "serial port" )
    ( "baudrate,b", po::value<int> ( &params.serial.baudrate )->default_value ( 115200 ), "baudrate" )
    ( "rps,r", po::value<int> ( &params.rps )->default_value ( 0 ), "motor power between -1.0 and 1.0" )
    ( "steering,s", po::value<int> ( &params.steering )->default_value ( 90 ), "servo steering between -1.0 and 1.0" );

    po::variables_map vm;
    try {
        po::store ( po::parse_command_line ( argc, argv, desc ), vm );
    } catch ( const std::exception &ex ) {
        std::cout << desc << std::endl;;
        exit ( 1 );
    }
    po::notify ( vm );

    if ( vm.count ( "help" ) )  {
        std::cout << desc << std::endl;
        exit ( 1 );
    }

    std::signal ( SIGINT, signal_handler );

    sleep ( 1 );
    auto  callback_fnc ( std::bind ( &callback, std::placeholders::_1,  std::placeholders::_2 ) );
    serial_arduino.init ( params.serial, callback_fnc );

    {
        /// send command
        car::com::objects::RaceCar target ( params.rps, params.steering );
        car::com::objects::Object o ( target, car::com::objects::TYPE_RACE_CAR );

        serial_arduino.addObject ( o );
    }
    while ( gSignalStatus == 0 ) {
        sleep ( 1 );
    }

    {
        /// stop motors
        car::com::objects::Object o ( car::com::objects::RaceCar ( 0, 0 ), car::com::objects::TYPE_RACE_CAR );
        serial_arduino.addObject ( o );
    }
    sleep ( 1 );

    std::cout << "good-bye!" << std::endl;
    std::cout << "SignalValue: " << gSignalStatus << '\n';
}

