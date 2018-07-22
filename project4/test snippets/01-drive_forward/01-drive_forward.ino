// 01-drive_forward.ino - Drive straight forward

#include <Servo.h>

// create servo object
Servo right_servo, left_servo;

// pin definition
const byte   right_servo_pin = 12;
const byte    left_servo_pin = 10;

// servo center values & speeds
int right_center_value = 1498;
int  left_center_value = 1486;
int           left_spd = 50;
int          right_spd = 55;

void setup() {
  attach_servos(1);
}

void loop() {
  drive(right_spd,left_spd);
}

void attach_servos(int ats) {
  if ( ats == 0) {
    right_servo.detach();
     left_servo.detach();
  }
  else {
    left_servo.attach( left_servo_pin);
    right_servo.attach(right_servo_pin);
  }
}

void drive(int right_speed, int left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
  left_servo.writeMicroseconds(left_center_value + left_speed);
}
