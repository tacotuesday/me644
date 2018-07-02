// straightLinePid_v2.1 - program to make a robot travel in a straight line
// using the PIDLibrary.The bulk of the code was originally written by Chris
// Richards (UofL); modified for data types and formatting consistency.
#include <Servo.h>
#include <PID_v1.h>

// create servo object to control a servo
Servo rServo;
Servo lServo;

// pin definition
const byte rEncoderPin = 10;
const byte lEncoderPin = 9;
const byte rServoPin = 12;
const byte lServoPin = 11;

// servo center values & speeds
int rCenterVal = 1515;
int lCenterVal = 1501;

// encoder counter and desired travel distance
volatile int ccLeft;
// = desired distance ft * (12 in/ft * 64 encoder_changes/rotation / 8 in/rotation)
int distance = 10*(12*64/8);  // = # of 0.125" w fine encoder wheels

// PID variables & initialization
double dt;                            // time difference between encoders
double lSpeed = 50;
double rSpeed = 53;
double desiredDt  = 0;               // desired time difference between encoders
PID myPID(&dt, &rSpeed, &desiredDt, 0.01, 1, 0, DIRECT);

void setup() {
  Serial.begin(9600);         // initialize USB communication
  myPID.SetMode(AUTOMATIC);   //turn on PID
  myPID.SetOutputLimits( rSpeed - 30, rSpeed + 30 );

  pinMode(rEncoderPin, INPUT_PULLUP);
  pinMode(lEncoderPin, INPUT_PULLUP);
  attachInterrupt(lEncoderPin, lCounter, CHANGE);
  attachServos(1);
  orientEncoders();
  delay(5000);
}

void loop() {
  ccLeft = 0;
  while (ccLeft < distance) {
    drive(rSpeed, lSpeed);
    dt = readEncoders();
    Serial.print(ccLeft);    Serial.print(" ");
    Serial.print(dt);         Serial.print(" ");
    Serial.print(rSpeed);  Serial.println(";");
    myPID.Compute();
  }
  drive(0, 0); delay(1000);
  attachServos(0);
}
long readEncoders() {
  unsigned long tRightPass, tLeftPass;
  long deltaTPass;

  int rValue = digitalRead(rEncoderPin);
  while (digitalRead(rEncoderPin) == rValue) { }
  tRightPass = millis();
  while (digitalRead(rEncoderPin) != rValue) { }
  tRightPass = millis() - tRightPass;

  int lValue = digitalRead(lEncoderPin);
  while (digitalRead(lEncoderPin)  == lValue) { }
  tLeftPass  = millis();
  while (digitalRead(lEncoderPin)  != lValue) { }
  tLeftPass  = millis() - tLeftPass;

  return deltaTPass = tLeftPass - tRightPass;
}

void orientEncoders() {
  int rValue = digitalRead(rEncoderPin);
  drive(10 - 20*rValue, 0);
  while (digitalRead(rEncoderPin) == rValue) { }
  drive(0, 0);
  int lValue = digitalRead(lEncoderPin);
  drive(0, 20*lValue - 10);
  while (digitalRead(lEncoderPin) == lValue) { }
  drive(0, 0);
}

void attachServos(int ats) {
  if ( ats == 0) {
    rServo.detach(); lServo.detach();
  }
  else {
    rServo.attach(rServoPin); lServo.attach(lServoPin);
  }
}

void drive(double rSpeed, double lSpeed) {
  rServo.writeMicroseconds(rCenterVal - rSpeed);
   lServo.writeMicroseconds( lCenterVal +  lSpeed);
}

void  lCounter() { ccLeft++; }
