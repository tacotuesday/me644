// Cook_Tobbe.ino -- Project 2 production code by Grafton Cook and Joe Cook_Tobbe

#include <Servo.h>
#include <PID_v1.h>

// create servo object to control a servo
Servo right_servo;
Servo  left_servo;

// pin definition
int right_encoder_pin = 10;
int  left_encoder_pin =  9;
int   right_servo_pin = 11;
int    left_servo_pin = 12;

// servo center values & speeds
int right_center_value = 1469;
int  left_center_value = 1491;

// encoder counter and desired travel distance
volatile int cc_left;
// = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
int distance = 10*(12*64/8);  // = # of 0.125" w fine encoder wheels in 10 feet

// PID variables & initialization
double dt;                            // time difference between encoders
double    left_spd = 150;             // speeds determined by turn_around_micro.ino
double   right_spd = 132;
double desired_dt  = 0;               // desired time difference between encoders
PID myPID(&dt, &right_spd, &desired_dt, 0.084457214, 7.712170356, 0.014842, DIRECT);

void setup() {
  Serial.begin(9600);         // initialize USB communication
  myPID.SetMode(AUTOMATIC);   //turn on PID
  myPID.SetOutputLimits( right_spd - 30, right_spd + 30 );

  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);
  attach_servos(1);
  orient_encoders();
  delay(1000);
}

void loop() {
  cc_left = 0;
  while (cc_left < distance) {
    drive(right_spd, left_spd);
    dt = read_encoders();
    Serial.print(cc_left);    Serial.print(" ");
    Serial.print(dt);         Serial.print(" ");
    Serial.print(right_spd);  Serial.println(";");
    myPID.Compute();
  }
  drive(0, 0);
  delay(1000);                // orient encoders, then delay one second
  attach_servos(0);
}

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

  return delta_t_pass = t_left_pass - t_right_pass;
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
    right_servo.detach(); left_servo.detach();
  }
  else {
    right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin);
  }
}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void  left_counter() { cc_left++; }
