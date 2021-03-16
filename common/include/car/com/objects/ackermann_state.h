#ifndef CAR_COM_OBJECTS_ACKERMANNSTATE_H
#define CAR_COM_OBJECTS_ACKERMANNSTATE_H


#include <cstdint>
#include <cstring>
#include <array>
#include <mx/objects/ackermann_state.h>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  AckermannState : public mx::objects::AckermannState{
public:
    AckermannState()
        : mx::objects::AckermannState()
        , stamp() {};
    AckermannState (const AckermannState &o)
        : mx::objects::AckermannState(o)
        , stamp(o.stamp){};

    AckermannState (float v_left, float v_right, float steering, uint16_t mode, bool coubled = true, bool now = true)
        : mx::objects::AckermannState(v_left,  v_right, steering, mode, coubled)
        , stamp(now){}
        
    void set(float v_left, float v_right, float steering, uint16_t mode, bool coubled = true, bool now = true) {
        mx::objects::AckermannState::set(v_left, v_right, steering, mode, coubled);
        if(now) stamp = Time::now();
    }
    Time stamp;
    
#if defined(__amd64__)
    
    friend std::ostream &operator << ( std::ostream &os, const AckermannState &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %s ]", stamp.getToStringReadable().c_str(), mx::objects::AckermannState::getToStringReadable().c_str());
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_ACKERMANNSTATE_H
