// This demo turns and LED on/off based on switch state
/* to make these variables GLOBAL, they are defined outside
   of functions */
int input_pin  = 8; // Don't have to do this.
int output_pin = 9; // Good practice for code portability.

int input_value;

void setup() {
  Serial.begin(9600); // must be consistent with Serial Monitor
  pinMode( input_pin, INPUT);
  pinMode(output_pin, OUTPUT);
}

void loop() {
  input_value = digitalRead(input_pin);
  Serial.print("the state of input pin 8 = ");
  Serial.println(input_value, DEC);

  digitalWrite(output_pin, input_value);
  delay(2000);
}
