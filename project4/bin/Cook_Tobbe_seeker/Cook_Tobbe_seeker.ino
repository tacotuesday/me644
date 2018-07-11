// Cook_Tobbe_Seeker.ino -- code to control Arduino seeker robot
#include <Servo.h>
#include <SPI.h>
#include "RF24.h"

void setup() {

}

void loop() {

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
  if ( ats == 0) {
    right_servo.detach(); left_servo.detach();
  }
  else {
    right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin);
  }
}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void  left_counter() { cc_left++; }
void  right_counter() { cc_right++; }
