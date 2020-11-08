#ifndef CAR_COM_OBJECTS_WHEEL_H
#define CAR_COM_OBJECTS_WHEEL_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>
#include <car/com/objects/pose.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {
    
static const int ROTATION = 0;
static const int STEERING = 1;
static const int FRONT_WHEEL_LEFT = 0;
static const int FRONT_WHEEL_RIGHT = 1;
static const int REAR_WHEEL_LEFT = 2;
static const int REAR_WHEEL_RIGHT = 3;

class  Wheel {
public:
    Wheel() 
    : stamp() 
    , target{0.0, 0.0}
    , angle{0.0, 0.0}
    , speed{0.0, 0.0}
    , torque{0.0, 0.0}
    , pose() {
    };
    Wheel ( float rotation, float steering, bool tstamp = true) : stamp(){
        setTarget(rotation, steering, tstamp);
    }
    void setTarget(float rotation, float steering, bool tstamp = true){
        target[ROTATION] = rotation, target[STEERING] = steering;
        target_stamp = Time::now();
    }
    void setTarget(float rotation, float steering, const Time &t){
        setTarget(rotation, steering, false);
        target_stamp = t;
    }
    void setAngle(float rotation, float steering, bool tstamp = true){
        angle[ROTATION] = rotation, target[STEERING] = steering;
        target_angle = Time::now();
    }
    void setSpeed(float rotation, float steering, bool tstamp = true){
        speed[ROTATION] = rotation, speed[STEERING] = steering;
        target_speed = Time::now();
    }
    void setTorque(float rotation, float steering, bool tstamp = true){
        torque[ROTATION] = rotation, torque[STEERING] = steering;
        target_torque = Time::now();
    }
    Time stamp;              /// object stamp time when it was submitted
    Time target_stamp;       /// time when the target value should or was applied
    float target[2];         /// target values dimensionless
    Time target_angle;       /// time when the angle measurement was taken or computed
    float angle[2];          /// [rad] absolute angles. Wheel positon and steering angle
    Time target_speed;       /// time when the speed measurement was taken or computed
    float speed[2];          /// [rad/s] speed. Wheel and steering speed 
    Time target_torque;      /// time when the torque measurement was taken or computed
    float torque[2];         /// [Nm] torque. Wheel and steering torque 
    Pose pose;               /// location/mount point of the wheel on the vehicle
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Wheel &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, Wheel &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f]", target[ROTATION], target[STEERING], angle[ROTATION], angle[STEERING], speed[ROTATION], speed[STEERING], torque[ROTATION], torque[STEERING] );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f ]", stamp.getToStringReadable().c_str(), target[ROTATION], target[STEERING], angle[ROTATION], angle[STEERING], speed[ROTATION], speed[STEERING], torque[ROTATION], torque[STEERING] );
        return std::string ( buf );
    }
    std::string getToStringReadableSimple() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, roation: %+4.3f pow, %+4.3f rps; steering: %+4.3f pow, %+4.3f rad  ]", stamp.getToStringReadable().c_str(), target[ROTATION], speed[ROTATION], target[STEERING], angle[STEERING] );
        return std::string ( buf );
    }
    bool setFromString ( const std::string &str ) {
        int start = str.find ( "[" );
        int end = str.find_last_of ( "]" );
        std::string data = str.substr ( start+1, end-1 );
        if ( sscanf ( data.c_str(), "%f,%f", &target[ROTATION], &target[STEERING]) == EOF ) return false;
        return true;
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_WHEEL_H
