// 19-ping_while -- Tests the PING's ability to operate conditionally

const int ping_pin = 7;
int cc1;
long duration, inches, garbage, real;

void setup() {
  Serial.begin(9600);   // initialize serial communication
}

void loop() {
  garbage = distance();
  delay(100);
  real = distance();
  while (real > 20 ) {
    Serial.println("YA BROKE IT");
    delay(100);
  }
}

long distance() {
  pinMode(ping_pin, OUTPUT);
  digitalWrite(ping_pin, LOW);
  delayMicroseconds(2);
  digitalWrite(ping_pin, HIGH);
  delayMicroseconds(5);
  digitalWrite(ping_pin, LOW);
  pinMode(ping_pin, INPUT);
  duration = pulseIn(ping_pin, HIGH);

  // convert the time into a distance
  inches = microsecondsToInches(duration);
  return inches;
}

// convert the time into a distance
// inches = microsecondsToInches(duration);}
unsigned long microsecondsToInches(unsigned long microseconds) {
  // According to Parallax's datasheet for the PING))), there are
  // 73.746 microseconds per inch (i.e. sound travels at 1130 feet per
  // second).  This gives the distance travelled by the ping, outbound
  // and return, so we divide by 2 to get the distance of the obstacle.
  // See: http://www.parallax.com/dl/docs/prod/acc/28015-PING-v1.3.pdf
  return microseconds / 67UL / 2UL;
  // 1 / ( 67 us/in * 1 s/1e6 us * 12 in / ft ) = 1243.8 ft/s = 379.1 m/s!
}
