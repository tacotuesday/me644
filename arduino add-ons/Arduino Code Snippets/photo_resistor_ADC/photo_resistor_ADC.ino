/* This demo controls the amount of light falling on a photo
 *  resistor by PWM to 3 LEDS. A switch turns on/off control.
 *  Place a coffee mug over the photo resistor & LEDs.
 */
 
// pin declaration
int analog_pin =  0;
int switch_pin =  8;
int LED_1_pin  =  9;
int LED_2_pin  = 10;
int LED_3_pin  = 11;

int time_delay = 1;             // loop delay (s)
int brightness;                 // value of photo resistor pin
int desired_brightness = 900;   // desired brightness level

int light_level = 0;                  // LEDs' level
int light_delta = 2;                  // LEDs' level step size
int light_max   = 255 - light_delta;  // LEDs' max level

void setup() {
  Serial.begin(9600);
  // I/O pin designation
  pinMode(switch_pin, INPUT);
  pinMode( LED_1_pin, OUTPUT);
  pinMode( LED_2_pin, OUTPUT);
  pinMode( LED_3_pin, OUTPUT);
}

void loop() {
  analogWrite(LED_1_pin, light_level);
  analogWrite(LED_2_pin, light_level);
  analogWrite(LED_3_pin, light_level);

  brightness = analogRead(analog_pin);
  Serial.println();
  Serial.print("brightness  = ");
  Serial.print(brightness);
  Serial.print(" (digital value) = ");
  Serial.print(brightness*3.3/1024);
  Serial.println(" (V)");

  switch (digitalRead(switch_pin)) {
    case 1:
      if (brightness  <  desired_brightness &&
          light_level <= light_max) {
        light_level = light_level + light_delta;
      }
      if (brightness  > desired_brightness &&
          light_level > light_delta) {
        light_level = light_level - light_delta;
      }
    Serial.print("light level = ");
    Serial.print(light_level);
    Serial.print(" (digital value) = ");
    Serial.print(light_level*3.3/255);
    Serial.print(" (V) = ");
    Serial.print(light_level*100/255);
    Serial.println(" (% duty cycle)");
  }
  delay(time_delay*1000);
}
