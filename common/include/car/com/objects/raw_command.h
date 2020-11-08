#ifndef CAR_COM_OBJECTS_RAWCOMMAND_H
#define CAR_COM_OBJECTS_RAWCOMMAND_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  RawCommand {
public:
    RawCommand() : stamp() , motor{0.0, 0.0}{
    };
    RawCommand ( float left, float right, float steering ) : stamp(){
        set(left, right, steering);
    }
    void set(float left, float right, float steering){
        motor[0] = left, motor[1] = right, servo = steering;
    }
    Time stamp;
    float roation[2];
    float steering;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const RawCommand &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, RawCommand &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %+4.2f, %+4.2f, %+4.2f]", motor[LEFT], motor[RIGHT], servo );
        return std::string ( buf );
    }
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %+4.2f, %+4.2f, %+4.2f ]", stamp.getToStringReadable().c_str(), motor[LEFT], motor[RIGHT], servo );
        return std::string ( buf );
    }
    bool setFromString ( const std::string &str ) {
        int start = str.find ( "[" );
        int end = str.find_last_of ( "]" );
        std::string data = str.substr ( start+1, end-1 );
        if ( sscanf ( data.c_str(), "%f,%f,%f", &motor[LEFT], &motor[RIGHT], &servo ) == EOF ) return false;
        return true;
    }
#endif
};
};
};
};


#endif // CAR_COM_OBJECTS_RAWCOMMAND_H
