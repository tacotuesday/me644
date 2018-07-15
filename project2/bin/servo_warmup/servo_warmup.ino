// servo_warmup.ino -- Small program to warm up my servo motors.

#include <Servo.h>

Servo rServo, lServo;

const byte rServoPin = 4;
const byte lServoPin = 12;
int rServoDrive = 1300;
int lServoDrive = 1700;
int servoInput;

void setup() {
  rServo.attach(rServoPin);
  lServo.attach(lServoPin);
}

void loop() {

  rServo.writeMicroseconds(rServoDrive);
  lServo.writeMicroseconds(lServoDrive);
}
