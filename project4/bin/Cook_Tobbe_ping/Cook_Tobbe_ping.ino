#include <Servo.h>
#include <PID_v1.h>
#include <SPI.h>
#include "RF24.h"   // RX/TX library

bool radioNumber = 0;   // Set this radio as radio number 0 or 1
RF24 radio(6,7);        // nRF24L01 radio on SPI bus plus CE & CSN pins
byte addresses[][6] = {"1Node","2Node"};

// create servo object to control a servo
Servo right_servo, left_servo, Ping_servo;

// pin definition
const byte   right_servo_pin = 12;
const byte    left_servo_pin = 13;
const byte right_encoder_pin = 10;
const byte  left_encoder_pin = 11;
const byte          Ping_pin =  8;
const byte    Ping_servo_pin =  9;

// servo center values
int right_center_value = 1484;    // CHANGE THESE VALUES AFTER TESTING
int  left_center_value = 1504;    // NEW SERVOS

// initialize variables, counters, and desired travel distance
// = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
// int  runonce = 1;          // Obsolete: moving main code from loop() to setup()
volatile  int cc_left;        // left encoder counter
unsigned long inches;         // distance to wall
float cmd[];                  // May need to change to int if Joe can't handle a float

// PID variables & initialization. Test for feasibility, but may not be needed.
double dt;                // time difference between encoders
double desired_dt  = 0;   // desired time difference between encoders
double    left_spd = 50;
double   right_spd = 50;
PID myPID(&dt, &right_spd, &desired_dt, .01, 1, 0, DIRECT);

void setup() {
  Serial.begin(115200);         // initialize USB communication
   radio.begin();
   radio.setPALevel(RF24_PA_LOW);

  // Open a writing and reading pipe on each radio, with opposite addresses
  if(radioNumber){ radio.openWritingPipe(addresses[1]); radio.openReadingPipe(1,addresses[0]); }
  else           { radio.openWritingPipe(addresses[0]); radio.openReadingPipe(1,addresses[1]); }
  radio.stopListening();                            // stop listening to enable sending.

  // myPID.SetMode(AUTOMATIC);   //turn on PID                 // Commented out to remove complexity
  // myPID.SetOutputLimits( right_spd - 30, right_spd + 30 );  // during testing. May be reintegrated.

  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);

  // TODO: Code block to complete Step 1: Drive to bridge y-coordinate
  // Orient PING to the right. Detect distance in inches. If less than 72 in, record distance as cmd[1] to conform with pathing document.
  // Drive forward: Count encoder passes, monitor PING for change in distances.
  // PING subroutine: If distance change greater than error threshold, save encoder counts.
  // Convert encoder count to inches and add six to find center of bridge. Save as cmd[0].
  // Then drive six inches forward.
  find_wall();
  // TODO: Code block to complete Step 2: Drive to bridge x-coordinate.
  // Reorient PING to left to reduce error from rear wall!
  // Reuse code from step one to count encoders to edge of the wall.
  // Convert encoder count to inches. Save as cmd[1].
  // TODO: Code block to complete Step 3: Drive to treasure scout y-coordinate.
  // Drive forward 24 inches using drive() command.
  // TODO: Code block to complete Step 4: Drive from treasure scout to wall y-coordinate.
  // Reuse code from step two to count encoders to edge of the wall.
  // Convert encoder count to inches and add six inches to find center. Save as cmd[2].
  // TODO: Code block to complete Step 5: Read distance to wall in inches. Save as cmd[3].
  // TODO: Code block to complete Step 6: Reverse previous steps to return to start
  // TODO: Code block to complete Step 7: TX cmd[] to grabber bot
  drive(0,0);
}

void loop() {
  // INSERT LOOPING TX CODE HERE]
  // https://playground.arduino.cc/InterfacingWithHardware/Nrf2401
}

unsigned long find_wall() {
  inches = 150;
  while (inches > 110) {
    // Trigger the PING))).
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    // Read the signal from the PING))): a HIGH pulse whose duration is the time
    // (in microseconds) from the TX to RX echo off object.
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    // Time to distance conversion.
    inches = microsecondsToInches(duration);
    cm     = microsecondsToCentimeters(duration);

    // Serial debugging for non-headless mode.
    Serial.print(inches);
    Serial.println(" in,    ");

    delay(50);
    last_inches = inches;
    return last_inches;
    drive(50,50);
  }
}

unsigned long distance_measure() {
  pinMode(Ping_pin, OUTPUT);
  digitalWrite(Ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(Ping_pin, LOW);
  pinMode(Ping_pin, INPUT);
  return pulseIn(Ping_pin, HIGH) / 67UL / 2UL;    // returns distance in inches
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
