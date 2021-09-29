#ifndef CAR_COM_OBJECTS_CONTROLPARAMETER_H
#define CAR_COM_OBJECTS_CONTROLPARAMETER_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class BLDCParameter {
public:
    static const int FORWARD = 0;
    static const int BACKWARD = 1;
    BLDCParameter() : angle_offset{0, 0} , nr_of_coils(11) {};
        
    BLDCParameter (const BLDCParameter &o)  : angle_offset{o.angle_offset[FORWARD], o.angle_offset[BACKWARD]}  , nr_of_coils(o.nr_of_coils) {};

    BLDCParameter (int16_t angle_offset_forward, int16_t angle_offset_backward, int16_t nr_of_coils) : angle_offset{angle_offset_forward, angle_offset_backward} , nr_of_coils(nr_of_coils) {};
        
    void set(int16_t angle_offset_forward, int16_t angle_offset_backward, int16_t nr_of_coils) {
        angle_offset[FORWARD] = angle_offset_forward, angle_offset[BACKWARD] = angle_offset_backward, this->nr_of_coils = nr_of_coils;
    }
    
    int16_t angle_offset[2];
    int16_t nr_of_coils;
    
#if defined(__amd64__)
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+3d, %+3d, %+2d]", angle_offset[FORWARD], angle_offset[BACKWARD], nr_of_coils);
        return std::string ( buf );
    }
#endif
};


class PIDParameter {
public:
    PIDParameter(): dt(0.), min(0.), max(0.), Kp(0.), Ki(0.), Kd(0.) {};
    PIDParameter(float dt, float min, float max, float Kp, float Ki, float Kd): dt(dt), min(min), max(max), Kp(Kp), Ki(Ki), Kd(Kd) {};
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

class  ControlParameter {
public:

    static const int LEFT = 0;
    static const int RIGHT = 1;
    enum Vehicle { 
        NA = 0, 
        MXR01 = 1, 
        MXR02 = 2};

    ControlParameter()
        : stamp() 
        , bldc({BLDCParameter(), BLDCParameter()})
        , pid({PIDParameter(), PIDParameter()}){
    };

    Time stamp;
    BLDCParameter bldc[2];
    PIDParameter pid[2];
    
    static ControlParameter get_default(Vehicle vehicle = NA){
        ControlParameter config;
        switch(vehicle){
            case MXR01:
                break;
            case MXR02:
            case NA:
            default:
                config.bldc[LEFT]  = BLDCParameter(-65     , -65 - 90, 11);
                config.bldc[RIGHT] = BLDCParameter(-80 + 90,      -80, 11);
                
                config.pid[LEFT] = PIDParameter(0.1, -1, 1, 0.2, 0.05, 0.01);
                config.pid[RIGHT] = PIDParameter(0.1, -1, 1, 0.2, 0.05, 0.01);
                
        }
        return config;
    }

#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const ControlParameter &o ) {
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


#endif // CAR_COM_OBJECTS_CONTROLPARAMETER_H
