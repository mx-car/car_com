#ifndef CAR_COM_OBJECTS_CONTROLCONFIG_H
#define CAR_COM_OBJECTS_CONTROLCONFIG_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class PIDConfig {
public:
    PIDConfig(): dt(0.), min(0.), max(0.), Kp(0.), Ki(0.), Kd(0.) {};
    PIDConfig(float dt, float min, float max, float Kp, float Ki, float Kd): dt(dt), min(min), max(max), Kp(Kp), Ki(Ki), Kd(Kd) {};
    float dt;
    float min;
    float max;
    float Kp;
    float Ki;
    float Kd;
#if defined(__amd64__)
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f]", dt, min, max, Kp, Ki, Kd);
        return std::string ( buf );
    }
#endif
};

class  ControlConfig {
public:

    static const int LEFT = 0;
    static const int RIGHT = 1;


    ControlConfig()
        : stamp() 
        , pid({PIDConfig(), PIDConfig()}){
    };

    Time stamp;
    PIDConfig pid[2];

#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const ControlConfig &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %s, %s]",  pid[LEFT].getToString().c_str(), pid[RIGHT].getToString().c_str());
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %s, %s]", stamp.getToStringReadable().c_str(), pid[LEFT].getToString().c_str(), pid[RIGHT].getToString().c_str());
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_CONTROLCONFIG_H
