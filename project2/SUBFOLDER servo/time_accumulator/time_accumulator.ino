#include <Servo.h>

// create servo object to control a servo
Servo test_servo;

// pin definition
const byte servo_pin = 12;
const byte speed_pin = 7;
unsigned long measured_t_pass = 100;    // Use long for time-holding variables
unsigned long test_time = 0;

// initial center values
int center_value       = 2000;    // Set this to the lower or upper limit

void setup() {
  pinMode(speed_pin, INPUT_PULLUP);
  Serial.begin(9600);
  while(measured_t_pass >= 20) {  // Experimentally-determined top speed
    test_time = millis();
    test_servo.attach(servo_pin);
    test_servo.writeMicroseconds(center_value);
    measured_t_pass = read_encoder();
    Serial.print("Center Value = ");
    Serial.print(center_value);
    Serial.print(", ");
    Serial.print("Test time = ");
    Serial.print(test_time);
    Serial.print(", ");
    Serial.print("Measured T Pass = ");
    Serial.print(measured_t_pass);
    Serial.println();
  }
}

void loop() {
}

long read_encoder() {
  unsigned long t_pass;
  int e_value = digitalRead(speed_pin);
  while (digitalRead(speed_pin) == e_value) { }
  t_pass = millis();
  while (digitalRead(speed_pin) != e_value) { }
  t_pass = millis() - t_pass;
  return t_pass;
}
