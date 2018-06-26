#include <Servo.h>              // include servo motor library

// create servo objects to control continuous and standard servo
Servo continuous_servo;
Servo   standard_servo;

// pin definition
int           switch_pin = 8;
int continuous_servo_pin = 4;
int   standard_servo_pin = 3;
int    potentiometer_pin = 1;

int pot_value;    // poteniometer value
int keybrd = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);           // initialize serial communication
  pinMode(switch_pin, INPUT);   // set switch_pin to input mode
  
  // attaches continuous servo object to pin continuous_servo_pin
  continuous_servo.attach(continuous_servo_pin);
  
  // attaches standard servo object to pin standard_servo_pin
    standard_servo.attach(  standard_servo_pin);  
}

void loop() {
  // DAC of potentiometer input voltage
  pot_value = analogRead(potentiometer_pin);

  // scale value to use with servo (0 - 180)
  pot_value = map(pot_value, 0, 1023, 0, 180);
  
  Serial.print("switch = ");
  Serial.print(digitalRead(switch_pin));
  Serial.print(" ;  pot_value = ");
  Serial.print(pot_value);
  Serial.print(" ;  keybrd = ");
  Serial.println(keybrd);

  switch (digitalRead(switch_pin)) {
    case 0:
      // set continuous servo speed according to scaled value
      continuous_servo.write(pot_value);
      delay(15);                // allow motor to catch up
      break;
    case 1:
      // set standard servo position according to keyboard input
      if (Serial.available() > 0) {
      keybrd = Serial.read(); // read the incoming byte
      keybrd = 20*(keybrd - 48); }

      standard_servo.write(keybrd);
      delay(15);                // allow motor to catch up
      break;
  }
}
