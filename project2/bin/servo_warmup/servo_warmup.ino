// servo_warmup.ino -- Small program to warm up my servo motors.

#include <Servo.h>

Servo rServo, lServo;

const byte rServoPin = 2;
const byte lServoPin = 3;
int rServoDrive = 1300;
int lServoDrive = 1700;
int servoInput;

void setup() {
  rServo.attach(rServoPin);
  rServo.writeMicroseconds(rServoDrive);

}

void loop() {
  lServo.attach(lServoPin);
  lServo.writeMicroseconds(lServoDrive);
}
