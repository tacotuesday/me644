#include <Servo.h>
#include <PID_v1.h>
#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

bool radioNumber = 0;   // Set this radio as radio number 0 or 1
RF24 radio(6,7);        // Set up nRF24L01 radio on SPI bus plus CE & CSN pins
byte addresses[][6] = {"1Node","2Node"};
int cmd[4] = {0};

// create servo object to control a servo
Servo right_servo, left_servo, Ping_servo;

// Pin Definition
const byte   right_servo_pin = 12;
const byte right_encoder_pin = 11;
const byte    left_servo_pin = 10;
const byte  left_encoder_pin =  9;
const byte    Ping_servo_pin =  8;
const byte          Ping_pin =  7;

// servo center values
int right_center_value = 1499;
int  left_center_value = 1486;
int Ping_right_value = 6;
int Ping_center_value = 96;
int Ping_left_value = 180;

// initialize variables, counters, and desired travel distance
// feet traveled = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
byte runonce = 1;               // flag to run loop once
int inches;                     // distance to wall
long duration;
volatile int cc_left, cc_right;

// PID variables & initialization
double dt;                      // time difference between encoders
double desired_dt  = 0;         // desired time difference between encoders
double    left_spd = 50;
double   right_spd = 56;
// PID myPID(&dt, &right_spd, &desired_dt, 0.084457214, 7.712170356, 0.014842, DIRECT);

void setup() {
  Serial.begin(115200);         // Initialize serial communication
   radio.begin();
   radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){ radio.openWritingPipe(addresses[1]); radio.openReadingPipe(1,addresses[0]); }
  else           { radio.openWritingPipe(addresses[0]); radio.openReadingPipe(1,addresses[1]); }
  radio.stopListening();                            // stop listening to enable sending.

  myPID.SetMode(AUTOMATIC);   //turn on PID
  myPID.SetOutputLimits( right_spd - 30, right_spd + 30 );

  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);
  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
}

void loop() {
  if (runonce == 1) {
    look(Ping_center_value);            // Orient the PING))) straight ahead.
    attach_servos(1);
    delay(100);
    orient_encoders();
    delay(100);
    cc_left = 0;
    look(Ping_right_value);    // Orient the PING))) 90 degrees to the right.

    cc_left = 0;
    int Ping_measure = 150; // Initialize a measured range to enter the while loop
    int max_range = 84;     // Max scan range ~= 7 ft

    // Search for first wall surrounding bridge
    while (Ping_measure > max_range) { Ping_measure = distance_measure(); drive(left_spd, right_spd); }
    delay(100); drive(0, 0); delay(100); orient_encoders(); delay(100);
    n1 = cc_left / 8;           // n1 = y-coordinate distance to wall start -> cmd[0]
    e1 = distance_measure();    // e1 = x-coordinate distance to wall -> cmd[1]
    delay(100);
    cc_left = 0; cc_right = 0;  // reset encoder counts

    // Move forward 18 inches to align with bridge
    while (cc_left < 144) { drive(left_spd, right_spd); }
    delay(100); orient_encoders(); delay(100); cc_left = 0;
    look(Ping_left_value); delay(100);    // orient the Ping toward expected treasure direction

    // Turn right to cross the bridge
    while (cc_left < 19 || cc_right < 19) { drive(left_spd - 30, right_spd + 30); }
    delay(100); drive(0, 0); delay(100); orient_encoders(); delay(100);
    cc_left = 0; cc_right = 0;

    // Cross the bridge to grabber bot's y-coordinate: e1 + 2'
    while (cc_left < (e1 + 24) * (64 / 8)) { drive(left_spd, right_spd); }
    delay(100); drive(0, 0); delay(100); orient_encoders(); delay(100);

    // Move forward to find the treasure
    while (Ping_measure > max_range) { Ping_measure = distance_measure(); drive(left_spd, right_spd); }
    delay(100); drive(0, 0); delay(100); orient_encoders(); delay(100);
    e2 = cc_left / 8;           // e2 = y-coordinate distance to wall start -> cmd[2]
    n2 = distance_measure();    // n2 = x-coordinate distance to wall -> cmd[3]
    delay(100);
    cc_left = 0; cc_right = 0;

    // Move out of grabber bot's path
    while (cc_left < 24 * (64 / 8)) { drive(left_spd, right_spd); }
    delay(100); drive(0, 0);

    cmd[] ={n1, e1, e2, n2};

    if (!radio.write( &cmd, sizeof(int) )) { Serial.println(F("failed")); }
    attach_servos(0); runonce = 0;
  }
}

int distance_measure() {
  pinMode(Ping_pin, OUTPUT);
  digitalWrite(Ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(Ping_pin, LOW);
  pinMode(Ping_pin, INPUT);
  duration = pulseIn(Ping_pin, HIGH);
  inches = duration / 67UL / 2UL;        // Expanded to remove microsecondsToInches()

  Serial.print(inches);
  Serial.print(" in,    ");
  if (inches > 1) {return inches;}
}

void look(int Ping_servo_position) {
  Ping_servo.attach(Ping_servo_pin);
  Ping_servo.write( Ping_servo_position);
  delay(2000);
  Ping_servo.detach();
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
  if ( ats == 0) { right_servo.detach(); left_servo.detach(); }
  else { right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin); }
}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void  left_counter() { cc_left++; }
void  left_counter() { cc_left++; }
