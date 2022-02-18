#ifndef CAR_COM_MESSAGE_H
#define CAR_COM_MESSAGE_H

#include <car/com/objects/object.h>
#include <map>
#if defined(__amd64__)
#include <iostream>
#endif

namespace car {
namespace com {
    
/**
 * Map of objects indexed by type
 * @author Markus Bader <markus.bader@mxien.ac.at>
 **/
using Objects = std::map<objects::ObjectType, objects::Object>;

/**
 * Header for serial message
 * @author Markus Bader <markus.bader@mxien.ac.at>
 **/
struct MessageHeader {
public:
    MessageHeader();
    MessageHeader ( uint16_t size, uint32_t seq, const objects::Time &stamp );
    uint16_t size;                       /// total message size if size
    uint32_t seq;                        /// consecutively increasing ID
    objects::Time stamp;                 /// time since epoch
    void zeros();
    objects::Time &time();
    const objects::Time &time() const;
    
#if defined(__amd64__)
    friend std::ostream &operator << ( std::ostream &os, const MessageHeader &o ) {
        os << "[" << o.size <<  ", " << o.seq << ", " << o.time() << "]";
        return os;
    };    
    std::string getToStringReadable() const {
        char buf[0xFF];
        sprintf ( buf, "[%s, %d, %d bytes  ]", stamp.getToStringReadable().c_str(), seq, size);
        return std::string ( buf );
    }
#endif
};

class Message : public MessageHeader {
protected:
public:
    Message();
    static const int MAX_BUFFER_SIZE = 0x8FF;
    char buffer[MAX_BUFFER_SIZE];
    int stack_pointer;
    bool isValid();
    void reset();
    unsigned int deserialize ( const char *data, unsigned int len );
    bool push_sync () ;
    car::com::objects::Object &pop_object ( car::com::objects::Object &object ) ;
    Message &push_object ( const car::com::objects::Object &object );
};


}
}

#endif // CAR_COM_MESSAGE_H
