#ifndef CAR_COM_OBJECTS_POSE_H
#define CAR_COM_OBJECTS_POSE_H

#include <cstdint>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  Pose {
public:
    Pose() : stamp(), x ( 0 ), y ( 0 ), theta ( 0 ) {};
    Pose ( float x, float y, float theta ) : stamp(), x ( x ), y ( y ), theta ( theta ) {};
    Time stamp;
    float x;
    float y;
    float theta;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Pose &o ) {
        os << "[" << o.x << ", " << o.y  << ", " << o.theta <<  "]";
        return os;
    };
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[ %4.2f m, %4.2f m, %4.2f rad]", x, y, theta );
        return std::string ( buf );
    }
#endif
};
};
};
};

#endif // CAR_COM_OBJECTS_POSE_H
