#include <Servo.h>

Servo Ping_servo;

const byte Ping_servo_pin = 9;
const int Ping_left = 0;
const int Ping_center = 90;
const int Ping_right = 180;

void setup() {
  Ping_servo.attach();
  Ping_servo.write(Ping_left);
  delay(3000);
  Ping_servo.write(Ping_center);
  delay(3000);
  Ping_servo.write(Ping_right);
  delay(3000);
}

void loop() {

}
