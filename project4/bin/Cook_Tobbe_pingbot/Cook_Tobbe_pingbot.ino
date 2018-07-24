// Cook_Tobbe_pingbot.ino -- Production code for robot that uses a Parallax PING))) to find treasure by sensing walls.

#include <Servo.h>
#include <PID_v1.h>
#include <SPI.h>    // Serial Peripheral Interface Bus used for short distance communication
#include "RF24.h"   // transciever library

bool radioNumber = 0;   // Set this radio as radio number 0 or 1
RF24 radio(6,7);        // Set up nRF24L01 radio on SPI bus plus CE & CSN pins
byte addresses[][6] = {"1Node","2Node"};
int cmd[4] = {0};        // Initialize array to send to grabber bot
int n1, e1, e2, n2;      // Declare variables that hold array values

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
int  left_center_value = 1497;
int   Ping_right_value = 6;
int  Ping_center_value = 96;
int    Ping_left_value = 180;

// initialize variables, counters, and desired travel distance
// feet traveled = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
byte runonce = 1;                                   // Loop control variable
unsigned long duration, meas_distance;
volatile int cc_left, cc_right;
volatile int Ping_measure;                                   // Declaring Ping_measure globally to debounce in setup() and be read from memory

// PID variables & initialization
double dt;                                          // time difference between encoders
double desired_dt  = 0;                             // desired time difference between encoders
double    left_spd = 100;
double   right_spd = 137;
double  degrees_90 = 25;
// PID myPID(&dt, &right_spd, &desired_dt, 0.084457214, 7.712170356, 0.014842, DIRECT);

void setup() {
  Serial.begin(115200);                             // Initialize serial communication
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
  attachInterrupt( left_encoder_pin,  left_count, CHANGE);
  attachInterrupt( right_encoder_pin, right_count, CHANGE);

  Ping_measure = distance_measure();                // Included to keep PING from reporting a zero to the first WHILE loop
}

void loop() {
  if (runonce = 1) {
    look(Ping_center_value);                        // Orient the PING))) straight ahead.
    attach_servos(1); delay(100); /* orient_encoders(); */ delay(500);
    look(Ping_right_value);                         // Orient the PING))) 90 degrees to the right.

    // Search for first wall surrounding bridge.
    cc_left = 0; cc_right = 0;                      // reset encoder counts
    Ping_measure = 150;                             // Initialize a measured range to enter the while loop
    int max_range = 84;                             // Most detectable surfaces will probably be <= 7 ft from sensor
    while (Ping_measure > max_range) {              // This is block is the template for much of the robot's motion.
      drive(right_spd, left_spd);                   // Drive forward a pre-determined speed
      // dt = read_encoders(); myPID.Compute();        // PID calculations
      Ping_measure = distance_measure();            // Search with Ping
      Serial.println(F("Finding bridge wall..."));  // Print to the terminal while still in the loop
    }
    delay(100); drive(0, 0); delay(100); /*orient_encoders();*/ delay(100);
    n1 = cc_left / 8;                               // Converts raw encoder count to inches. n1 = y-coordinate distance to wall start -> cmd[0].
    e1 = distance_measure();                        // e1 = x-coordinate distance to wall -> cmd[1]
    delay(100);


    // Move forward 18 inches to align with bridge.
    cc_left = 0; cc_right = 0;
    while (cc_left < 144) {
      drive(right_spd, left_spd);
      // dt = read_encoders(); myPID.Compute();
      Serial.println(F("Aligning with bridge..."));
    }
    delay(100); /*orient_encoders();*/ delay(100);

    // Turn 90 degrees right to cross the bridge. Rotation is slow to avoid overshoot.
    cc_left = 0; cc_right = 0;
    while (cc_left < degrees_90 || cc_right < degrees_90) {
      drive(-right_spd, left_spd);
      // dt = read_encoders(); myPID.Compute();
      Serial.println(F("Turning right..."));
    }
    delay(100); drive(0, 0); delay(100); /*orient_encoders();*/ delay(100);
    look(Ping_left_value); delay(100);              // orient the Ping toward expected treasure direction

    // Cross the bridge to grabber bot's y-coordinate: e1 + 2'.
    cc_left = 0; cc_right = 0;
    while (cc_left < (e1 + 24) * (64 / 8)) {
      drive(left_spd, right_spd);
      // dt = read_encoders(); myPID.Compute();
      Serial.println(F("Crossing the bridge..."));
    }
    delay(100); drive(0, 0); delay(100); /*orient_encoders();*/ delay(100);

    // Move forward to find the treasure
    Ping_measure = 150;                             // Reset Ping_measure to search for wall
    while (Ping_measure > max_range) {
      drive(right_spd, left_spd);
      // dt = read_encoders(); myPID.Compute();
      Ping_measure = distance_measure();
      Serial.println(F("Finding treasure..."));
    }
    delay(100); drive(0, 0); delay(100); /*orient_encoders();*/ delay(100);
    e2 = cc_left / 8;                               // e2 = y-coordinate distance to wall start -> cmd[2]
    n2 = distance_measure();                        // n2 = x-coordinate distance to wall -> cmd[3]
    delay(100);


    // Move out of grabber bot's path by travelling 24 inches.
    cc_left = 0; cc_right = 0;
    while (cc_left < 24 * (64 / 8)) {
      drive(left_spd, right_spd);
      // dt = read_encoders(); myPID.Compute();
      Serial.println(F("Search complete!")); }
    delay(100); drive(0, 0);

    // Print and transmit found distances.
    Serial.println(n1); Serial.println(e1); Serial.println(e2); Serial.println(n2);
    cmd[0] = n1; cmd[1] = e1; cmd[2] = e2; cmd[3] = n2;

    if (!radio.write( &cmd, sizeof(cmd) )) { Serial.println(F("TX Failed!")); }
    attach_servos(0); runonce = 0;
  }
}

void look(int Ping_servo_position) {
  Ping_servo.attach(Ping_servo_pin);
  Ping_servo.write( Ping_servo_position);
  delay(2000);
  Ping_servo.detach();
}

int distance_measure() {                            // Returns INT for easier integration into TX array.
  pinMode(Ping_pin, OUTPUT);                        // No values >= 32,767 expected -> low chance of overflow.
  digitalWrite(Ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(Ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(Ping_pin, LOW);
  pinMode(Ping_pin, INPUT);
  duration = pulseIn(Ping_pin, HIGH);
  meas_distance = duration / 67UL / 2UL;
  if (meas_distance > 0) {return meas_distance;}
  else {
    pinMode(Ping_pin, OUTPUT);                        // Another attempt to debounce zero-first-value problem.
    digitalWrite(Ping_pin, LOW);
    delayMicroseconds(2);
    digitalWrite(Ping_pin, HIGH);
    delayMicroseconds(5);
    digitalWrite(Ping_pin, LOW);
    pinMode(Ping_pin, INPUT);
    duration = pulseIn(Ping_pin, HIGH);
    meas_distance = duration / 67UL / 2UL;
    return meas_distance;
    }
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

void  left_count() { cc_left++; }
void  right_count() { cc_right++; }
