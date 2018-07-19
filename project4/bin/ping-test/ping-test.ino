const int pingPin = 8;  // sensor signal pin
// establish variables for ping duration distance (in., cm)
unsigned long duration, inches, last_inches, display_inches, cm;

void setup() {
  Serial.begin(9600);   // initialize serial communication

  test666 = find_wall();
  Serial.print("The last recorded value was ");
  Serial.println(test_666);
}

void loop() {

}

unsigned long find_wall() {
  inches = 150;
  while (inches > 110) {
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
    Serial.print(" in,    ");
    Serial.print(cm);
    Serial.print(" cm");
    Serial.println();

    delay(100);
    last_inches = inches;
    return last_inches;
  }
}

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
