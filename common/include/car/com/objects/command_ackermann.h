#ifndef CAR_COM_OBJECTS_COMMANDACKERMANN_H
#define CAR_COM_OBJECTS_COMMANDACKERMANN_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {



class  CommandAckermann {
public:
    static const uint16_t UNIT_DIRECT = 0;
    static const uint16_t UNIT_SPEED_ANGLE = 1;
    typedef uint16_t Units;

    CommandAckermann()
        : stamp()
        , units(UNIT_DIRECT)
        , forward(0)
        , steering(0.) {
    };
    CommandAckermann ( float forward_velocity, float steering_angle, Units command_units = UNIT_DIRECT, bool now = true) : stamp() {
        set(forward_velocity, steering_angle, command_units, now);
    }
    void set(float forward_velocity, float steering_angle, Units command_units = UNIT_DIRECT, bool now = true) {
        forward = forward_velocity, steering = steering_angle;
        if(now) stamp = Time::now();
    }
    Time stamp;
    Units units;
    float forward;
    float steering;

#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const CommandAckermann &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f]", forward, steering);
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        switch(units) {
        case UNIT_DIRECT:
            sprintf ( buf, "[%s, %+4.3f power, %+4.3f servo ]", stamp.getToStringReadable().c_str(), forward,  steering );
            break;
        case UNIT_SPEED_ANGLE:
            sprintf ( buf, "[%s, %+4.3fm/s, %+4.3frad ]", stamp.getToStringReadable().c_str(), forward,  steering );
            break;
        default:
            sprintf ( buf, "not supported units" );
        }
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_COMMANDACKERMANN_H
