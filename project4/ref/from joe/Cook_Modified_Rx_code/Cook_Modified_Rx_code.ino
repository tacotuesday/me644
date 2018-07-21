#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

bool radioNumber = 1;   // Set this radio as radio number 0 or 1
RF24 radio(6,7);        // Set up nRF24L01 radio on SPI bus plus CE & CSN pins

byte          addresses[][6] = {"1Node","2Node"};

unsigned long cmd[3];
int cmd1=0;
int cmd2=0; 
int cmd3=0;
int cmd4=0; 

int  runonce = 1;                 // flag to run loop 

void setup() {
  Serial.begin(115200);
   radio.begin();
   radio.setPALevel(RF24_PA_LOW);
  
  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){ radio.openWritingPipe(addresses[1]); radio.openReadingPipe(1,addresses[0]); }
  else           { radio.openWritingPipe(addresses[0]); radio.openReadingPipe(1,addresses[1]); }
  radio.startListening();                            // enable listening.
  

}

void loop() {

 Serial.println(cmd);
 
 if (runonce == 1 && radio.available()) {
    while (radio.available()) {                   // While there is data ready
      radio.read( &cmd, sizeof(unsigned long) );  // Get the payload
    }

cmd1=cmd[0];
cmd2=cmd[1];
cmd3=cmd[2];
cmd4=cmd[3];
 
 }

 runonce=0;
}

