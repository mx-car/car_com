#ifndef CAR_COM_OBJECTS_ERROR_H
#define CAR_COM_OBJECTS_ERROR_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>
#include <car/com/objects/pose.h>
#include <car/com/objects/twist.h>
#include <car/com/objects/wheel.h>
#include <car/com/objects/array.h>
#include <car/com/objects/ackermann_config.h>
#include <car/com/objects/command_ackermann.h>

#if defined(__amd64__)
#include <iostream>
#include <sstream>
#endif

namespace car {
namespace com {
namespace objects {
    
    
class  Error {
public:
    Error(){
        info[0] = 'n';
        info[1] = 'a';
        info[2] = '\0';
    };
    Time stamp;
    uint16_t code;
    char info[0x1F];
    
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Error &o ) {
        os << o.getToString();
        return os;
    };
    std::string getToString() const {
        std::stringstream ss;
        ss << "[ " << stamp << ", " << code <<  ", " << info << "]" << std::endl;
        return ss.str();
    }
    std::string getToStringReadable() const {
        std::stringstream ss;
        ss << "[ " << stamp << ", " << code <<  ", " << info << "]" << std::endl;
        return ss.str();
    }
#endif
};
};
};
};

#endif // CAR_COM_OBJECTS_ERROR_H
