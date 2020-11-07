#ifndef CAR_COM_OBJECTS_RACECAR_H
#define CAR_COM_OBJECTS_RACECAR_H


#include <cstdint>
#include <cstring>
#include <car/com/objects/time.h>
#include <car/com/objects/pose.h>
#include <car/com/objects/raw_command.h>
#include <car/com/objects/raw_measurement.h>
#include <car/com/objects/twist.h>
#include <car/com/objects/description_vehicle.h>

#if defined(__amd64__)
#include <iostream>
#include <sstream>
#endif

namespace car {
namespace com {
namespace objects {

    
class  RaceCar {
public:
    RaceCar() {
        set(0, 0, 0);
    };
    RaceCar ( float rps_left, float rps_right, float steering ) {
        stamp = Time::now();
        set(rps_left, rps_right, steering);
    };
    Time stamp;
    Twist twist;
    RawCommand motors;
    DescriptionVehicle description;
    RawMeasurement measurments;
    Pose  pose;
    
    RaceCar &set(float rps_left, float rps_right, float steering){
        motors.set(rps_left, rps_right, steering);
        return *this;
    }
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const RaceCar &o ) {
        os << o.getToString();
        return os;
    };
    friend std::istream& operator>>(std::istream &input, RaceCar &o)
    {
        std::string str;
        getline (input, str);
        o.setFromString(str);
        return input;
    }
    std::string getToString() const {
        std::stringstream ss;
        ss << "[ " << stamp << ", " << twist << ", " << motors << ", " << description << ", " << measurments << ", " << pose << "]" << std::endl;
        return ss.str();
    }
    std::string getToStringReadable() const {
        std::stringstream ss;
        ss << stamp.getToStringReadable().c_str() << std::endl;
        ss << "Twist        : "  << twist.getToStringReadable() << std::endl;
        ss << "Motors       : "  << motors.getToStringReadable() << std::endl;
        ss << "Description  : "  << description.getToStringReadable() << std::endl;
        ss << "Measurements : "  << measurments.getToStringReadable() << std::endl;
        ss << "Pose         : "  << pose.getToStringReadable() << std::endl;
        return ss.str();
    }
    bool setFromString ( const std::string &str ) {
        return motors.setFromString(str);;
    }
#endif

};
};
};
};


#endif // CAR_COM_OBJECTS_CAR_H
