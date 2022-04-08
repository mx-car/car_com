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
    bool operator==( const BLDCParameter &o ) const { 
        return o.angle_offset[FORWARD] == angle_offset[FORWARD] && o.angle_offset[BACKWARD] == angle_offset[BACKWARD] && o.nr_of_coils == nr_of_coils ; 
    }
    bool operator!=( const BLDCParameter &o ) const { 
        return o.angle_offset[FORWARD] != angle_offset[FORWARD] || o.angle_offset[BACKWARD] != angle_offset[BACKWARD] || o.nr_of_coils != nr_of_coils ; 
    }
    
#if defined(__amd64__)
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, " %+3d, %+3d, %+2d", angle_offset[FORWARD], angle_offset[BACKWARD], nr_of_coils);
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
    void set(float dt, float min, float max, float Kp, float Ki, float Kd){
        this->dt = dt, this->min = min, this->max = max, this->Kp = Kp, this->Ki = Ki, this->Kd = Kd;
    };
    bool operator==( const PIDParameter &o ) const { 
        return o.dt == dt && o.min == min && o.max == max && o.min == min && o.Kp == Kp && o.Ki == Ki && o.Kd == Kd ; 
    }
    bool operator!=( const PIDParameter &o ) const { 
        return o.dt != dt || o.min != min || o.max != max || o.min != min || o.Kp != Kp || o.Ki != Ki || o.Kd != Kd ; 
    }
#if defined(__amd64__)
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, " %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f, %+4.3f", dt, min, max, Kp, Ki, Kd);
        return std::string ( buf );
    }
#endif
};

class  ControlParameter {
public:

    static const int LEFT = 0;
    static const int RIGHT = 1;
    static const char* side_name(int i){
        switch(i){
            case LEFT:
                return "left";
            case RIGHT:
                return "right";
            default:
                return "na";
        }
    }
    enum Vehicle { 
        NA = 0, 
        MXR01 = 1, 
        MXR02 = 2};
    static const char* vehicle_name(Vehicle i){
        switch(i){
            case MXR01:
                return "mxr01";
            case MXR02:
                return "mxr02";
            case NA:
            default:
                return "na";
        }
    }

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
                config.bldc[LEFT].set(-65     , -65 - 90, 11);
                config.bldc[RIGHT].set(-80 + 90,      -80, 11);
                
                config.pid[LEFT].set(0.1, -1, 1, 0.2, 0.05, 0.01);
                config.pid[RIGHT].set(0.1, -1, 1, 0.2, 0.05, 0.01);
                break;
            case NA:
            default:
                config.bldc[LEFT].set( 0  , -90, 11);
                config.bldc[RIGHT].set( +90,   0, 11);
                
                config.pid[LEFT].set(0.1, -1, 1, 0.2, 0.05, 0.01);
                config.pid[RIGHT].set(0.1, -1, 1, 0.2, 0.05, 0.01);
                
        }
        return config;
    }
    bool operator==( const ControlParameter &o ) const { 
        return o.bldc[LEFT] == bldc[LEFT] && o.bldc[RIGHT] == bldc[RIGHT] && o.pid[LEFT] == pid[LEFT] && o.pid[RIGHT] == pid[RIGHT]; 
    }
    bool operator!=( const ControlParameter &o ) const { 
        return o.bldc[LEFT] != bldc[LEFT] || o.bldc[RIGHT] != bldc[RIGHT] || o.pid[LEFT] != pid[LEFT] || o.pid[RIGHT] != pid[RIGHT]; 
    }

#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const ControlParameter &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[[%s], [%s]], [ [%s], [%s]]",  bldc[LEFT].getToString().c_str(), bldc[RIGHT].getToString().c_str(), pid[LEFT].getToString().c_str(), pid[RIGHT].getToString().c_str());
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %s]", stamp.getToStringReadable().c_str(), getToString().c_str());
        return std::string ( buf );
    }
#endif
};
}
}
}


#endif // CAR_COM_OBJECTS_CONTROLPARAMETER_H
