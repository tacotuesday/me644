//02-drive_forward_count.ino - Drive straight forward, returning encoder values
// Check PID example code to see if returning cc_left and cc_right is more accurate
// than returning the variable. Also, look into reading encoder counts via pointer.

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
int left_spd = 50;
int right_spd = 55;

// encoder counter and desired travel distance
volatile int cc_left, cc_right;   // encoder counters
volatile long enc_count;

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);

  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attachInterrupt(  left_encoder_pin,  left_counter, CHANGE);

  attach_servos(1);
  delay(1000);                    // time delay to situate robot
}

void loop() {
  enc_count = (cc_left + cc_right)/2;
  drive(left_spd, right_spd);
  Serial.print("cc_left: ");
  Serial.print(cc_left);
  Serial.print("        cc_right: ");
  Serial.println(cc_right);
  Serial.print("        enc_count: ");
  Serial.println(enc_count);
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
