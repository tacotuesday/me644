#include <Servo.h>

Servo right_servo, left_servo, Ping_servo;

const byte   right_servo_pin = 12;
const byte    left_servo_pin = 13;
const byte right_encoder_pin = 10;
const byte  left_encoder_pin = 11;
const byte          Ping_pin =  8;
const byte    Ping_servo_pin =  9;

int right_center_value = 1484;    // CHANGE THESE VALUES AFTER TESTING
int  left_center_value = 1504;    // NEW SERVOS
int test_var;

volatile  int cc_left;        // left encoder counter
unsigned long inches;         // distance to wall
float cmd[];                  // May need to change to int if Joe can't handle a float

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);

  look(180);
  test_var = find_wall();
}

void loop() {
  Serial.print(test_var);
}

unsigned long find_wall() {
  inches = 150;
  while (inches > 110) {
    drive(50,50);
    
    // Trigger the PING))).
    pinMode(pingPin, OUTPUT);
    digitalWrite(pingPin, LOW);
    delayMicroseconds(2);
    digitalWrite(pingPin, HIGH);
    delayMicroseconds(5);
    digitalWrite(pingPin, LOW);

    // Read the signal from the PING))): a HIGH pulse whose duration is the time
    // (in microseconds) from the TX to RX echo off object.
    pinMode(pingPin, INPUT);
    duration = pulseIn(pingPin, HIGH);

    // Time to distance conversion.
    inches = microsecondsToInches(duration);
    cm     = microsecondsToCentimeters(duration);

    // Serial debugging for non-headless mode.
    Serial.print(inches);
    Serial.println(" in,    ");

    delay(50);
    last_inches = inches;
    return last_inches;
  }
}

void look(int Ping_servo_position) {
  Ping_servo.attach(Ping_servo_pin);
  Ping_servo.write( Ping_servo_position);
  delay(2000);
  Ping_servo.detach();
}
