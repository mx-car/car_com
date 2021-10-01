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

void callback ( car::com::Message &header,  car::com::Objects & objects );

struct Parameters {
    static int loop;
    car::com::pc::Parameters serial;
    float rps;
    float steering;
    bool decouble;
    float wheel_diameter;
    float wheel_displacement;
    float axis_displacement;
};

using namespace car::com::objects;
car::com::pc::SerialInterface serial_arduino;
AckermannState ackermann_command;
ControlParameter control_parameter;

int main ( int argc, char* argv[] )
{
    namespace po = boost::program_options;

    Parameters params;
    po::options_description desc ( "Allowed Parameters" );
    desc.add_options()
    ( "help", "get this help message" )
    ( "port,m", po::value<std::string> ( &params.serial.port )->default_value ( "/dev/ttyACM0" ), "serial port" )
    ( "baudrate,b", po::value<int> ( &params.serial.baudrate )->default_value ( 115200 ), "baudrate" )
    ( "decouble,c", po::value<bool> ( &params.decouble )->default_value ( false ),  "decouble motors" )
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
    AckermannConfig ackermann_config(params.wheel_diameter, params.wheel_displacement, params.axis_displacement );
    ackermann_command.set( params.rps, params.rps, params.steering, AckermannState::MODE_VELOCITY );

    auto  callback_fnc ( std::bind ( &callback, std::placeholders::_1,  std::placeholders::_2 ) );
    serial_arduino.init ( params.serial, callback_fnc );
    sleep ( 1 );
    {

        serial_arduino.addObject ( Object (ackermann_config, TYPE_ACKERMANN_CONFIG ) );
        serial_arduino.addObject ( Object( ackermann_command, TYPE_ACKERMANN_CMD ) );
    }
    sleep(1);
    {
        control_parameter = ControlParameter::get_default(ControlParameter::MXR02);
        serial_arduino.addObject ( Object( control_parameter, TYPE_CONTROL_PARAMETER ) );
    }
    while ( gSignalStatus == 0 ) {
        sleep ( 1 );
    }

    {
        /// stop motors
        ackermann_command.set(0, 0, 0, ackermann_command.MODE_PWM, false );
        Object o ( ackermann_command, TYPE_ACKERMANN_CMD );
        serial_arduino.addObject ( o );
    }
    std::cout << "good-bye!" << std::endl;
    std::cout << "SignalValue: " << gSignalStatus << '\n';
    sleep ( 1 );
}

