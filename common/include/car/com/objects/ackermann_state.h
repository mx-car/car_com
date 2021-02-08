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
    AckermannState()
        : stamp()
        , v{0, 0}
        , steering(0.) {
    };
    AckermannState (const AckermannState &o)
        : stamp(o.stamp)
        , v{o.v[LEFT], o.v[RIGHT]}
        , steering(o.steering) {};

    AckermannState (float v_left, float v_right, float steering, bool now = true)
        : stamp(now)
        , v{v_left, v_right}
        , steering(steering) {}
    void set(float v_left, float v_right, float steering, bool now = true) {
        v[LEFT] = v_left, v[RIGHT] = v_right, this->steering = steering;
        if(now) stamp = Time::now();
    }

    template <class T>
    void set(T o) {
        v[LEFT] = o.v[LEFT], v[RIGHT] = o.v[RIGHT];
        steering = o.steering;
    }

    Time stamp;
    float v[2];
    float steering;

#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const AckermannState &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f]", v[LEFT], v[RIGHT],  steering);
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.3fm/s, %+4.3fm/s, %+4.3frad ]", stamp.getToStringReadable().c_str(), v[LEFT], v[RIGHT],  steering );
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_ACKERMANNSTATE_H
