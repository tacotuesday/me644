/*  This demo controls the amount of light falling on a photo-resistor
 *  by PWM to 3 LEDS and PID feedback control.
 *  Place a coffee mug over the photo resistor & LEDs.*/

#include <PID_v1.h>

// pin declaration
int analog_pin =  0;
int LED_1_pin  =  9;
int LED_2_pin  = 10;
int LED_3_pin  = 11;

// PID variables
double brightness;                 // value of photo resistor pin
double desired_brightness = 800;   // desired brightness level
double LED_level;                  // duty cycle to LEDs (0 - 255)

//Specify the links and initial tuning parameters
// works
//PID myPID(&brightness, &LED_level, &desired_brightness, .01, 1, 0, DIRECT);
// saturates output causing oscillatory behavior
PID myPID(&brightness, &LED_level, &desired_brightness, .1, 10, 0, DIRECT);

void setup() {
  Serial.begin(9600);
  // I/O pin designation
  pinMode( LED_1_pin, OUTPUT);
  pinMode( LED_2_pin, OUTPUT);
  pinMode( LED_3_pin, OUTPUT);

  myPID.SetMode(AUTOMATIC);   //turn on PID
}

void loop() {
  brightness = analogRead(analog_pin);
  Serial.println();
  Serial.print("brightness  = ");
  Serial.print(brightness);
  Serial.print(" (digital value) = ");
  Serial.print(brightness*3.3/1024);
  Serial.println(" (V)");

  myPID.Compute();
  analogWrite(LED_1_pin, LED_level);
  analogWrite(LED_2_pin, LED_level);
  analogWrite(LED_3_pin, LED_level);
  Serial.print("LED level   = ");
  Serial.print(LED_level);
  Serial.print(" (digital value) = ");
  Serial.print(LED_level*3.3/255);
  Serial.print(" (V) = ");
  Serial.print(LED_level*100/255);
  Serial.println(" (% duty cycle)");
  delay(1000);
}
