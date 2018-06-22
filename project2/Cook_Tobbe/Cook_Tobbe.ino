/* Cook_Tobbe.ino -- production code for Grafton Cook and Joe
Tobbe's Project 1. Based on turn_around_micro.ino, servo_motor.ino by Chris
Richards (UofL)*/

#include <Servo.h>

// TODO: Define servo center values and speeds (see center_check.ino)
// Create servo objects
Servo right_servo, left_servo;

// Define pinout
const byte  left_encoder_pin =  9;
const byte right_encoder_pin = 10;
const byte    left_servo_pin = 11;
const byte   right_servo_pin = 12;

// TODO: Determine center values and left/right speeds for my servos
const int  left_center_value  = 1500;
const int  right_center_value = 1500;
const byte left_spd           = 50;
const byte right_spd          = 50;

// encoder counter and desired travel distance
volatile int cc_left, cc_right;     // encoder counters
int cc;                             // # times out and back
const int distance = 10*(12*64/8);  // = # of 0.125" w fine encoder wheels = 3 foot distance
// const int turn_180 = 51;         // # Encoder counts to cause it to turn 180 deg. Review prompt before deleting.

void setup() {
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);

  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attachInterrupt(  left_encoder_pin,  left_counter, CHANGE);

  attach_servos(1);
  orient_encoders();
  delay(5000);                    // time delay to situate robot
}

void loop() {
  cc_left = 0; cc_right = 0;        // Initialize counter variables
  while (cc_left < distance || cc_right < distance) {
    drive(right_spd, left_spd);    // Drive the robot forward
  }
  drive(0, 0);                     // Detaches servo motors

/* Following block controls 180-degree turn. Review prompt before deleting.
  cc_left = 0; cc_right = 0;
  while (cc_left < turn_180 || cc_right < turn_180) {
    drive(right_spd, -left_spd);
  }
  drive(0, 0);
*/
  cc++;
  if (cc > 1) { attach_servos(0); }
}

// Function to read encoder location and adjust for accurate counting
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

// Function to attach/detach servos
void attach_servos(int ats) {
  if ( ats == 0) {
    right_servo.detach();
     left_servo.detach();
  }
  else {
    right_servo.attach(right_servo_pin);
     left_servo.attach( left_servo_pin);
  }
}

// Function to drive robot forward via servos.
// TODO: Look into remapping servos to drive them with one input
void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }
