/*
* Getting Started example sketch for nRF24L01+ radios
* Very basic example of how to send data from one node to another
* Updated: Dec 2014 by TMRh20
*/
#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

/****************** User Config ***************************/
/***      Set this radio as radio number 0 or 1         ***/
bool radioNumber = 0;    // equivalent to boolean data type 

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);        // pins that CE & CSN are connected to, respectively.
/**********************************************************/

byte addresses[][6] = {"1Node","2Node"};

// Used to control whether this node is sending or receiving
bool role = 0;

void setup() {
  Serial.begin(115200);
  Serial.println(F("RF24/examples/GettingStarted"));
  Serial.println(F("*** PRESS 'T' to begin transmitting to the other node"));
  
  radio.begin();

  // Set the PA Level low to prevent power supply related issues since this is a
  // getting_started sketch, and the likelihood of close proximity of the devices.
  //  RF24_PA_MAX is default.
  radio.setPALevel(RF24_PA_MAX);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){
    radio.openWritingPipe(addresses[1]);
    radio.openReadingPipe(1,addresses[0]);
  }
  else{
    radio.openWritingPipe(addresses[0]);
    radio.openReadingPipe(1,addresses[1]);
  }
  
  // Start the radio listening for data
  radio.startListening();
}

void loop() {
  /****************** Ping Out Role ***************************/  
  if (role == 1)  {
    radio.stopListening();                // First, stop listening so we can talk.
    
    Serial.println(F("Now sending"));

    unsigned long start_time = micros();  // Take time & send. Will block until complete.
    if (!radio.write( &start_time, sizeof(unsigned long) )){
      Serial.println(F("failed"));
    }
    radio.startListening();               // Now, continue listening
    
    unsigned long started_waiting_at = micros();  // timeout period, get current {microsec)
    boolean timeout = false;                      // variable to indicate if response received or not
    
    while ( ! radio.available() ){                    // While nothing is received
      if (micros() - started_waiting_at > 200000 ){   // If waited > 200ms, indicate timeout & exit while loop
          timeout = true;
          break;
      }      
    }
        
    if ( timeout ){                                         // Describe the results
        Serial.println(F("Failed, response timed out."));
    }
    else
    {
      unsigned long got_time;                         // Grab response, compare & send to debugging spew
      radio.read( &got_time, sizeof(unsigned long) );
      unsigned long end_time = micros();
      
      // Spew it
      Serial.print(F("Sent "));
      Serial.print(start_time);
      Serial.print(F(", Got response "));
      Serial.print(got_time);
      Serial.print(F(", Round-trip delay "));
      Serial.print(end_time-start_time);
      Serial.println(F(" microseconds"));
    }

    // Try again 1s later
    delay(1000);
  }

  /****************** Pong Back Role ***************************/
  if ( role == 0 )
  {
    unsigned long got_time;
    
    if( radio.available()){
                                                                    // Variable for the received timestamp
      while (radio.available()) {                                   // While there is data ready
        radio.read( &got_time, sizeof(unsigned long) );             // Get the payload
      }
     
      radio.stopListening();                                        // First, stop listening so we can talk   
      radio.write( &got_time, sizeof(unsigned long) );              // Send the final one back.      
      radio.startListening();                                       // Resume listening to catch next packets.     
      Serial.print(F("Sent response "));
      Serial.println(got_time);  
   }
 }

  /****************** Change Roles via Serial Commands ***************************/
  if ( Serial.available() )
  {
    char c = toupper(Serial.read());
    if ( c == 'T' && role == 0 ){      
      Serial.println(F("*** CHANGING TO TRANSMIT ROLE -- PRESS 'R' TO SWITCH BACK"));
      role = 1;                  // Become the primary transmitter (ping out)
    }
    else if ( c == 'R' && role == 1 ){
      Serial.println(F("*** CHANGING TO RECEIVE ROLE -- PRESS 'T' TO SWITCH BACK"));      
       role = 0;                // Become the primary receiver (pong back)
       radio.startListening();
    }
  }
} // end of loop
