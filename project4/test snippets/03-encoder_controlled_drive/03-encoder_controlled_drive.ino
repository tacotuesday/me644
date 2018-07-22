// 03-encoder_controlled_drive.ino - Drive straight forward, returning encoder values,
// and stop based on predefined encoder count

#include <Servo.h>

Servo right_servo, left_servo;

const byte   right_servo_pin = 12;
const byte    left_servo_pin = 10;
const byte right_encoder_pin = 11;
const byte  left_encoder_pin =  9;

int right_center_value = 1498;
int  left_center_value = 1486;
int           left_spd = 50;
int          right_spd = 55;

volatile  int cc_left, cc_right;       // encoder counters
int cc;                           // # times out and back
int distance = 1*(12*64/8);  // = # of 0.125" w fine encoder wheels = 3 foot distance

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);

  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attachInterrupt(  left_encoder_pin,  left_counter, CHANGE);

  attach_servos(1);
  delay(2000);
}

void loop() {
  cc_left = 0; cc_right = 0;
  while (cc_left < distance || cc_right < distance) {
    drive(right_spd, left_spd);
  }
  cc++;
  if (cc > 1) { attach_servos(0); }
}

void drive(int right_speed, int left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void attach_servos(int ats) {
  if ( ats == 0) { right_servo.detach(); left_servo.detach(); }
  else { right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin); }
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }
