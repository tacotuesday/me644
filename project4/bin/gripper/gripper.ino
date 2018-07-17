// gripper.ino -- code to control Arduino seeker robot
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

void setup() {
  // Counter variables
  byte ats;
  int cc1;
  volatile int cc_left, cc_right;

  // Servo housekeeping declarations
  Servo right_servo, left_servo;
  const byte right_servo_pin = ;
  const byte right_encoder_pin = ;
  const int right_center_value = ;
  const byte right_spd = ;
  const byte left_servo_pin = ;
  const byte left_encoder_pin = ;
  const int left_center_value = ;
  const byte left_speed = ;
  const byte grip_servo_pin = ;
  const int grip_center_value = ;

  const byte turn_180 = 51;    // change based on results from turn_around_micro.ino calibration?

  // These variables store RF-recieved values from the Seeker Robot.
  byte distance_to_flag_in;
  int r_enc_flag_orient_count_out;   // Orients Gripper to match Seeker's angle of travel
  int l_enc_flag_orient_count_out;
  int r_enc_flag_orient_count_in;    // TODO: Eliminate redundant variables--these should be
  int l_enc_flag_orient_count_in;    // equal and opposite variables above. Write function?

  orient_encoders();
// TODO: move forward to spot that seeker robot initially occupied
// drive() needs to be in a FOR loop?
  drive(,);
// TODO: recieve encoder counts from seeker robot
// RX->store in variables
// TODO: orient encoders based on rotation variables from seeker robot
// TODO: recieve distance from seeker robot
// RX millisecondsInCentimeters()->input distance into calculation
// TODO: travel to flag
// FOR i = (distance in centimeters, i++): drive(). Halt after distance.
// TODO: grab flag
// TODO: adapt code to turn 180 degrees from turn_around_micro.ino
// TODO: travel to beginning
// FOR i = (distance in centimeters, i++): drive(). Halt after distance.
// TODO: travel to finish point-- will need to figure out how this is laid out
// based on prompt. To find the gates-- have robot travel back to start position,
// then send the orientation encoder counts to the opposite servo motors.
// ASSUMING START COAXIAL WITH BRIDGE!
}

void loop() {

}

// TODO: Write PING))) search algorithm.
// TODO: To find the flag--sweep robot. Flag found if distance_current < distance_last minus some
// experimentally-determined offset to correct for "finding" the wall
// TODO: To find the gates-- have robot travel back to start position, then send the orientation
// encoder counts to the opposite servo motors. ASSUMING START COAXIAL WITH BRIDGE!

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

void attach_servos(byte ats) {
  if ( ats == 0) {
    right_servo.detach(); left_servo.detach();
  }
  else {
    right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin);
  }
}

void drive(byte right_speed, byte left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void  left_counter() { cc_left++; }
void  right_counter() { cc_right++; }

// Return the distance to flag in encoder window counts
void distance_to_flag_enc(int distance_to_flag_in) {
  return distance_to_flag_in*(12*64/8);
}
