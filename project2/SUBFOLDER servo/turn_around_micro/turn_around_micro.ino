#include <Servo.h>

// create servo object
Servo right_servo, left_servo;

// pin definition
int  left_encoder_pin =  9;
int right_encoder_pin = 10;
int    left_servo_pin = 11;
int   right_servo_pin = 12;

// servo center values & speeds
int  left_center_value = 1499;
int right_center_value = 1512;
double  left_spd       = 50;
double right_spd       = 57;

// encoder counter and desired travel distance
volatile int cc_left, cc_right;   // encoder counters
int cc;                           // # times out and back
int distance = 3*(12*64/8);  // = # of 0.125" w fine encoder wheels = 3 foot distance
int turn_180 = 51;                // # encoder counts to cause it to turn 180 deg. turn

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
  cc_left = 0; cc_right = 0;
  while (cc_left < distance || cc_right < distance) {
    drive(right_spd, left_spd);
  }
  drive(0, 0);
  cc_left = 0; cc_right = 0;
  while (cc_left < turn_180 || cc_right < turn_180) {
    drive(right_spd, -left_spd);
  }
  drive(0, 0);
  cc++;
  if (cc > 1) { attach_servos(0); }
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
    right_servo.detach();
     left_servo.detach();
  }
  else {
    right_servo.attach(right_servo_pin);
     left_servo.attach( left_servo_pin);
  }
}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }
