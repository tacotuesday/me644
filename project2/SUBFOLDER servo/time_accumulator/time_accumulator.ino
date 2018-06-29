#include <Servo.h>

// specify control mode (0 = .write, 1 = .writeMicroseconds)
int control_mode = 1;

// create servo object to control a servo
Servo test_servo;

// pin definition
const byte servo_pin = 12;
const byte speed_pin = 7;
unsigned int measured_t_pass = 0;
unsigned long test_time = 0;

// initial center values
int center_value       = 90;    // when using .write
int center_value_micro = 2000;  //when using .writeMicroseconds

int keybrd;   // keyboard command variable

void setup() {
  pinMode(speed_pin, INPUT_PULLUP);
  if (control_mode == 1) { center_value = center_value_micro; }
  Serial.begin(9600);
  Serial.println("keyboard command definition:");
  Serial.println("1 = Test servo");
}

void loop() {
  if (Serial.available() > 0) {
    keybrd = Serial.read(); // read the incoming byte
    keybrd = keybrd - 48;
    switch (keybrd) {
      case 1:
        test_servo.attach(servo_pin);
        Serial.println("");
      break;
    }

    // while(measured_t_pass >= 20) {
    //   measured_t_pass = read_encoder();
    //   test_time = millis(); }

    Serial.print( "Center Value = ");
    Serial.print(center_value);
    Serial.print(", ");
    Serial.print( "Test time = ");
    Serial.print(test_time);
    Serial.print(", ");
    Serial.print("Measured T Pass = ");
    Serial.print(measured_t_pass);
    Serial.println();


  keybrd = 0;
  if (control_mode == 0) { test_servo.write(center_value); }
  else { test_servo.writeMicroseconds(center_value); }
  }
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
