/*
  Blink
  Turns on an LED on for one second, then off for one second, repeatedly.

  This example code is in the public domain.
 */

#include <Arduino.h>

#include <car/com/objects/text.h>
#include <car/com/mc/interface.h>
#include <car/time/cycle_rate.h>

car::com::mc::Interface msg_tx;   				/// object to hande the serial communication
car::com::mc::Interface msg_rx;   				/// object to hande the serial communication
car::time::CycleRate cycle_rate(1000);          /// object for a constant cycle control
car::com::objects::Text text;                   /// object to send

// the setup routine runs once when you press reset:
void setup() {
  Serial.begin ( 115200 );			/// init serial
  while (!Serial);
  msg_rx.try_sync();   			    /// blocks until a sync message arrives
}

// the loop routine runs over and over again forever:
void loop() {
  if(cycle_rate.passed() > 0){
        msg_tx.reset();				/// removes all objects in message
        if(!text.empty()) {
          msg_tx.push_object ( car::com::objects::Object (text, car::com::objects::TYPE_TEXT ) );
        }
        msg_tx.send();				        /// sends the message
    }
    if ( msg_rx.receive() ) {			/// check for messages
        static car::com::objects::Object object;
        while ( msg_rx.pop_object ( object ).isValid() ) {
            switch ( object.type ) {
            case car::com::objects::TYPE_SYNC: 	/// case sync object
                car::com::objects::Time::compute_offset ( msg_rx.stamp ); /// set clock
                break;
            default:/// case unkown type
                text.write ( "Unknown type received" );
                continue;
            }
        }
    }

  delay(10);               // wait for a second

}