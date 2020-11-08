/**
 * @author Markus Bader <markus.bader@mxien.ac.at>
 * @license Simplified BSD License
 */

#include "car/com/mc/interface.h"

using namespace car::com::mc;

uint32_t Interface::tx_count = 0;
uint32_t Interface::rx_count = 0;
uint16_t Interface::sync_count = 0;

void Interface::writeByte(uint8_t bt) {
    if (bt == 0x00) {
        serialbuffer[serialbuffer_used++] = 0x02;
        serialbuffer[serialbuffer_used++] = 0x30;
    } else if (bt == 0x01) {
        serialbuffer[serialbuffer_used++] = 0x02;
        serialbuffer[serialbuffer_used++] = 0x31;
    } else if (bt == 0x02) {
        serialbuffer[serialbuffer_used++] = 0x02;
        serialbuffer[serialbuffer_used++] = 0x32;
    } else if (bt == 0x04) {
        serialbuffer[serialbuffer_used++] = 0x02;
        serialbuffer[serialbuffer_used++] = 0x34;
    } else {
        serialbuffer[serialbuffer_used++] = bt;
    }
}

int8_t Interface::readByte(uint8_t* bt) {
    uint8_t temp;
    temp = serialbuffer[serialbuffer_used++];

    if (temp == 0x00) {
        // assert(false); // Start Delimiter
        return 1;
    } else if (temp == 0x01) {
        // assert(false); // End Delimiter
        return 1;
    } else if (temp == 0x02) {
        temp = serialbuffer[serialbuffer_used++];
        if (temp == 0x30) *bt = 0x00;
        if (temp == 0x31) *bt = 0x01;
        if (temp == 0x32) *bt = 0x02;
        if (temp == 0x34) *bt = 0x04;
        return 0;
	// assert(false);
    } else {
        *bt = temp;
        return 0;
    }
}

int Interface::send () {
    if ( Serial ) {
        time() = car::com::objects::Time::now();
	    this->seq = tx_count;
        serialbuffer_used = 0;
        char *c = ( char * ) this;
        serialbuffer[serialbuffer_used++] = 0x00;  // Start Delimiter
        for ( unsigned int i = 0; i < sizeof ( MessageHeader ); i++ ) {
            writeByte( (uint8_t) *(c++) );
        }
        for (unsigned int i = 0; i < this->size; i++ ) {
            writeByte( (uint8_t) buffer[i] );
        }
        serialbuffer[serialbuffer_used++] = 0x01;  // End Delimiter



        Serial.write((const char*) serialbuffer, (size_t) serialbuffer_used);
        //for (unsigned int i = 0; i < serialbuffer_used; i++)   Serial.write((char) serialbuffer[i]);


        //Serial.flush();
        //Serial.send_now();
        tx_count++;
    }
    return sizeof ( MessageHeader ) + this->size;
}

int Interface::receive() {
    while ( 1 ) {
        uint16_t temp;
        if ( Serial.available() ) {
            temp = Serial.read();
            if (temp == 0x00) { // Start Delimiter
                serialbuffer_used = 0;
            } else if (temp == 0x01) {
                break; // End Delimiter -> continue with parsing
            } else {
                // assert(serialbuffer_used < MAX_BUFFER_SIZE);
                serialbuffer[serialbuffer_used++] = temp;
            }
        } else {
            return 0; // Currently no more bytes to read
        }
    }

    reset();
    uint8_t rdByte;
    int rx_count = 0;

    serialbuffer_used = 0; // Used for reading

    char *c = ( char* ) this;
    for ( uint16_t i = 0; i < sizeof ( MessageHeader ); i++ ) {
        readByte(&rdByte);
        *c++ = rdByte;
        rx_count++;
    }

    if ( rx_count == sizeof ( MessageHeader ) ) {
        for ( uint16_t i = 0; i < this->size; i++ ) {
            readByte((uint8_t*) &this->buffer[i]);
            rx_count++;
        }
    }
    rx_count++;

    stack_pointer = 0;
    return rx_count;
}

void Interface::try_sync() {
    if ( car::com::objects::Time::offest().isSet() ) return;
    car::com::objects::Object object;
    for ( int i = 0; car::com::objects::Time::offest().isSet() == false; i++ ) {
        if ( i%10 == 0 ) {
            this->reset();
            this->push_object ( car::com::objects::Object ( car::com::objects::TYPE_SYNC_REQUEST ) );
            this->send();
        };
        if ( receive() ) {      /// check for messages
            while ( this->pop_object ( object ).isValid() ) {
                if ( object.type == car::com::objects::TYPE_SYNC ) {
                    car::com::objects::Time::compute_offset(time());
                    /*
                    car::com::objects::Time::offest() = time();
                    auto t = car::com::objects::TimePoint{};  /// epoch
                    //t = t + std::chrono::seconds(time().sec) + std::chrono::nanoseconds(time().nsec) - std::chrono::milliseconds(millis());
                    t = t + std::chrono::seconds(time().sec) + std::chrono::nanoseconds(time().nsec) - std::chrono::microseconds(micros()); 
                    car::com::objects::Time::offest().from(t);
                    */
                }
            }
        }
        delay ( 100 );
    }
}
