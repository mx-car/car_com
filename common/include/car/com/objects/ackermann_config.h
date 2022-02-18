#ifndef CAR_COM_OBJECTS_ACKERMANNCONFIG_H
#define CAR_COM_OBJECTS_ACKERMANNCONFIG_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  AckermannConfig {
public:
    AckermannConfig()
    : stamp()
    , wheel_diameter(0)
    , wheel_displacement(0.)
    , wheel_axle_displacement(0.){
    };
    AckermannConfig ( float _wheel_diameter, float _wheel_displacement, float _wheel_axle_displacement, bool now = true ) : stamp(){
        set(_wheel_diameter, _wheel_displacement, _wheel_axle_displacement, now);
    }
    void set(float _wheel_diameter, float _wheel_displacement, float _wheel_axle_displacement, bool now = true){
        wheel_diameter = _wheel_diameter, wheel_displacement = _wheel_displacement, wheel_axle_displacement = _wheel_axle_displacement;
        if(now) stamp = Time::now();
    }
    
    template <class T>
    void set(T o){
        wheel_diameter = o.wheel_diameter;
        wheel_displacement = o.wheel_displacement;
        wheel_axle_displacement = o.wheel_axle_displacement;
    }
    
    Time stamp;
    
    float wheel_diameter;
    float wheel_displacement;
    float wheel_axle_displacement;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const AckermannConfig &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f]", wheel_diameter, wheel_displacement,  wheel_axle_displacement);
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.3fm, %+4.3fm, %+4.3fm ]", stamp.getToStringReadable().c_str(), wheel_diameter, wheel_displacement,  wheel_axle_displacement );
        return std::string ( buf );
    }
#endif
};
}
}
}


#endif // CAR_COM_OBJECTS_ACKERMANNCONFIG_H
