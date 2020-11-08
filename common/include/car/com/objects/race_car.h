#ifndef CAR_COM_OBJECTS_RACECAR_H
#define CAR_COM_OBJECTS_RACECAR_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>
#include <car/com/objects/pose.h>
#include <car/com/objects/raw_command.h>
#include <car/com/objects/raw_measurement.h>
#include <car/com/objects/twist.h>
#include <car/com/objects/wheel.h>
#include <car/com/objects/description_vehicle.h>

#if defined(__amd64__)
#include <iostream>
#include <sstream>
#endif

namespace car {
namespace com {
namespace objects {
    
    
class  RaceCar {
public:
    RaceCar()
    : stamp()
    , wheels_model_checked(false)  {
    };
    RaceCar (float engery, float steering) 
    : stamp()
    , wheels_model_checked(false) {
        stamp = Time::now();
        setTargetAckermann(engery, steering);
    };
    Time stamp;
    bool wheels_model_checked;
    Wheel wheels[4];
    Twist twist;
    PoseStamped  pose;
    
    RaceCar &setTargetAckermann(float engery, float steering){
        wheels_model_checked = false;
        Time t = Time::now();
        wheels[FRONT_WHEEL_LEFT].setTarget(0, steering, t);
        wheels[FRONT_WHEEL_RIGHT].setTarget(0, steering, t);
        wheels[REAR_WHEEL_LEFT].setTarget(engery, 0, t);
        wheels[REAR_WHEEL_RIGHT].setTarget(engery, 0, t);
        return *this;
    }
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const RaceCar &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        std::stringstream ss;
        ss << "[ " << stamp << ", " << wheels_model_checked;
        for(size_t i = 0; i < 4; i++) ss << ", " << wheels;
        ss << ", " << pose << "]" << std::endl;
        return ss.str();
    }
    std::string getToStringReadable() const {
        std::stringstream ss;
        ss << stamp.getToStringReadable().c_str() << std::endl;
        ss << "Wheels       :";
        for(size_t i = 0; i < 4; i++) ss << " " << i << ": " <<  wheels[i].getToStringReadableSimple() << std::endl;
        ss << "Twist        : "  << twist.getToStringReadable() << std::endl;
        ss << "Pose         : "  << pose.getToStringReadable() << std::endl;
        return ss.str();
    }
#endif

};
};
};
};


#endif // CAR_COM_OBJECTS_CAR_H
