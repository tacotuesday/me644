/********************************************************
 * PID Basic Example
 * Simply illustrates how output tracks the input based  on PID Direction argument.
 * Reading analog input pin 1 from a potentiometer to SIMULATE measured motor speed.
 * Sending PWM controller output signal to pin 2 to control motor speed.
 * Note:  since the measured motor speed is simulated by the potentiometer and there
 * is no feeback, output simply trends to limits to try to adjust input, which it
 * cannot physically do!
 * last update:  06-24-18 CMR
 ********************************************************/

#include <PID_v1.h>

// digital pins
int motor_pin  =  2;

// define PID variables
double measured_speed, control_signal_to_motor, desired_speed, error;

// specify links and initial tuning parameters
// use Direction = DIRECT  to INCREASE controller output when e = r - y > 0 
//PID myPID(&measured_speed, &control_signal_to_motor, &desired_speed, 2, 5, 1, DIRECT);

// use Direction = REVERSE to DECREASE controller output when e = r - y > 0 
PID myPID(&measured_speed, &control_signal_to_motor, &desired_speed, 2, 5, 1, REVERSE);

void setup() {
  Serial.begin(9600);
  pinMode(motor_pin, OUTPUT);   // pin connected to base of npn to control motor speed
  desired_speed = 500;          // set desired speed
  myPID.SetMode(AUTOMATIC);     // turn the PID on
}

void loop() {
  analogWrite(motor_pin, control_signal_to_motor);
  measured_speed = analogRead(1);
  error = desired_speed - measured_speed;
  Serial.print("measured_speed = "); Serial.print(measured_speed); Serial.print(", ");
  Serial.print("error = ");          Serial.print(error);          Serial.print(", ");
  Serial.print("control_signal_to_motor = "); Serial.println(control_signal_to_motor);
  myPID.Compute();
}
