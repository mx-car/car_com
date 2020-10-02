#ifndef CAR_COM_OBJECTS_TIME_H
#define CAR_COM_OBJECTS_TIME_H

#include <cstdint>

#if defined(__amd64__)
#include <iostream>
#else
#include <Arduino.h>
#endif


namespace car {
namespace com {
namespace objects {


class  Time {
private:
    static Time OFFSET;
    static bool CLOCK_SYNC;
public:
    Time() : sec ( 0 ) , nsec ( 0 ) {};
    Time ( int32_t sec, int32_t nsec )  :sec ( sec ) , nsec ( nsec ) {};
    int32_t sec;      /// seconds (stamp_secs) since epoch
    int32_t nsec;     /// nanoseconds = 0.000000001 sec since stamp_secsf
    void add(int32_t millisecond){
        int32_t dsec = ( millisecond / 1000 ) + sec;
        int32_t dnsec = ( millisecond % 1000UL ) * 1000000UL + nsec;
        sec = dsec + dnsec / 1000000UL;
        nsec = dnsec % 1000000UL;
    }
    static Time &offest() {
        return OFFSET;
    }
    bool isSet() const {
        return !((sec == 0) && (nsec == 0));
    }
    void now(){
#if defined(__amd64__)
#else
        sec = OFFSET.sec;
        nsec = OFFSET.sec;
        add(millis());
#endif
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
