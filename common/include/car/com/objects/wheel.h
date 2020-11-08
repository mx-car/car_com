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
    : target{0.0, 0.0}
    , angle{0.0, 0.0}
    , speed{0.0, 0.0}
    , torque{0.0, 0.0} {
    };
    Wheel ( float rotation, float steering){
        setTarget(rotation, steering);
    }
    void setTarget(float rotation, float steering){
        target[ROTATION] = rotation, target[STEERING] = steering;
    }
    void setAngle(float rotation, float steering){
        angle[ROTATION] = rotation, target[STEERING] = steering;
    }
    void setSpeed(float rotation, float steering){
        speed[ROTATION] = rotation, speed[STEERING] = steering;
    }
    void setTorque(float rotation, float steering){
        torque[ROTATION] = rotation, torque[STEERING] = steering;
    }
    float target[2];         /// target values dimensionless
    float angle[2];          /// [rad] absolute angles. Wheel positon and steering angle
    float speed[2];          /// [rad/s] speed. Wheel and steering speed 
    float torque[2];         /// [Nm] torque. Wheel and steering torque 
    
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
        char buf[0x1FF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f]", target[ROTATION], target[STEERING], angle[ROTATION], angle[STEERING], speed[ROTATION], speed[STEERING], torque[ROTATION], torque[STEERING] );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0x1FF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f ]", target[ROTATION], target[STEERING], angle[ROTATION], angle[STEERING], speed[ROTATION], speed[STEERING], torque[ROTATION], torque[STEERING] );
        return std::string ( buf );
    }
    std::string getToStringReadableSimple() const {
        char buf[0x1FF];
        sprintf ( buf, "[ roation: %+4.3f pow, %+4.3f rps; steering: %+4.3f pow, %+4.3f rad  ]", target[ROTATION], speed[ROTATION], target[STEERING], angle[STEERING] );
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

class  WheelTimeStamp {
public:
    WheelTimeStamp() {}
    Time target;
    Time angle;
    Time speed;
    Time torque;
#if defined(__amd64__)
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %s, %s, %s ]", target.getToStringReadable().c_str(), angle.getToStringReadable().c_str(), speed.getToStringReadable().c_str(), torque.getToStringReadable().c_str() );
        return std::string ( buf );
    }
#endif
};

class  WheelCommand {
public:
    WheelCommand() 
    : stamp()
    , wheel{{0.,0.}, {0.,0.}, {0.,0.}, {0.,0.}}{
    };
    static const WheelCommand getCommandAckermann(float engery, float steering){
        WheelCommand cmd;
        cmd.wheel[FRONT_WHEEL_LEFT][ROTATION] = 0;
        cmd.wheel[FRONT_WHEEL_LEFT][STEERING] = steering;
        cmd.wheel[FRONT_WHEEL_RIGHT][ROTATION] = 0;
        cmd.wheel[FRONT_WHEEL_RIGHT][STEERING] = steering;
        
        cmd.wheel[REAR_WHEEL_LEFT][ROTATION] = engery;
        cmd.wheel[REAR_WHEEL_LEFT][STEERING] = 0;
        cmd.wheel[REAR_WHEEL_RIGHT][ROTATION] = engery;
        cmd.wheel[REAR_WHEEL_RIGHT][STEERING] = 0;
        
        cmd.stamp = Time::now();
        return cmd;
    }
    Time stamp;
    float wheel[4][2];
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const WheelCommand &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0x1FF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f]", 
                  wheel[FRONT_WHEEL_LEFT ][ROTATION], wheel[FRONT_WHEEL_LEFT ][STEERING], 
                  wheel[FRONT_WHEEL_LEFT ][ROTATION], wheel[FRONT_WHEEL_RIGHT][STEERING], 
                  wheel[REAR_WHEEL_LEFT  ][ROTATION], wheel[REAR_WHEEL_LEFT  ][STEERING], 
                  wheel[REAR_WHEEL_RIGHT ][ROTATION], wheel[REAR_WHEEL_RIGHT ][STEERING] );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0x1FF];
        float avr_rotation = wheel[REAR_WHEEL_LEFT ][ROTATION] / 2.0 + wheel[REAR_WHEEL_RIGHT ][ROTATION] / 2.0;
        float avr_steering = wheel[FRONT_WHEEL_LEFT][STEERING] / 2.0 + wheel[FRONT_WHEEL_RIGHT][STEERING] / 2.0;
        sprintf ( buf, "[%s, %+4.3f pow, %+4.3f servo ]", stamp.getToStringReadable().c_str(), avr_rotation, avr_steering);
        return std::string ( buf );
    }
#endif
};

};
};
};


#endif // CAR_COM_OBJECTS_WHEEL_H
