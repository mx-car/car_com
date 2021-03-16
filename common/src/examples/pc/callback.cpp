#include <car/com/pc/interface.h>

extern car::com::pc::SerialInterface serial_arduino;
extern car::com::objects::AckermannState ackermann_command;

car::com::objects::Time tprev;
void callback ( car::com::Message &header,  car::com::Objects & objects )
{
    car::com::objects::Time tnow = car::com::objects::Time::now();
    car::com::objects::Duration dt = tnow - tprev;
    tprev = tnow;
    std::cout << std::endl << "Header: "  << header.getToStringReadable() << " with " << objects.size()  << " objects, " << std::fixed << std::setw( 5 ) << std::setprecision(3) << dt.toSec() << "sec" << std::endl;
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
        case car::com::objects::TYPE_ACKERMANN_CONFIG: {
            car::com::objects::AckermannConfig config;
            object.get ( config );
            std::cout << "AckermannConfig : " << config.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_ACKERMANN_STATE: {
            car::com::objects::AckermannState cmd;
            object.get ( cmd );
            std::cout << "AckermannState : " << cmd.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_ACKERMANN_CMD: {
            car::com::objects::AckermannState cmd;
            object.get ( cmd );
            std::cout << "AckermannCommand : " << cmd.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_POSE_STAMPED: {
            car::com::objects::PoseStamped pose;
            object.get ( pose );
            std::cout << "PoseStamped : " << pose.getToStringReadable() << std::endl;
        }
        break;
        case car::com::objects::TYPE_ARRAY16SC4: {
            car::com::objects::Array16SC4 array;
            object.get ( array );
            std::cout << "Array16SC4 : " << array << std::endl;
        }
        break;
        case car::com::objects::TYPE_ARRAY16SC8: {
            car::com::objects::Array16SC8 array;
            object.get ( array );
            std::cout << "Array16SC8 : " << array << std::endl;
        }
        break;
        case car::com::objects::TYPE_ARRAY16FC4: {
            car::com::objects::Array16FC4 array;
            object.get ( array );
            std::cout << "Array16FC4 : " << array << std::endl;
        }
        break;
        case car::com::objects::TYPE_ARRAY16FC8: {
            car::com::objects::Array16FC8 array;
            object.get ( array );
            std::cout << "Array16FC8 : " << array << std::endl;
        }
        case car::com::objects::TYPE_CONTROL_CONFIG: {
            car::com::objects::ControlConfig o;
            object.get ( o );
            std::cout << "ControlConfig : " << o << std::endl;
        }
        break;
        default:
            std::cout << "Type id: " << object.type << ", of size: " << object.size << std::endl;
        }
    }
    serial_arduino.addObject ( car::com::objects::Object( ackermann_command, car::com::objects::TYPE_ACKERMANN_CMD ) );
    std::cout << "ackermann_command : " << ackermann_command.getToStringReadable() << std::endl;
}
