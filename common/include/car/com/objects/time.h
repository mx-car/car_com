#ifndef CAR_COM_OBJECTS_TIME_H
#define CAR_COM_OBJECTS_TIME_H

#include <cstdint>

#if defined(__amd64__)
#include <iostream>
#endif


namespace car {
namespace com {
namespace objects {


class  Time {
public:
    Time() : sec ( 0 ) , nsec ( 0 ) {};
    Time ( int32_t sec, int32_t nsec )  :sec ( sec ) , nsec ( nsec ) {};
    int32_t sec;      /// seconds (stamp_secs) since epoch
    int32_t nsec;     /// nanoseconds = 0.000000001 sec since stamp_secs
    void now(){
    }
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Time &o ) {
        os << "[" << o.sec << ", " << o.nsec <<  "]";
        return os;
    };
#endif
};
};
};
};

#endif // CAR_COM_OBJECTS_TIME_H
