#ifndef CAR_COM_OBJECTS_ACTUATORS_H
#define CAR_COM_OBJECTS_ACTUATORS_H


#include <cstdint>
#include <cstring>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  Actuators {
public:
    Actuators() : rps ( 0 ), rad ( 0 ) {};
    Actuators ( float rps, float rad ) : rps ( rps ), rad ( rad ) {};
    float rps;
    float rad;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const Actuators &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, Actuators &o)
    {
        std::string str;
        getline (input, str);
	o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %4.2f, %4.2f]", rps, rad );
        return std::string ( buf );
    }
    bool setFromString ( const std::string &str ) {
        int start = str.find ( "[" );
        int end = str.find_last_of ( "]" );
        std::string data = str.substr ( start+1, end-1 );
        if ( sscanf ( data.c_str(), "%f,%f", &rps, &rad ) == EOF ) return false;
        return true;
    }
#endif

};
};
};
};


#endif // CAR_COM_OBJECTS_ACTUATORS_H
