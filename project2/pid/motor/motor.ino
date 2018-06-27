// PID DC motor control Example
#include <PID_v1.h>

// digital pins
int motor_pin = 2, speed_pin = 31;

// define PID variables
double measured_t_pass;
double control_signal_to_motor = 255;  // != 0 to start inital motor rotation.
                                       // necessary for read_encoder()
double desired_t_pass;
double error;

// specify links and initial tuning parameters
PID myPID(&measured_t_pass, &control_signal_to_motor, &desired_t_pass, 0.01, 0.1, 0.0, REVERSE);

void setup() {
  Serial.begin(9600);
  pinMode(motor_pin, OUTPUT);   // pin connected to base of npn to control motor speed
  pinMode(speed_pin,  INPUT);   // pin connected to optical encoder signal
  desired_t_pass = 90;          // set desired speed
  myPID.SetMode(AUTOMATIC);     // turn on PID
}

void loop() {
  analogWrite(motor_pin, control_signal_to_motor);
  measured_t_pass = read_encoder();
  error = desired_t_pass - measured_t_pass;
  Serial.print("measured_t_pass = "); Serial.print(measured_t_pass); Serial.print(", ");
  Serial.print("error = ");          Serial.print(error);          Serial.print(", ");
  Serial.print("control_signal_to_motor = "); Serial.println(control_signal_to_motor);
  myPID.Compute();
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
