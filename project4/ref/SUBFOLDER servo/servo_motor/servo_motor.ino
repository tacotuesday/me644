#include <Servo.h>              // include servo motor library

// Create servo objects
Servo contServo;
Servo stdServo;

// pin definition
const byte swPin = 8;
const byte contServoPin = 4;
const byte stdServoPin = 3;
const byte potPin = 1;

int potVal;    // poteniometer value
int keybrd = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);           // initialize serial communication
  pinMode(swPin, INPUT);   // set swPin to input mode

  // attaches continuous servo object to pin contServoPin
  contServo.attach(contServoPin);

  // attaches standard servo object to pin stdServoPin
  stdServo.attach(stdServoPin);
}

void loop() {
  // DAC of potentiometer input voltage
  potVal = analogRead(potPin);

  // scale value to use with servo (0 - 180)
  potVal = map(potVal, 0, 1023, 0, 180);

  Serial.print("switch = ");
  Serial.print(digitalRead(swPin));
  Serial.print(" ;  potVal = ");
  Serial.print(potVal);
  Serial.print(" ;  keybrd = ");
  Serial.println(keybrd);

  switch (digitalRead(swPin)) {
    case 0:
      // set continuous servo speed according to scaled value
      contServo.write(potVal);
      delay(15);                // allow motor to catch up
      break;
    case 1:
      // set standard servo position according to keyboard input
      if (Serial.available() > 0) {
      keybrd = Serial.read(); // read the incoming byte
      keybrd = 20*(keybrd - 48); }

      stdServo.write(keybrd);
      delay(15);                // allow motor to catch up
      break;
  }
}
