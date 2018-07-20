#include <Servo.h>
#include <PID_v1.h>
#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

bool radioNumber = 1;   // Set this radio as radio number 0 or 1
RF24 radio(6,7);        // Set up nRF24L01 radio on SPI bus plus CE & CSN pins
byte          addresses[][6] = {"1Node","2Node"};

// create servo object to control a servo
Servo right_servo, left_servo, gripper_servo;

// pin definition
int   right_servo_pin = 12;
int    left_servo_pin = 11;
int right_encoder_pin = 10;
int  left_encoder_pin =  9;
int gripper_servo_pin =  8;

// servo center values
int right_center_value = 1515;
int  left_center_value = 1501;

// initialize variables, counters, and desired travel distance
// = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
//int distance = 4*(12*64/8);       // = # of 0.125" w fine encoder wheels
int distance=0;
int turn_180 = 52;                // # encoder pulses for 180 degree turn
int turn_90  = 26;                // # encoder pulses for  90 degree turn
int  runonce = 1;                 // flag to run loop once
volatile int  cc_left, cc_right;  // encoder counters

// PID variables & initialization
double dt;              // time difference between encoders
double desired_dt = 0;  // desired time difference between encoders
double  left_spd  = 50;
double right_spd  = 54;
PID myPID(&dt, &right_spd, &desired_dt, 0.01, 1.0, 0, DIRECT);

// Array creation to receive data from Bridge Scout
int data[4] ;
int N1=0; int N2=0; int E1=0; int E2=0;

void setup() {
  Serial.begin(115200);
   radio.begin();
   radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){ radio.openWritingPipe(addresses[1]); radio.openReadingPipe(1,addresses[0]); }
  else           { radio.openWritingPipe(addresses[0]); radio.openReadingPipe(1,addresses[1]); }
  radio.startListening();                            // enable listening.

  myPID.SetMode(AUTOMATIC);   //turn on PID
  myPID.SetOutputLimits( right_spd - 30, right_spd + 30 );

  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);
  attachInterrupt(right_encoder_pin, right_counter, CHANGE);
  attach_servos(1);
  orient_encoders();

}

void loop()
{

  if (runonce == 1 && radio.available()) {
    while (radio.available()) {                   // While there is data ready
      radio.read( &data, sizeof(unsigned long) );  // Get the payload
    }

  N1=data[0];
  N2=data[1];
  E1=data[2];
  E2=data[3];

  distance=E2*64/8;

 // open gripper
    gripper(0); delay(3000);

    // forward toward treasure
    while (cc_left < distance) {
       drive(right_spd, left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

    // 90 deg. left turn
    while (cc_left < turn_90-1 || cc_right < turn_90-1) {
      drive(right_spd, -left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

    // forward toward flag
    while(cc_left < ((N1+N2-6.0)*(64/8)) {
       drive( right_spd,  left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

    // pick up flag
    gripper(150); delay(3000);

    // 180 deg. right turn
    while (cc_left < turn_180 || cc_right < turn_180) {
      drive(right_spd, -left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

    // forward away from flag
    while(cc_left < (N2-6.0)*(64/8)) {
       drive( right_spd,  left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

    // 90 deg. right turn
    while (cc_left < turn_90 || cc_right < turn_90) {
      drive(-right_spd, left_spd);
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

distance=(E2+24+E1)*(64/8);

    // forward back to start
    while (cc_left < distance) {
       drive( right_spd,  left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

        // 90 deg. left turn
    while (cc_left < turn_90-1 || cc_right < turn_90-1) {
      drive(right_spd, -left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);

     // South towards finish
    while(cc_left < (N1)*(64/8)) {
       drive( right_spd,  left_spd); dt = read_encoders(); myPID.Compute();
    }; drive(0, 0); orient_encoders(); cc_left = 0; cc_right = 0; delay(1000);


    // release flag
    gripper(0); attach_servos(0); runonce = 0;
  }
}

void gripper(int gripper_servo_position) {
  gripper_servo.attach(gripper_servo_pin);
  gripper_servo.write(gripper_servo_position);
  delay(2000);
  gripper_servo.detach();
}

long read_encoders() {
  unsigned long t_right_pass, t_left_pass;
  long delta_t_pass;

  int right_value = digitalRead(right_encoder_pin);
  while (digitalRead(right_encoder_pin) == right_value) { }
  t_right_pass = millis();
  while (digitalRead(right_encoder_pin) != right_value) { }
  t_right_pass = millis() - t_right_pass;

  int left_value = digitalRead(left_encoder_pin);
  while (digitalRead(left_encoder_pin)  == left_value) { }
  t_left_pass  = millis();
  while (digitalRead(left_encoder_pin)  != left_value) { }
  t_left_pass  = millis() - t_left_pass;

  delta_t_pass = t_left_pass - t_right_pass;
  return delta_t_pass;
}

void orient_encoders() {
  int right_value = digitalRead(right_encoder_pin);
  drive(10 - 20*right_value, 0);
  while (digitalRead(right_encoder_pin) == right_value) { }
  drive(0, 0);
  int left_value = digitalRead(left_encoder_pin);
  drive(0, 20*left_value - 10);
  while (digitalRead(left_encoder_pin) == left_value) { }
  drive(0, 0);
}

void attach_servos(int ats) {
  if ( ats == 0) { right_servo.detach(); left_servo.detach(); }
  else { right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin); }
}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void  left_counter() { cc_left++; }
void right_counter() { cc_right++; }
