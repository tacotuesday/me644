// 04-drive_read_turn90.ino - Drive straight forward, returning encoder values,
// and turn 90 degrees

#include <Servo.h>

// create servo object
Servo right_servo, left_servo;

// pin definition
const byte  left_encoder_pin = 19;
const byte right_encoder_pin = 11;
const byte    left_servo_pin = 10;
const byte   right_servo_pin = 12;

// servo center values & speeds
int right_center_value = 1498;
int  left_center_value = 1486;
int           left_spd = 50;
int          right_spd = 55;

// encoder counter and desired travel distance
volatile int cc_left, cc_right;   // encoder counters
volatile long enc_count;
int cc;                           // # times out and back
int distance = 3*(12*64/8);  // = # of 0.125" w fine encoder wheels = 3 foot distance
int turn_90 = 26;                // # encoder counts to cause it to turn 180 deg. turn

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);

  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attachInterrupt(  left_encoder_pin,  left_counter, CHANGE);

  attach_servos(1);
  delay(2000);                    // time delay to situate robot
}

void loop() {
  cc_left = 0; cc_right = 0;
  while (cc_left < distance || cc_right < distance) {
    drive(right_spd, left_spd);
    enc_count = (cc_left + cc_right)/2;
    drive(left_spd, right_spd);
    Serial.print("cc_left: ");
    Serial.print(cc_left);
    Serial.print("        cc_right: ");
    Serial.println(cc_right);
    Serial.print("        enc_count: ");
    Serial.println(enc_count);
  }

  drive(0, 0);
  cc_left = 0; cc_right = 0;
  while (cc_left < turn_90 || cc_right < turn_90) {
    drive(-right_spd, left_spd);
  }
  drive(0, 0);

  cc++;
  if (cc > 1) { attach_servos(0); }
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

void drive(int right_speed, int left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
  left_servo.writeMicroseconds(left_center_value + left_speed);
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }
