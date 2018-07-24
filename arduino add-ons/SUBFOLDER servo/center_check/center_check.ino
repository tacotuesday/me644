#include <Servo.h>

// specify control mode (0 = .write, 1 = .writeMicroseconds)
int control_mode = 1;

// create servo object to control a servo
Servo test_servo;

// pin definition
int right_servo_pin = 10;
// int  left_servo_pin = 11;
int speed_pin = 11;
int spd;
int cc1;

// initial center values
int center_value       = 90;    // when using .write
int center_value_micro = 1500;  //when using .writeMicroseconds

int keybrd;   // keyboard command variable

void setup() {
  pinMode(speed_pin, INPUT_PULLUP);
  if (control_mode == 1) { center_value = center_value_micro; }
  Serial.begin(9600);
  Serial.println("keyboard command definition:");
  Serial.println("1 = test right servo");
  Serial.println("2 = test  left servo");
  Serial.println("3 = increment active servo by 1");
  Serial.println("4 = decrement active servo by 1");
  Serial.println("5 = increment active servo by 10");
  Serial.println("6 = decrement active servo by 10");
}

void loop() {
  if (Serial.available() > 0) {
    keybrd = Serial.read(); // read the incoming byte
    keybrd = keybrd - 48;
    switch (keybrd) {
      case 1:
        Serial.println("TESTING RIGHT SERVO");
        test_servo.attach(right_servo_pin);
        Serial.println("");
      break;
      // case 2:
      //   Serial.println("TESTING LEFT SERVO");
      //   test_servo.attach(left_servo_pin);
      break;
      case 3:  center_value = center_value + 1;  break;
      case 4:  center_value = center_value - 1;  break;
      case 5:  center_value = center_value + 10; break;
      case 6:  center_value = center_value - 10; break;
    }
    Serial.print( "center_value = ");
    Serial.println(center_value);
    // spd = 0;
    // for (cc1 = 0; cc1 < 16; cc1++) {
    //   spd = spd + pulseIn(speed_pin, HIGH);
    // }
    // spd = spd/16;
    // Serial.print(spd);
    // Serial.println(" (us)");
  }
  keybrd = 0;
  if (control_mode == 0) { test_servo.write(center_value); }
  else { test_servo.writeMicroseconds(center_value); }
}
