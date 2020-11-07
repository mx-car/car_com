#ifndef CAR_COM_OBJECTS_RAWMEASUREMENT_H
#define CAR_COM_OBJECTS_RAWMEASUREMENT_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  RawMeasurement {
public:
    RawMeasurement()
    : stamp(), rps{0.0, 0.0}, torque{0.0, 0.0}{
    };
    RawMeasurement ( float rps_left, float rps_right, float torque_left, float rps_torque_right ) : stamp() {
        set(rps_left, rps_right, torque_left, rps_torque_right);
    }
    void set(float rps_left, float rps_right, float torque_left, float rps_torque_right ){
        rps[LEFT] = rps_left, rps[RIGHT] = rps_right, torque[LEFT] = torque_left, torque[RIGHT] = rps_torque_right;
    }
    Time stamp;
    float rps[2];
    float torque[2];
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const RawMeasurement &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.3f, %+4.3f, %+4.3f, %+4.3f]", rps[LEFT], rps[RIGHT], torque[LEFT], torque[RIGHT] );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.3f rps, %+4.3f rps, %+4.3f Nm, %+4.3f Nm]", stamp.getToStringReadable().c_str(), rps[LEFT], rps[RIGHT], torque[LEFT], torque[RIGHT] );
        return std::string ( buf );
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_RAWMEASUREMENT_H
