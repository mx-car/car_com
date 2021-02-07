#ifndef CAR_COM_OBJECTS_TWIST_H
#define CAR_COM_OBJECTS_TWIST_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  Twist {
public:
    Twist() 
    :v(0), w(0){
    };
    Twist ( float velocity, float rotoation ) {
        set(velocity, rotoation);
    }
    void set(float velocity, float rotoation){
        v = velocity, w = rotoation;
    }
    template <class T>
    void set(T o){
        v = o.v;
        w = o.w;
    }
    float v;
    float w;
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Twist &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, Twist &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.2f, %+4.2f]", v, w );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.2fm/s, %+4.2frad/s ]", v, w );
        return std::string ( buf );
    }
    bool setFromString ( const std::string &str ) {
        int start = str.find ( "[" );
        int end = str.find_last_of ( "]" );
        std::string data = str.substr ( start+1, end-1 );
        if ( sscanf ( data.c_str(), "%f,%f", &v, &w ) == EOF ) return false;
        return true;
    }
#endif
};

class  TwistStamped : public Twist {
public:
    TwistStamped() 
    : Twist(){
    };
    TwistStamped ( float velocity, float rotoation ) {
        set(velocity, rotoation);
    }
    void set(float velocity, float rotoation){
        v = velocity, w = rotoation;
    }
    Time stamp;
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const TwistStamped &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, TwistStamped &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.2f, %+4.2f]", v, w );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.2fm/s, %+4.2frad/s ]", stamp.getToStringReadable().c_str(), v, w );
        return std::string ( buf );
    }
    bool setFromString ( const std::string &str ) {
        int start = str.find ( "[" );
        int end = str.find_last_of ( "]" );
        std::string data = str.substr ( start+1, end-1 );
        if ( sscanf ( data.c_str(), "%f,%f", &v, &w ) == EOF ) return false;
        return true;
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_TWIST_H
