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
    float rps;
    float steering;
    float wheel_diameter;
    float wheel_displacement;
    float axis_displacement;
};

car::com::pc::SerialInterface serial_arduino;


void callback ( car::com::Message &header,  car::com::Objects & objects )
{

    std::cout << std::endl << "Header: "  << header.getToStringReadable() << " with " << objects.size()  << " objects" << std::endl;
    for ( car::com::Objects::iterator it=objects.begin(); it!=objects.end(); ++it ) {
        car::com::objects::Object &object = it->second;
        switch ( it->first ) {
        case car::com::objects::TYPE_SYNC_REQUEST: {
            std::cout << "Sync request" << std::endl;
        }
        break;
        case car::com::objects::TYPE_TEXT: {
            car::com::objects::Text text;
            object.get ( text );
            std::cout << "Text: " << text.txt << std::endl;
        }
        break;
        case car::com::objects::TYPE_STATE_ACKERMANN: {
            car::com::objects::StateAckermann state;
            object.get ( state );
            std::cout << "StateAckermann  : " << state.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_COMMAND_ACKERMANN: {
            car::com::objects::CommandAckermann cmd;
            object.get ( cmd );
            std::cout << "CommandAckermann: " << cmd.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_CONFIG_ACKERMANN: {
            car::com::objects::AckermannConfig config;
            object.get ( config );
            std::cout << "AckermannConfig : " << config.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_ACKERMANN_REAR_WHEEL_DRIVE: {
            car::com::objects::AckermannRearWheelDrive cmd;
            object.get ( cmd );
            std::cout << "AckermannRearWheelDrive : " << cmd.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_ARRAY_4N: {
            car::com::objects::Array<4> array;
            object.get ( array );
            std::cout << "Array<4> : " << array << std::endl;
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
    ( "rps,r", po::value<float> ( &params.rps )->default_value ( 0 ), "motor power between -1.0 and 1.0" )
    ( "steering,s", po::value<float> ( &params.steering )->default_value ( 90 ), "servo steering between -1.0 and 1.0" )
    ( "wheel_diameter", po::value<float> ( &params.wheel_diameter )->default_value ( 0.065 ), "wheel diameter [m]" )
    ( "wheel_displacement", po::value<float> ( &params.wheel_displacement )->default_value ( 0.153 ), "wheel displacement [m]" )
    ( "axis_displacement", po::value<float> ( &params.axis_displacement )->default_value ( 0.26 ), "axis displacement [m]" );

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

    /// send command
    car::com::objects::AckermannRearWheelDrive ackermann_rear_wheel_drive( params.rps, params.rps, params.steering );
    car::com::objects::AckermannConfig ackermann_config(params.wheel_diameter, params.wheel_displacement, params.axis_displacement );

    auto  callback_fnc ( std::bind ( &callback, std::placeholders::_1,  std::placeholders::_2 ) );
    serial_arduino.init ( params.serial, callback_fnc );
    sleep ( 1 );
    {

        serial_arduino.addObject ( car::com::objects::Object (ackermann_config, car::com::objects::TYPE_ACKERMANN_CONFIG ) );
        serial_arduino.addObject ( car::com::objects::Object( ackermann_rear_wheel_drive, car::com::objects::TYPE_ACKERMANN_REAR_WHEEL_DRIVE ) );

    }
    while ( gSignalStatus == 0 ) {
        sleep ( 1 );
    }

    {
        /// stop motors
        car::com::objects::CommandAckermann command_object( 0, 0 );
        command_object.inhibitor = true;
        car::com::objects::Object o ( command_object, car::com::objects::TYPE_COMMAND_ACKERMANN );
        serial_arduino.addObject ( o );
    }
    sleep ( 1 );

    std::cout << "good-bye!" << std::endl;
    std::cout << "SignalValue: " << gSignalStatus << '\n';
}

