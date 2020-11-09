#ifndef CAR_COM_OBJECTS_CONFIGACKERMANN_H
#define CAR_COM_OBJECTS_CONFIGACKERMANN_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  ConfigAckermann {
public:
    ConfigAckermann()
    : stamp()
    , wheel_diameter(0)
    , wheel_displacement(0.)
    , axis_displacement(0.){
    };
    ConfigAckermann ( float _wheel_diameter, float _wheel_displacement, float _axis_displacement, bool now = true ) : stamp(){
        set(_wheel_diameter, _wheel_displacement, _axis_displacement, now);
    }
    void set(float _wheel_diameter, float _wheel_displacement, float _axis_displacement, bool now = true){
        wheel_diameter = _wheel_diameter, wheel_displacement = _wheel_displacement, axis_displacement = _axis_displacement;
        if(now) stamp = Time::now();
    }
    Time stamp;
    
    float wheel_diameter;
    float wheel_displacement;
    float axis_displacement;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const ConfigAckermann &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f]", wheel_diameter, wheel_displacement,  axis_displacement);
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.3fm, %+4.3fm, %+4.3fm ]", stamp.getToStringReadable().c_str(), wheel_diameter, wheel_displacement,  axis_displacement );
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_ACKERMANNCONFIG_H
