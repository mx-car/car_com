#ifndef CAR_COM_OBJECTS_ACKERMANNSTATE_H
#define CAR_COM_OBJECTS_ACKERMANNSTATE_H


#include <cstdint>
#include <cstring>
#include <array>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  AckermannState {
public:
    static const int LEFT = 0;
    static const int RIGHT = 1;
    static const int MODE_NA = 0;
    static const int MODE_PWM = 1;
    static const int MODE_VELOCITY = 2;
    AckermannState()
        : stamp()
        , coubled{0, 0}
        , v{0, 0}
        , steering(0.)
        , mode(MODE_NA) {
    };
    AckermannState (const AckermannState &o)
        : stamp(o.stamp)
        , coubled{o.coubled[LEFT], o.coubled[RIGHT]}
        , v{o.v[LEFT], o.v[RIGHT]}
        , steering(o.steering) 
        , mode(o.mode) {};

    AckermannState (float v_left, float v_right, float steering, uint16_t mode, bool coubled = true, bool now = true)
        : stamp(now)
        , coubled{coubled, coubled}
        , v{v_left, v_right}
        , steering(steering)
        , mode(mode) {}
        
    void set(float v_left, float v_right, float steering, uint16_t mode, bool coubled = true, bool now = true) {
        v[LEFT] = v_left, v[RIGHT] = v_right, this->steering = steering, this->mode = mode;
        this->coubled[LEFT] = coubled, this->coubled[RIGHT] = coubled;
        if(now) stamp = Time::now();
    }

    template <class T>
    void copy_from(const T &o) {
        coubled[LEFT] = o.coubled[LEFT], coubled[RIGHT] = o.coubled[RIGHT];
        v[LEFT] = o.v[LEFT], v[RIGHT] = o.v[RIGHT];
        steering = o.steering;
        mode = o.mode;
    }
    template <class T>
    void copy_to(T &o) const{
        o.coubled[LEFT] = coubled[LEFT];
        o.coubled[RIGHT] = coubled[RIGHT];
        o.v[LEFT] = v[LEFT];
        o.v[RIGHT] = v[RIGHT];
        o.steering = steering;
        o.mode = mode;
    }

    Time stamp;
    bool coubled[2];
    float v[2];
    float steering;
    uint16_t mode; 
    
#if defined(__amd64__)
    const char* modet_type() const{
        switch(mode){
            case MODE_NA:
                return "na";
                break;
            case MODE_PWM:
                return "pwm";
                break;
            case MODE_VELOCITY:
                return "velocity";
                break;
            default:
                return "unkown";
        }
    }
    
    friend std::ostream &operator << ( std::ostream &os, const AckermannState &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %d, %+4.3f, %d, %+4.3f, %+4.3f]", coubled[LEFT], v[LEFT], coubled[RIGHT], v[RIGHT],  steering);
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %d, %+4.3fm/s, %d, %+4.3fm/s, %+4.3frad, %s ]", stamp.getToStringReadable().c_str(), coubled[LEFT], v[LEFT], coubled[RIGHT], v[RIGHT],  steering, modet_type());
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_ACKERMANNSTATE_H
