#include <Servo.h>
#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

bool radioNumber = 1;   // Set this radio as radio number 0 or 1
RF24 radio(6,7);        // Set up nRF24L01 radio on SPI bus plus CE & CSN pins

byte          addresses[][6] = {"1Node","2Node"};
unsigned long cmd  = 0;  // variable to store keyboard command

// create servo objects
Servo right_servo, left_servo, gripper_servo;

// pin definition
int right_servo_pin = 12;
int  left_servo_pin = 11;
int     gripper_pin = 8;

// center values
int right_center_value = 1517;
int  left_center_value = 1504;

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
  if( radio.available()){
    while (radio.available()) {                   // While there is data ready
      radio.read( &cmd, sizeof(unsigned long) );  // Get the payload
    }
    switch (cmd) {
      case 0: drive(  0,     0); Serial.println("brake");          break;
      case 1: drive(-100,  100); Serial.println("forward");        break;
      case 2: drive( 100, -100); Serial.println("backward");       break;
      case 3: drive( 30,    30); Serial.println("right");          break;
      case 4: drive(-30,   -30); Serial.println("left");           break;
      case 5: gripper(0);        Serial.println("open   gripper"); break;
      case 6: gripper(150);      Serial.println("close  gripper"); break;
      case 7: gripper(500);      Serial.println("detach gripper"); break;
    }
  }
}

void drive(int right_speed, int left_speed) {
  if ( right_speed != 0 && left_speed != 0) {
    right_servo.attach(right_servo_pin);  
     left_servo.attach( left_servo_pin);
    right_servo.writeMicroseconds(right_center_value + right_speed);
     left_servo.writeMicroseconds( left_center_value +  left_speed);
  }
  else {
    right_servo.writeMicroseconds(right_center_value);
     left_servo.writeMicroseconds( left_center_value);
    right_servo.detach();
     left_servo.detach();
  }
}

void gripper(int gripper_servo_position) {
  if ( gripper_servo_position == 500 ) {
    gripper_servo.detach();
  }
  else {
    gripper_servo.attach(gripper_pin);
    gripper_servo.write(gripper_servo_position);
  }
}
