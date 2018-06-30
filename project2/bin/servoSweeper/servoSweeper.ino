// servoSweeper.ino -- a program to sweep through a servo motor's range and
// record the time of an encoder window passing. It sweeps up and down to make
// validate the previously-found values. readEncoder() written by Christopher
// Richards and reformatted to camel case for style consistency.

#include <Servo.h>

// Create a servo object
Servo testServo;

// Define pinout
const byte servoPin = 12;
const byte speedPin = 11;
int offsetLockout = 15;  // Adjust this based on how your servo acts at center.
int defineCenter = 1469;  // Use center_check.ino to find this value for servo.
int servoInput;
unsigned long measuredTPass;
unsigned long delayLength = 350;  // Delay in microseconds: adjust accordingly.

void setup() {
  pinMode(speedPin, INPUT_PULLUP);
  Serial.begin(9600);
  Serial.print("Servo Input");
  Serial.print(", ");
  Serial.print("Measured T Pass");
  Serial.println();
  testServo.attach(servoPin);
  for (servoInput = 1350; servoInput <= (defineCenter - offsetLockout); ++servoInput) {
    testServo.writeMicroseconds(servoInput);
    measuredTPass = readEncoder();
    Serial.print(servoInput);
    Serial.print(", ");
    Serial.print(measuredTPass);
    Serial.println();
    delayMicroseconds(delayLength);

  }
  for (servoInput = (defineCenter + offsetLockout); servoInput <= 1550; ++servoInput) {
    testServo.writeMicroseconds(servoInput);
    measuredTPass = readEncoder();
    Serial.print(servoInput);
    Serial.print(", ");
    Serial.print(measuredTPass);
    Serial.println();
    delayMicroseconds(delayLength);
  }
  testServo.writeMicroseconds(defineCenter);
}

void loop() {

}

long readEncoder() {
  unsigned long tPass;

  int eValue = digitalRead(speedPin);
  while (digitalRead(speedPin) == eValue) { }
  tPass = millis();
  while (digitalRead(speedPin) != eValue) { }
  tPass = millis() - tPass;
  return tPass;
}
