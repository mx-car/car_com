#ifndef CAR_COM_OBJECTS_STATE_ACKERMANN_H
#define CAR_COM_OBJECTS_STATE_ACKERMANN_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>
#include <car/com/objects/pose.h>
#include <car/com/objects/twist.h>
#include <car/com/objects/wheel.h>
#include <car/com/objects/config_ackermann.h>
#include <car/com/objects/command_ackermann.h>

#if defined(__amd64__)
#include <iostream>
#include <sstream>
#endif

namespace car {
namespace com {
namespace objects {
    
    
class  StateAckermann {
public:
    StateAckermann(){};
    Time stamp;
    Wheel wheels[4];
    WheelTimeStamp wheels_tstamp;
    TwistStamped twist;
    PoseStamped  pose;
    
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const StateAckermann &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        std::stringstream ss;
        ss << "[ " << stamp;
        for(size_t i = 0; i < 4; i++) ss << ", " << wheels;
        ss << ", " << pose << "]" << std::endl;
        return ss.str();
    }
    std::string getToStringReadable() const {
        std::stringstream ss;
        ss << stamp.getToStringReadable().c_str() << std::endl;
        ss << " Wheels       :" << wheels_tstamp.getToStringReadable() << std::endl;
        for(size_t i = 0; i < 4; i++) ss << "  " << i << ": " <<  wheels[i].getToStringReadableSimple() << std::endl;
        ss << " Twist        : "  << twist.getToStringReadable() << std::endl;
        ss << " Pose         : "  << pose.getToStringReadable();
        return ss.str();
    }
#endif
};
};
};
};

#endif // CAR_COM_OBJECTS_CAR_H
