#ifndef CAR_COM_OBJECTS_DESCRIPTIONVEHICLE_H
#define CAR_COM_OBJECTS_DESCRIPTIONVEHICLE_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  DescriptionVehicle {
public:
    DescriptionVehicle()
    : stamp()
    , wheel_diameter{0., 0.}
    , wheel_displacement(0.)
    , axis_displacement(0.){
    };
    DescriptionVehicle ( float _wheel_diameter_left, float _wheel_diameter_right, float _wheel_displacement, float _axis_displacement ) : stamp(){
        set(_wheel_diameter_left, _wheel_diameter_right, _wheel_displacement, _axis_displacement);
    }
    void set(float _wheel_diameter_left, float _wheel_diameter_right, float _wheel_displacement, float _axis_displacement){
        wheel_diameter[LEFT] = _wheel_diameter_left, wheel_diameter[RIGHT] = _wheel_diameter_right, wheel_displacement = _wheel_displacement, axis_displacement = _axis_displacement;
    }
    Time stamp;
    float wheel_diameter[2];
    float wheel_displacement;
    float axis_displacement;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const DescriptionVehicle &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f]", wheel_diameter[LEFT], wheel_diameter[RIGHT], wheel_displacement,  axis_displacement);
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.3fm, %+4.3fm, %+4.3fm, %+4.3fm ]", stamp.getToStringReadable().c_str(), wheel_diameter[LEFT], wheel_diameter[RIGHT], wheel_displacement,  axis_displacement );
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_DESCRIPTIONVEHICLE_H
