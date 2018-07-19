#include <Servo.h>

// Create servo object
Servo test_servo;

// Pin definition
const byte servo_pin = 12;
const byte speed_pin = 2;
// Declare time-holding variables using long to avoid overflow
volatile long measured_t_pass = 100;
unsigned long test_time = 0;

// initial center values
int center_value = 2000;  // Set this to the lower or upper limit

void setup() {
  pinMode(speed_pin, INPUT_PULLUP);
  Serial.begin(9600);
  test_servo.attach(servo_pin);
  attachInterrupt(digitalPinToInterrupt(2), read_encoder, CHANGE);
  // Experimentally-determined t_pass at max motor speed = 20ms.
  // This is for ONE specific servo: modify the below value based on calibration
  // curve for the servo motor being evaluated.
  while(measured_t_pass >= 17) {
    test_time = millis(); // Start timing the servo
    test_servo.writeMicroseconds(center_value);
    measured_t_pass = read_encoder(); // Get time for one spoke pass
    Serial.print(center_value);
    Serial.print(", ");
    Serial.print(test_time);
    Serial.print(", ");
    Serial.print(measured_t_pass);
    Serial.println();
  }
}

void loop() { }


long read_encoder() {
  volatile long t_pass;
  int e_value = digitalRead(speed_pin);
  while (digitalRead(speed_pin) == e_value) { }
  t_pass = millis();
  while (digitalRead(speed_pin) != e_value) { }
  t_pass = millis() - t_pass;
  return t_pass;
}
