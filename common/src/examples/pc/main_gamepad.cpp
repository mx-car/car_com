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
    bool decouble;
    float wheel_diameter;
    float wheel_displacement;
    float axis_displacement;
    int axis_steering;
    int axis_velocity;
    float max_velocity;
    int button_couble;
    int button_velocity_mode;
    std::string device;
};

Parameters params;
car::com::pc::SerialInterface serial_arduino;
using namespace car::com::objects;
AckermannState ackermann_command;
ControlParameter control_parameter;


int main ( int argc, char* argv[] )
{
    namespace po = boost::program_options;

    po::options_description desc ( "Allowed Parameters" );
    desc.add_options()
    ( "help", "get this help message" )
    ( "port,m", po::value<std::string> ( &params.serial.port )->default_value ( "/dev/ttyACM0" ), "serial port" )
    ( "baudrate,b", po::value<int> ( &params.serial.baudrate )->default_value ( 115200 ), "baudrate" )
    ( "decouble,c", po::value<bool> ( &params.decouble )->default_value ( false ),  "decouble motors" )
    ( "wheel_diameter", po::value<float> ( &params.wheel_diameter )->default_value ( 0.065 ), "wheel diameter [m]" )
    ( "wheel_displacement", po::value<float> ( &params.wheel_displacement )->default_value ( 0.153 ), "wheel displacement [m]" )
    ( "axis_displacement", po::value<float> ( &params.axis_displacement )->default_value ( 0.26 ), "axis displacement [m]" )
    ( "max_velocity", po::value<float> ( &params.max_velocity )->default_value ( 1.0 ), "velocity max" )
    ( "axis_steering", po::value<int> ( &params.axis_steering )->default_value ( 3 ), "steering axis" )
    ( "axis_velocity", po::value<int> ( &params.axis_velocity )->default_value ( 4 ), "velocity axis" )
    ( "button_couble", po::value<int> ( &params.button_couble )->default_value ( 5 ), "couble motors" )
    ( "button_velocity_mode", po::value<int> ( &params.button_velocity_mode )->default_value ( 4 ), "velocity mode button" )
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
    AckermannConfig ackermann_config(params.wheel_diameter, params.wheel_displacement, params.axis_displacement );
    ackermann_command.set( 0, 0, 0, ackermann_command.MODE_PWM );

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
    mx::Joystick joy;

    if(joy.open(params.device) == -1) {
        printf("Could not open joystick");
        return 0;
    };

    /* This loop will exit if the controller is unplugged. */
    joy.start();
    mx::Joystick::Values gamepad;
    while ( gSignalStatus == 0 )
    {
        joy.get(gamepad);
        ackermann_command.coubled[0] = gamepad.button(params.button_couble);
        ackermann_command.coubled[1] = gamepad.button(params.button_couble);
        ackermann_command.v[0] = +gamepad.axis(params.axis_velocity) * params.max_velocity;
        ackermann_command.v[1] = -gamepad.axis(params.axis_velocity) * params.max_velocity;
        ackermann_command.steering = -gamepad.axis(params.axis_steering);
        ackermann_command.stamp = Time::now();
        
        ackermann_command.mode = AckermannState::MODE_PWM;
        if(gamepad.button(params.button_velocity_mode)) ackermann_command.mode = AckermannState::MODE_VELOCITY;
        
        usleep(1000);
    }
    joy.stop();
    joy.get_future().wait();
    {
        /// stop motors
        ackermann_command.set(0, 0, 0, AckermannState::MODE_PWM, false );
        Object o ( ackermann_command, TYPE_ACKERMANN_CMD );
        serial_arduino.addObject ( o );
    }
    sleep ( 1 );

    std::cout << "good-bye!" << std::endl;
    std::cout << "SignalValue: " << gSignalStatus << '\n';
}

