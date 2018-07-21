// servo_warmup.ino -- Small program to warm up my servo motors.

#include <Servo.h>

Servo rServo, lServo;

const byte rServoPin = 12;
const byte lServoPin = 10;
int rServoDrive = 1400;
int lServoDrive = 1600;
int servoInput;

void setup() {
  rServo.attach(rServoPin);
  lServo.attach(lServoPin);
}

void loop() {

  rServo.writeMicroseconds(rServoDrive);
  lServo.writeMicroseconds(lServoDrive);
}
