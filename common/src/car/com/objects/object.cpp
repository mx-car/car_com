
#include <car/com/objects/object.h>
#include <cstdlib>


using namespace car::com::objects;

Time Time::OFFSET ( 0,0 );


ObjectHeader::ObjectHeader()
    : type ( 0 )
    , size ( 0 ) {};
ObjectHeader::ObjectHeader ( const ObjectHeader &o )
    : type ( o.type ), size ( o.size ) {};
ObjectHeader::ObjectHeader ( ObjectType type, int16_t size )
    : type ( type )
    , size ( size ) {};



Object::Object()
    : ObjectHeader()
    , buffer ( NULL ) {};
Object::Object ( ObjectType type )
    : ObjectHeader ( type, 0 )
    , buffer ( NULL ) {};
Object::Object ( const Object &o )
    : ObjectHeader ( o )
    , buffer ( NULL )  {
    if ( ( o.buffer != NULL ) && ( this->size > 0 ) ) {
        alloc ( this->size );
        copy_buffer_from ( o.buffer );
    }
};
Object::~Object () {
    dealloc();
};

void Object::alloc ( size_t size ) {
    dealloc();
    if ( size > 0 ) buffer = ( char* ) malloc ( size );
}
void Object::dealloc() {
    if ( buffer != NULL ) free ( buffer );
    buffer = NULL;
}
void Object::copy_buffer_from ( const char *src ) {
    if ( buffer != NULL ) {
        for ( std::size_t i = 0; i < this->size; i++ ) {
            buffer[i] = src[i];
        }
    }
}

Object& Object::empty() {
    this->type = TYPE_EMPTY;
    this->size = 0;
    dealloc();
    return *this;
}
Object& Object::error() {
    this->type = TYPE_ERROR;
    this->size = 0;
    dealloc();
    return *this;
}
bool Object::isValid() {
    return ! ( ( this->type == TYPE_ERROR ) || ( this->size < 0 ) );
}
unsigned int Object::deserialize ( const char *data, unsigned int len ) {
    if ( len <  sizeof ( ObjectHeader ) ) {
        this->error();
        return len;
    }
    * ( ( ObjectHeader * ) this )  = * ( ( ObjectHeader * ) data );
    if ( len-sizeof ( ObjectHeader ) < this->size ) {
        this->error();
        return len;
    }
    alloc ( this->size );
    copy_buffer_from ( data + sizeof ( ObjectHeader ) );
    return  this->size + sizeof ( ObjectHeader );
}
Object& Object::operator= ( const Object &o ) {
    this->size = o.size;
    this->type = o.type;
    alloc ( this->size );
    copy_buffer_from ( o.buffer );
    return *this;
}
