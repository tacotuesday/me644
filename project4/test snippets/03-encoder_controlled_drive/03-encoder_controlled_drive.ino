#include <Servo.h>

Servo right_servo, left_servo, Ping_servo;

const byte   right_servo_pin = 12;
const byte    left_servo_pin = 10;
const byte right_encoder_pin = 11;
const byte  left_encoder_pin =  9;
const byte    Ping_servo_pin =  8;
const byte          Ping_pin =  7;

int right_center_value = 1484;    // CHANGE THESE VALUES AFTER TESTING
int  left_center_value = 1504;    // NEW SERVOS
double    left_spd = 50;
double   right_spd = 50;
const int Ping_left = 180;
const int Ping_center = 96;
const int Ping_right = 6;
int test_var = 0;

volatile  int cc_left, cc_right;       // encoder counters
unsigned long distance_in_inches;      // distance to wall
int cmd[4] = {0};                      // May need to change to float if Joe doesn't mind

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);
  attachInterrupt( left_encoder_pin,  left_counter, CHANGE);
  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attach_servos(1);

  look(Ping_right);               // turn the servo encoder 90 to the right
  delay(2000);                    // make sure PING))) servo is properly oriented
  // test_var = find_wall();         // drive forward, look for wall edge
  cmd[0] = test_var;              // return horizontal distance to wall edge for debugging
  drive(0,0);                     // stop the motors
  drive_x_feet(1);              // move forward six inches to measure true distance to wall
  cmd[1] = distance_measure();    // measure distance to find_wall
  // drive_x_feet(12);             // move to center of bridge
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

void attach_servos(int ats) {
  if ( ats == 0) { right_servo.detach(); left_servo.detach(); }
  else { right_servo.attach(right_servo_pin); left_servo.attach( left_servo_pin);
  Ping_servo.attach(Ping_servo_pin); }
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
  pinMode(       Ping_pin, OUTPUT);
  digitalWrite(  Ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(  Ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(  Ping_pin, LOW);
  pinMode(       Ping_pin, INPUT);
  return pulseIn(Ping_pin, HIGH) / 67UL / 2UL;
}

void drive_x_feet(int length) {
  int dist;
  cc_left = 0; cc_right = 0;
  dist = length * (12*64/8);
  while (cc_left < dist /*|| cc_right < dist*/) {
    drive(right_spd, left_spd);
  }
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; }
