#ifndef CAR_COM_OBJECTS_CMDRAW_H
#define CAR_COM_OBJECTS_CMDRAW_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/state.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  CmdRaw : public State {
public:
    CmdRaw() : State() {
    };
    CmdRaw ( float rps_left, float rps_right, float steering ) : State(rps_left, rps_right, steering) {};

};
};
};
};


#endif // CAR_COM_OBJECTS_CMDRAW_H
