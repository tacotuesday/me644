#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

/* Set this radio as radio number 0 or 1 */
bool radioNumber = 0;    // equivalent to boolean data type

/* Hardware configuration: Set up nRF24L01 radio on SPI bus plus pins 7 & 8 */
RF24 radio(7,8);        // pins that CE & CSN are connected to, respectively.

byte          addresses[][6] = {"1Node","2Node"};
unsigned long cmd  = 0;  // variable to store keyboard command

void setup() {
  Serial.begin(115200);
  Serial.println("commands:");
  Serial.println("0 = brake");
  Serial.println("1 = forward");
  Serial.println("2 = backward");
  Serial.println("3 = right");
  Serial.println("4 = left");
  Serial.println("5 = open   gripper");
  Serial.println("6 = close  gripper");
  Serial.println("7 = detach gripper");
   radio.begin();
   radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){ radio.openWritingPipe(addresses[1]); radio.openReadingPipe(1,addresses[0]); }
  else           { radio.openWritingPipe(addresses[0]); radio.openReadingPipe(1,addresses[1]); }
  radio.stopListening();                            // stop listening to enable sending.
}

void loop() {
  /* read number from terminal screen */
  if (Serial.available()) { cmd = Serial.read() - 48; }
  if (!radio.write( &cmd, sizeof(unsigned long) )){ Serial.println(F("failed")); }
//  delay(1000);    // Try again 1 second later
}
