
#include <signal.h>
#include <boost/program_options.hpp>
#include <mx/serial/interface.h>

using namespace std;
bool loop_main = true;
void ctrl_handler ( int s ) {
    loop_main = false;
}

struct Parameters {
    static int loop;
    mx::serial::Parameters serial;
    int rps;
};

mx::serial::SerialInterface serial_arduino;

void callback ( mx::serial::Message &header,  mx::serial::Objects & objects ) {

    mx::serial::Pose pose;
    mx::serial::Text text;
    std::cout << header << std::endl;
    for ( mx::serial::Objects::iterator it=objects.begin(); it!=objects.end(); ++it ) {
        mx::serial::Object &object = it->second;
        switch ( it->first ) {
        case mx::serial::TYPE_SYNC_REQUEST:
            std::cout << "Sync request" << std::endl;
            break;
        case mx::serial::TYPE_POSE:
            object.get ( pose );
            std::cout << "Pose: " << pose << std::endl;
            pose.y +=1.5;
            serial_arduino.addObject ( mx::serial::Object ( pose, mx::serial::TYPE_POSE ) );
            break;
        case mx::serial::TYPE_TEXT:
            object.get ( text );
            std::cout << "Text: " << text.txt << std::endl;
            break;
        default:
            std::cout << "Type id: " << object.type << ", of size: " << object.size << std::endl;
        }
    }
}


int main ( int argc, char* argv[] ) {
    namespace po = boost::program_options;

    Parameters params;
    po::options_description desc ( "Allowed Parameters" );
    desc.add_options()
    ( "help", "get this help message" )
    ( "port,m", po::value<std::string> ( &params.serial.port )->default_value ( "/dev/ttyACM0" ), "serial port" )
    ( "baudrate,b", po::value<int> ( &params.serial.baudrate )->default_value ( 115200 ), "baudrate" )
    ( "rps,r", po::value<int> ( &params.rps )->default_value ( 0 ), "motor rotation per second" );

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

    struct sigaction sigIntHandler;
    sigIntHandler.sa_handler = ctrl_handler;
    sigemptyset ( &sigIntHandler.sa_mask );
    sigIntHandler.sa_flags = 0;
    sigaction ( SIGINT, &sigIntHandler, NULL );

    sleep ( 1 );
    auto  callback_fnc ( std::bind ( &callback, std::placeholders::_1,  std::placeholders::_2 ) );
    serial_arduino.init ( params.serial, callback_fnc );
    
    {
        /// send command
        mx::serial::Object o(mx::serial::TActuators(params.rps, 0), mx::serial::TYPE_COMMAND_ACTUATORS);
        serial_arduino.addObject(o);
    }
    while ( loop_main ) {
        sleep ( 8 );
    }
    
    {
        /// stop motors
        mx::serial::Object o(mx::serial::TActuators(0, 0), mx::serial::TYPE_COMMAND_ACTUATORS);
        serial_arduino.addObject(o);
    }
    sleep ( 1 );
    cout<<"good-bye!" <<endl;
}
