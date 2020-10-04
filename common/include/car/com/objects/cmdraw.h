#ifndef CAR_COM_OBJECTS_CMDRAW_H
#define CAR_COM_OBJECTS_CMDRAW_H


#include <cstdint>
#include <cstring>

#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
namespace objects {

class  CmdRaw {
public:
    static const int LEFT = 0;
    static const int RIGHT = 1;
    CmdRaw() {
        set(0, 0, 0);
    };
    CmdRaw ( float rps_left, float rps_right, float steering ) {
        set(rps_left, rps_right, steering);
    };
    float rps[2];
    float steering;
    
    CmdRaw &set(float rps_left, float rps_right, float steering){
        rps[LEFT] = rps_left, rps[RIGHT] = rps_right, this->steering = steering;
        return *this;
    }
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const CmdRaw &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, CmdRaw &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        char buf[0xFF];
        sprintf ( buf, "[ %4.2f, %4.2f, %4.2f]", rps[LEFT], rps[RIGHT], steering );
        return std::string ( buf );
    }
    bool setFromString ( const std::string &str ) {
        int start = str.find ( "[" );
        int end = str.find_last_of ( "]" );
        std::string data = str.substr ( start+1, end-1 );
        if ( sscanf ( data.c_str(), "%f,%f,%f", &rps[LEFT], &rps[RIGHT], &steering ) == EOF ) return false;
        return true;
    }
#endif

};
};
};
};


#endif // CAR_COM_OBJECTS_CMDRAW_H
