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
    Pose() : x ( 0 ), y ( 0 ), theta ( 0 ) {};
    Pose ( float x, float y, float theta ) : x ( x ), y ( y ), theta ( theta ) {};
    float x;
    float y;
    float theta;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Pose &o ) {
        os << "[" << o.x << ", " << o.y  << ", " << o.theta <<  "]";
        return os;
    };
#endif
};
};
};
};

#endif // CAR_COM_OBJECTS_POSE_H
