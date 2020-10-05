/**
 * @author Markus Bader <markus.bader@mx-robotics.com>
 * @license Simplified BSD License
 */


#ifndef CAR_COM_MC_INTERFACE_H
#define CAR_COM_MC_INTERFACE_H

#include <Arduino.h>
#include <car/com/message.h>

namespace car {
namespace com {
namespace mc {


class  Interface : public Message {
public:
    static uint32_t tx_count;  	/// static variale to increase message count
    static uint32_t rx_count;  	/// static variale to increase message count
    static uint16_t sync_count; 	/// number of sync attempts
    Interface() : Message() {};
    int send();
    int receive();
    void try_sync();
private:
    void writeByte( uint8_t bt );
    int8_t readByte( uint8_t* bt );
    uint8_t serialbuffer[MAX_BUFFER_SIZE];
    uint16_t serialbuffer_used;
};
};
};
};
#endif //CAR_COM_MC_INTERFACE_H

