#include <csignal>
#include <iostream>
#include <boost/program_options.hpp>
#include <car/com/pc/interface.h>
#include <mx/joystick.h>
#include <limits>       // std::numeric_limits

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
    int axis_steering;
    int axis_velocity;
    int button_uncouble;
    int dead_men_button;
    std::string device;
};

Parameters params;
car::com::pc::SerialInterface serial_arduino;
car::com::objects::AckermannState ackermann_command;


int main ( int argc, char* argv[] )
{
    namespace po = boost::program_options;

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
    ( "axis_displacement", po::value<float> ( &params.axis_displacement )->default_value ( 0.26 ), "axis displacement [m]" )
    ( "axis_steering", po::value<int> ( &params.axis_steering )->default_value ( 0 ), "Steering Axis" )
    ( "axis_velocity", po::value<int> ( &params.axis_velocity )->default_value ( 2 ), "Velocity Axis" )
    ( "button_uncouble", po::value<int> ( &params.button_uncouble )->default_value ( 4 ), "Uncouble motors" )
    ( "dead_men_button", po::value<int> ( &params.dead_men_button )->default_value ( 5 ), "Dead Men's Button" )
    ( "device,d", po::value<std::string> ( &params.device )->default_value ( "/dev/input/js0" ), "joystick device" );

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
    ackermann_command.set(params.wheel_diameter, params.wheel_displacement, params.axis_displacement );
    car::com::objects::AckermannState ackermann_state( params.rps, params.rps, params.steering );

    auto  callback_fnc ( std::bind ( &callback, std::placeholders::_1,  std::placeholders::_2 ) );
    serial_arduino.init ( params.serial, callback_fnc );
    sleep ( 1 );
    {
        serial_arduino.addObject ( car::com::objects::Object (ackermann_command, car::com::objects::TYPE_ACKERMANN_CONFIG ) );
        serial_arduino.addObject ( car::com::objects::Object( ackermann_state, car::com::objects::TYPE_ACKERMANN_CMD ) );

    }
    mx::Joystick joy;

    if(joy.init(params.device) == -1) {
        printf("Could not open joystick");
        return 0;
    };

    /* This loop will exit if the controller is unplugged. */
    joy.start();

    while ( gSignalStatus == 0 )
    {
        ackermann_command.coubled[0] = (joy.button(params.button_uncouble).value == 0);
        ackermann_command.coubled[1] = (joy.button(params.button_uncouble).value == 0);
        ackermann_command.v[0] = ((float) joy.axis(params.axis_velocity).x) / ((float) std::numeric_limits<short>::max()) * 3.;
        ackermann_command.v[1] = -ackermann_command.v[0];
    }



    {
        /// stop motors
        ackermann_command.set(0, 0, 0, false );
        car::com::objects::Object o ( ackermann_command, car::com::objects::TYPE_ACKERMANN_CMD );
        serial_arduino.addObject ( o );
    }
    sleep ( 1 );

    std::cout << "good-bye!" << std::endl;
    std::cout << "SignalValue: " << gSignalStatus << '\n';
}

