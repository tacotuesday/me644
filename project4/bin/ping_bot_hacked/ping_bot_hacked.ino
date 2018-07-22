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
const byte    left_servo_pin = 10;
const byte right_encoder_pin = 11;
const byte  left_encoder_pin =  9;
const byte    Ping_servo_pin =  8;
const byte          Ping_pin =  7;

// servo center values
int right_center_value = 1499;
int  left_center_value = 1486;
int Ping_center_value = 96;
int Ping_left_value = 180;
int Ping_right_value = 6;


// initialize variables, counters, and desired travel distance
// = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
byte runonce = 1;                // flag to run loop once
int distance = 1*(12*64/8);      // = # of 0.125" w fine encoder wheels
int half_dist = (1*(12*64/8))/2; // used to go forward six inches
long duration;
unsigned long inches;            // distance to wall
volatile int cc_left, cc_right;           // left encoder counter
volatile int feet_traveled;
volatile int current_dist;

// PID variables & initialization
double dt;                // time difference between encoders
double desired_dt  = 0;   // desired time difference between encoders
double    left_spd = 50;
double   right_spd = 56;
// PID myPID(&dt, &right_spd, &desired_dt, 0.084457214, 7.712170356, 0.014842, DIRECT);

void setup() {
  Serial.begin(115200);         // initialize USB communication
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
    delay(50);
    orient_encoders();
    delay(50);
    cc_left = 0; current_dist = 0;
    look(Ping_left_value);    // Orient the PING))) 90 degrees to the left.

    cc_left = 0;
    int Ping_measure = 150; // Initialize a measured range to enter the while loop
    int max_range = 84;     // Max scan range ~= 7 ft

    // Search for first wall
    while (Ping_measure > max_range) {Ping_measure = distance_measure(); drive(left_spd, right_spd); }


    if (!radio.write( &inches, sizeof(unsigned long) )) { Serial.println(F("failed")); }
    // cc_left = 0;
    // while (cc_left < 185) { drive(-65, -50); }
    // drive(0, 0); delay(1000);
    attach_servos(0); runonce = 0;
  }
}

void one_foot_forward() {
  int distance = 1*(12*64/8);
  while (cc_left < distance) {
    drive(right_spd, left_spd);
  }
  drive(0, 0); delay(1000);
}

int distance_measure() {
  pinMode(Ping_pin, OUTPUT);
  digitalWrite(Ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(Ping_pin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(Ping_pin, INPUT);
  duration = pulseIn(Ping_pin, HIGH);

  // convert the time into a distance
  inches = duration / 67UL / 2UL;

  Serial.print(inches);
  Serial.print(" in,    ");
if (inches > 1) {return inches;}
}

// unsigned long microsecondsToInches(unsigned long microseconds) {
//   // According to Parallax's datasheet for the PING))), there are
//   // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
//   // second).  This gives the distance travelled by the ping, outbound
//   // and return, so we divide by 2 to get the distance of the obstacle.
//   // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
//   return microseconds / 67UL / 2UL;
//   // 1 / ( 67 us/in * 1 s/1e6 us * 12 in / ft ) = 1243.8 ft/s = 379.1 m/s!
// }

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
