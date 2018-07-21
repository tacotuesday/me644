#include <Servo.h>

Servo Ping_servo;

const byte   right_servo_pin = 12;
const byte    left_servo_pin = 10;
const byte right_encoder_pin = 11;
const byte  left_encoder_pin =  9;
const byte    Ping_servo_pin =  8;
const byte          Ping_pin =  7;
int       center_value = 90;
int control_mode = 0;
int keybrd;
// const int Ping_center = 60;
// const int Ping_right = 0;


void setup() {
  // Ping_servo.attach(Ping_servo_pin);
  // Ping_servo.write(Ping_center);
  // delay(3000);
  // Ping_servo.write(Ping_left);
  // delay(3000);
  // Ping_servo.write(Ping_right);
  // delay(3000);
  Serial.begin(9600);
  Serial.println("keyboard command definition:");
  Serial.println("1 = test right servo");
  Serial.println("2 = increment active servo by 1");
  Serial.println("3 = decrement active servo by 1");
  Serial.println("4 = increment active servo by 10");
  Serial.println("5 = decrement active servo by 10");
}

void loop() {
  if (Serial.available() > 0) {
    keybrd = Serial.read(); // read the incoming byte
    keybrd = keybrd - 48;
    switch (keybrd) {
      case 1:
        Serial.println("TESTING PING SERVO");
        Ping_servo.attach(Ping_servo_pin);
        Serial.println("");
      break;
      case 2:  center_value = center_value + 1;  break;
      case 3:  center_value = center_value - 1;  break;
      case 4:  center_value = center_value + 10; break;
      case 5:  center_value = center_value - 10; break;
    }
    Serial.print( "Current value = ");
    Serial.println(center_value);
    keybrd = 0;
    if (control_mode == 0) { Ping_servo.write(center_value); }
    else { Ping_servo.writeMicroseconds(center_value); }
  }
}
