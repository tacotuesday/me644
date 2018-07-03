#include <Servo.h>

// Create servo object
Servo test_servo;

// Pin definition
const byte servo_pin = 12;
const byte speed_pin = 7;
// Declare time-holding variables using long to avoid overflow
unsigned long measured_t_pass = 100;
unsigned long test_time = 0;

// initial center values
int center_value = 2000;  // Set this to the lower or upper limit

void setup() {
  pinMode(speed_pin, INPUT_PULLUP);
  Serial.begin(9600);
  // Experimentally-determined t_pass at max motor speed = 20ms.
  // This is for ONE specific servo: modify the below value based on calibration
  // curve for the servo motor being evaluated.
  while(measured_t_pass >= 19) {
    test_servo.attach(servo_pin);
    test_time = millis(); // Start timing the servo
    test_servo.writeMicroseconds(center_value);
    measured_t_pass = read_encoder(); // Get time for one spoke pass
    Serial.print("Center Value");
    Serial.print(", ");
    Serial.print("Test time");
    Serial.print(", ");
    Serial.print("Measured T Pass");
    Serial.print("");
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
  unsigned long t_pass;
  int e_value = digitalRead(speed_pin);
  while (digitalRead(speed_pin) == e_value) { }
  t_pass = millis();
  while (digitalRead(speed_pin) != e_value) { }
  t_pass = millis() - t_pass;
  return t_pass;
}
