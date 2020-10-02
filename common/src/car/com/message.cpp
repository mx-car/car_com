
#include <car/com/message.h>
#include <cstdlib>
#include <cstring>

using namespace car::com;



MessageHeader::MessageHeader()
    : size ( 0 )
    , seq ( 0 )
    , stamp() {};
MessageHeader::MessageHeader ( uint16_t size, uint32_t seq, const car::com::objects::Time &stamp )
    : size ( size )
    , seq ( seq )
    , stamp ( stamp ) {};

void MessageHeader::zeros() {
    size = 0, seq = 0, stamp.sec = 0, stamp.nsec = 0;
}

car::com::objects::Time &MessageHeader::time(){
    return stamp;
}

const car::com::objects::Time &MessageHeader::time() const{
    return stamp;
}

Message::Message()
    : MessageHeader()
    , stack_pointer ( 0 ) {};

bool Message::isValid() {
    return ( this->size < MAX_BUFFER_SIZE );
}
void Message::reset() {
    this->zeros();
    stack_pointer = 0;
    memset ( buffer, '\0', MAX_BUFFER_SIZE );
}

unsigned int Message::deserialize ( const char *data, unsigned int len ) {
    stack_pointer = 0;
    if ( len <  sizeof ( MessageHeader ) ) return -1;
    * ( ( MessageHeader * ) this )  = * ( ( MessageHeader * ) data );
    for ( std::size_t i = 0; i < this->size; i++ ) {
        buffer[i] = data[sizeof ( MessageHeader ) +i];
    }
    return this->size + sizeof ( MessageHeader );
}

bool Message::push_sync () {
    if ( this->size + sizeof ( car::com::objects::ObjectHeader ) <= MAX_BUFFER_SIZE ) {
        car::com::objects::ObjectHeader *object_header = ( car::com::objects::ObjectHeader* )  buffer + this->size;
        object_header->type = car::com::objects::TYPE_SYNC;
        object_header->size = 0;
        this->size += sizeof ( car::com::objects::ObjectHeader );
        return true;
    } else {
        return false;
    }
}
car::com::objects::Object &Message::pop_object ( car::com::objects::Object &object ) {
    if ( stack_pointer < this->size ) {
        stack_pointer+= object.deserialize ( &buffer[stack_pointer], this->size - stack_pointer );
    } else {
        object.error();
    }
    return object;
}

Message &Message::push_object ( const car::com::objects::Object &object ) {
    if ( this->size + sizeof ( car::com::objects::ObjectHeader ) +  object.size <= MAX_BUFFER_SIZE ) {
        memcpy ( buffer + this->size, &object, sizeof ( car::com::objects::ObjectHeader ) );
        this->size += sizeof ( car::com::objects::ObjectHeader );
        memcpy ( buffer + this->size, object.buffer, object.size );
        this->size += object.size;
    }
    return *this;
}
