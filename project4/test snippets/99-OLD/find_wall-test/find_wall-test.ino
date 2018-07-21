#include <Servo.h>

Servo right_servo, left_servo, Ping_servo;

const byte   right_servo_pin = 12;
const byte    left_servo_pin = 10;
const byte right_encoder_pin = 11;
const byte  left_encoder_pin =  9;
const byte    Ping_servo_pin =  8;
const byte          Ping_pin =  4;

int right_center_value = 1484;    // CHANGE THESE VALUES AFTER TESTING
int  left_center_value = 1504;    // NEW SERVOS
double    left_spd = 50;
double   right_spd = 50;
const int Ping_left = 180;
const int Ping_center = 96;
const int Ping_right = 6;
int test_var;

volatile  int cc_left, cc_right;       // encoder counters
unsigned long distance_in_inches;      // distance to wall
int cmd[4] = {0};                      // May need to change to float if Joe doesn't mind

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);
  attachInterrupt( right_encoder_pin, right_counter, CHANGE);

  look(Ping_right);               // turn the servo encoder 90 to the right
  delay(2000);                    // make sure PING))) servo is properly oriented
  // test_var = find_wall();         // drive forward, look for wall edge
  // cmd[0] = test_var;              // return horizontal distance to wall edge for debugging
  // drive(0,0);                     // stop the motors
  drive_x_inches(6);              // move forward six inches to measure true distance to wall
  cmd[1] = distance_measure();    // measure distance to find_wall
  drive_x_inches(12);             // move to center of bridge
}

void loop() {
  Serial.print("test_var: ");
  Serial.print(test_var);
  Serial.print("    cmd[0]: ");
  Serial.print(cmd[0]);
  Serial.print("    cmd[1]: ");
  Serial.print(cmd[1]);
  Serial.print("    cc_right: ");
  Serial.println(cc_right);

}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

unsigned long find_wall() {
  distance_in_inches = 150;
  while (distance_in_inches > 9) {
    drive(left_spd, right_spd);
    distance_in_inches = distance_measure();
    return distance_in_inches;
  }
}

void look(int Ping_servo_position) {
  Ping_servo.attach(Ping_servo_pin);
  Ping_servo.write( Ping_servo_position);
  delay(2000);
  Ping_servo.detach();
}

unsigned long distance_measure() {
  // The PING))) is triggered by a HIGH pulse of 2 or more microseconds.
  // Give a short LOW pulse beforehand to ensure a clean HIGH pulse:
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  // The same pin is used to read the signal from the PING))): a HIGH
  // pulse whose duration is the time (in microseconds) from the sending
  // of the ping to the reception of its echo off of an object.
  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  cm     = microsecondsToCentimeters(duration);

  Serial.print(inches);
  Serial.print(" in,    ");
  Serial.print(cm);
  Serial.print(" cm");
  Serial.println();

  delay(1000);
}

void drive_x_inches(int length) {
  int dist;
  cc_left = 0; cc_right = 0;
  dist = length * (12*64/8);
  while (cc_left < dist || cc_right < dist) {
    drive(right_spd, left_spd);
  }
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }

unsigned long microsecondsToInches(unsigned long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 67UL / 2UL;
  // 1 / ( 67 us/in * 1 s/1e6 us * 12 in / ft ) = 1243.8 ft/s = 379.1 m/s!
}

unsigned long microsecondsToCentimeters(unsigned long microseconds) {
  // The speed of sound is 340 m/s or 29 microseconds per centimeter.
  // The ping travels out and back, so to find the distance of the
  // object we take half of the distance travelled.
  return microseconds / 45UL / 2UL;
}
