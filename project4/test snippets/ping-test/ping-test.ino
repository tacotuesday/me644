const int pingPin = 4;  // sensor signal pin
// establish variables for ping duration distance (in., cm)
unsigned long duration, inches, last_inches, display_inches, microseconds;

unsigned long microsecondsToInches(unsigned long microseconds) {
  return microseconds / 67UL / 2UL;
  }

void setup() {
  Serial.begin(9600);   // initialize serial communication

  // while (inches > 5)  {
  //       inches = getDistance();
  //       Serial.println(inches);
  // }

}

void loop() {
  while (inches > 5)  {
        inches = getDistance();
  }
  Serial.println(inches);
}

unsigned long getDistance() {
  pinMode(pingPin, OUTPUT);
  digitalWrite(pingPin, LOW);
  delayMicroseconds(2);
  digitalWrite(pingPin, HIGH);
  delayMicroseconds(5);
  digitalWrite(pingPin, LOW);

  pinMode(pingPin, INPUT);
  duration = pulseIn(pingPin, HIGH);

  inches = microseconds / 67UL / 2UL;
  return inches;
}
