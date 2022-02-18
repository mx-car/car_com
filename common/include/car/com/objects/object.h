#ifndef CAR_COM_OBJECTS_OBJECT_H
#define CAR_COM_OBJECTS_OBJECT_H

#include <car/com/objects/time.h>
#include <car/com/objects/pose.h>
#include <car/com/objects/text.h>
#include <car/com/objects/twist.h>
#include <car/com/objects/error.h>
#include <car/com/objects/ackermann_config.h>
#include <car/com/objects/ackermann_state.h>
#include <car/com/objects/command_ackermann.h>
#include <car/com/objects/state_ackermann.h>
#include <car/com/objects/control_parameters.h>
#include <cstdio>

namespace car {
namespace com {
namespace objects {

static const uint16_t TYPE_ERROR = 0;    /// error on decodeding
static const uint16_t TYPE_EMPTY = 1;    /// no type encoded
static const uint16_t TYPE_SYNC_REQUEST = 10; /// sync request type
static const uint16_t TYPE_SYNC = 11;    /// sync type

static const uint16_t TYPE_TEXT = 100;
static const uint16_t TYPE_POSE = 200;
static const uint16_t TYPE_MOTION_POSE_ESTIMATED = 201;
static const uint16_t TYPE_MOTION_POSE_COVARIANCE_ESTIMATED = 251;
static const uint16_t TYPE_POSE_STAMPED = 210;
static const uint16_t TYPE_ACKERMANN_CONFIG = 400;
static const uint16_t TYPE_ACKERMANN_CMD = 401;
static const uint16_t TYPE_ACKERMANN_STATE = 402;
static const uint16_t TYPE_MOTOR = 500;
static const uint16_t TYPE_MOTOR_STATE = 501;
static const uint16_t TYPE_MOTOR_PID = 502;
static const uint16_t TYPE_SERVO = 600;
static const uint16_t TYPE_SERVO_STATE = 601;
static const uint16_t TYPE_SERVO_PID = 602;
static const uint16_t TYPE_IMU_ACCELEROMETER = 700;
static const uint16_t TYPE_IMU_GYROSCOPE = 701;
static const uint16_t TYPE_IMU_MAGNETOMETER = 702;
static const uint16_t TYPE_IMU_ENVIRONMENT = 703;
static const uint16_t TYPE_COMMAND_ACTUATORS = 5010;
static const uint16_t TYPE_COMMAND_MOTOR_PID = 5030;
static const uint16_t TYPE_COMMAND_SERVO_PID = 5040;
static const uint16_t TYPE_COMMAND_ACKERMANN_CONFIG = 5050;
static const uint16_t TYPE_COMMAND_DELAY_MICROS = 5060;
static const uint16_t TYPE_CONTROL_CONFIG = 5070;

static const uint16_t TYPE_STATE_RAW = 6000;
static const uint16_t TYPE_RAW_COMMAND = 6010;
static const uint16_t TYPE_STATE_ACKERMANN = 6020;
static const uint16_t TYPE_COMMAND_ACKERMANN = 6030;
static const uint16_t TYPE_CONFIG_ACKERMANN = 6040;
static const uint16_t TYPE_CONTROL_PARAMETER = 6050;
static const uint16_t TYPE_ARRAY16SC1 = 6101;
static const uint16_t TYPE_ARRAY16SC4 = 6104;
static const uint16_t TYPE_ARRAY16SC8 = 6108;
static const uint16_t TYPE_ARRAY16FC1 = 6201;
static const uint16_t TYPE_ARRAY16FC4 = 6204;
static const uint16_t TYPE_ARRAY16FC8 = 6208;

typedef uint16_t ObjectType;

class ObjectHeader {
public:
    ObjectHeader();
    ObjectHeader ( const ObjectHeader &o );
    ObjectHeader ( ObjectType type, int16_t size );
    ObjectType type;
    int16_t size;
};

class Object : public ObjectHeader {
public:
    char* buffer;
    Object();
    Object ( ObjectType type );
    Object ( const Object &o );
    template <class T> Object ( const T& src, ObjectType type ) : ObjectHeader ( type, sizeof ( T ) ), buffer ( NULL ) {
        set ( src, type );
    }
    ~Object ();
    Object& empty() ;
    Object& error() ;
    bool isValid() ;
    /**
     * @param data array with the serialized object
     * @param len array length
     * @return number of consumed bytes or the length argument
     */
    unsigned int deserialize ( const char *data, unsigned int len ) ;
    Object& operator= ( const Object &o );
    template <class T>const T& set ( const T& src, uint16_t type ) {
        this->type = type;
        this->size = sizeof ( T );
        alloc ( this->size );
        memcpy ( buffer, &src, this->size );
        return src;
    }
    template <class T> T& get ( T& des ) const {
        if ( ( sizeof ( T ) == size ) && ( size > 0 ) ) {
            memcpy ( (void*) &des, buffer, this->size );
        }
        return des;
    }
protected:
    void alloc ( size_t size );
    void dealloc();
    void copy_buffer_from ( const char *src );
};
}
}
}

#endif // CAR_COM_OBJECTS_OBJECT_H
