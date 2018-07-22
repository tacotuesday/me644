// 02-drive_forward_count.ino - Drive straight forward, returning encoder values
// Check PID example code to see if returning cc_left and cc_right is more accurate
// than returning the variable. Also, look into reading encoder counts via pointer.

#include <Servo.h>
#include <PID_v1.h>

// create servo object
Servo right_servo, left_servo;

// pin definition
const byte  left_encoder_pin =  9;
const byte right_encoder_pin = 11;
const byte    left_servo_pin = 10;
const byte   right_servo_pin = 12;

// servo center values & speeds
int right_center_value = 1498;
int  left_center_value = 1486;

// encoder counter and desired travel distance
volatile int cc_left = 0, cc_right = 0;   // encoder counters

// PID variables & initialization
int dt;              // time difference between encoders
double desired_dt = 0;  // desired time difference between encoders
double  left_spd  = 50;
double right_spd  = 54;
// PID myPID(&dt, &right_spd, &desired_dt, 0.084457214, 7.712170356, 0.014842, DIRECT);

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);

  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attachInterrupt(  left_encoder_pin,  left_counter, CHANGE);

  attach_servos(1);
  // delay(1000);                    // time delay to situate robot
}

void loop() {
  drive(left_spd, right_spd);
  // dt = read_encoders();
  // myPID.Compute();
  int a = left_total();
  Serial.println(a);
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
  left_servo.writeMicroseconds(left_center_value + left_speed);
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }
int left_total() {cc_left++; int enc_dist = cc_left * (64/8); return enc_dist;}

long read_encoders() {
  unsigned long t_right_pass, t_left_pass;
  long delta_t_pass;

  int right_value = digitalRead(right_encoder_pin);
  while (digitalRead(right_encoder_pin) == right_value) { }
  t_right_pass = millis();
  while (digitalRead(right_encoder_pin) != right_value) { }
  t_right_pass = millis() - t_right_pass;

  int left_value = digitalRead(left_encoder_pin);
  while (digitalRead(left_encoder_pin)  == left_value) { }
  t_left_pass  = millis();
  while (digitalRead(left_encoder_pin)  != left_value) { }
  t_left_pass  = millis() - t_left_pass;

  delta_t_pass = t_left_pass - t_right_pass;
  return delta_t_pass;
}
