#ifndef CAR_COM_OBJECTS_TIME_H
#define CAR_COM_OBJECTS_TIME_H

#include <cstdint>

#if defined(__amd64__)
#include <iostream>
#include <chrono>
#else
#include <Arduino.h>
#endif


namespace car {
namespace com {
namespace objects {

static const int LEFT = 0;
static const int RIGHT = 1;

#if defined(__amd64__)
using Clock = std::chrono::high_resolution_clock;
using TimePoint = std::chrono::time_point<Clock>;
#endif

class  Duration {
public:
    int32_t sec;      /// seconds (stamp_secs) since epoch
    int32_t nsec;     /// nanoseconds = 0.000000001 sec since stamp_secsf
    Duration() : sec ( 0 ), nsec ( 0 ) {};
    Duration ( int32_t sec, int32_t nsec )  :sec ( sec ), nsec ( nsec ) {};
    float toSec() {
        float second = ((float) nsec) / 1000000000. + (float) sec;
        return second;
    }
};

class  Time {
private:
    static Time OFFSET;
    static uint32_t OFFSET_MICROS;
    static bool CLOCK_SYNC;
public:
    Time() : sec ( 0 ), nsec ( 0 ) {};
    Time(bool now) : sec ( 0 ), nsec ( 0 ) {
        if(now) {
#if defined(__amd64__)
            this->from(Clock::now());
#else
            sec = OFFSET.sec, nsec = OFFSET.nsec;
            this->add_microseconds(micros());
#endif
        }
    };
    Time ( int32_t sec, int32_t nsec )  :sec ( sec ), nsec ( nsec ) {};
    int32_t sec;      /// seconds (stamp_secs) since epoch
    int32_t nsec;     /// nanoseconds = 0.000000001 sec since stamp_secsf
    void nomralize() {
        sec += nsec % 1000000000UL;
    }
    void add_milliseconds(int64_t millisecond) {
        int64_t dsec = ( millisecond / 1000 ) + sec;
        int64_t dnsec = ( millisecond % 1000UL ) * 1000000UL + nsec;
        sec = dsec + dnsec / 1000000000UL;
        nsec = dnsec % 1000000000UL;
    }
    void add_microseconds(int64_t microsecond) {
        int64_t dsec = microsecond / 1000000UL + sec;
        int64_t dmsec = microsecond % 1000000UL;
        int64_t dnsec = dmsec * 1000UL + nsec;
        sec = dsec + dnsec / 1000000000UL;
        nsec = dnsec % 1000000000UL;
    }
    static Time &offest() {
        return OFFSET;
    }
    bool isSet() const {
        return !((sec == 0) && (nsec == 0));
    }
#if defined(__amd64__)
    void from(const TimePoint &src) {
        std::chrono::duration<int64_t, std::nano> dt = src.time_since_epoch();
        std::chrono::seconds  dsec = std::chrono::duration_cast<std::chrono::seconds>(dt);
        std::chrono::duration<int64_t,  std::nano> dnsec = dt - dsec;
        sec = dsec.count();
        nsec = std::chrono::duration_cast<std::chrono::nanoseconds>(dnsec).count();
    }
    void to(TimePoint &des) const {
        des = TimePoint{} + std::chrono::seconds(sec) + std::chrono::nanoseconds(nsec);
    }
    template<typename TimeObject>
    void set(const TimeObject &s) {
        sec = s.sec, nsec = s.nsec;
    }
#endif

#if defined(__amd64__)
#else
    static void compute_offset(Time &t) {
        int64_t t_now = micros();
        int64_t t_now_sec = (t_now  / 1000000UL);
        int64_t t_now_nsec = (t_now  % 1000000UL) * 1000UL;
        OFFSET.nsec = t.nsec - t_now_nsec;
        OFFSET.sec = t.sec - t_now_sec;
        if(OFFSET.nsec < 0) {
            OFFSET.sec = OFFSET.sec - 1;
            OFFSET.nsec = 1000000000UL + OFFSET.nsec;
        }
        OFFSET_MICROS = OFFSET.nsec/1000UL + OFFSET.sec * 1000000UL;
    }
    /**
     * returns the time based on the micro clock adding the mu clock offset
     * @param microsecond
     **/
    static Time fromMicros(int64_t microsecond) {
        Time t(OFFSET.sec, OFFSET.nsec);
        t.add_microseconds(microsecond);
        return t;
    }
    /**
     * returns the time based on the micro clock substracting the mu clock offset
     * @param t
     **/
    static uint32_t toMicros(const Time& t) {
        uint32_t microsecond = t.nsec/1000UL + t.sec * 1000000UL - OFFSET_MICROS;
        return microsecond;
    }
#endif

    /**
     * returns the current time
     * @pre on the micro controller Time::compute_offset must be called first
     **/
    static Time now() {
#if defined(__amd64__)
        Time t;
        t.from(Clock::now());
        return t;
#else
        Time t = fromMicros(micros());
#endif
        return t;
    }

#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Time &o ) {
        os << "[" << o.sec << ", " << o.nsec <<  "]";
        return os;
    };
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[ %02d:%02d:%02d,%09d ]", sec%86400/3600, sec%3600/60, sec%60, nsec );
        return std::string ( buf );
    }
#endif


    friend Duration operator-(const Time &lhs, const Time &rhs)
    {
        Duration d(lhs.sec - rhs.sec, lhs.nsec - rhs.nsec);
        /*
        int64_t dnsec = d.nsec % 1000000000UL;
        d.sec += dnsec / 1000000000UL;
        d.nsec -= dnsec;
        */
        return d;
    }
};
};
};
};

#endif // CAR_COM_OBJECTS_TIME_H
