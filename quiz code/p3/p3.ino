//p3.ino -- Practice for Quiz 5, problem 3.

//Define pinout
const byte injSwitch = 2;
const byte polButton = 3;
const byte fireButton = 4;
const byte ambLED = 5;

void setup() {
  Serial.begin(9600);
  pinMode(injuries, INPUT);
  pinMode(police, INPUT_PULLUP);
  pinMode(fire, INPUT_PULLUP);
  pinMode(ambulance, OUTPUT);
}

void loop() {
  byte injuries = digitalRead(injSwitch);
  byte police = digitalRead(polButton);
  byte fire = digitalRead(fireButton);
  byte ambulance = digitalRead(ambLED);

  if ((police == 0 && (injuries == 1 || fire == 1)) || fire == 0) {
    digitalWrite(ambulance, HIGH);
  }
  digitalWrite(ambulance, HIGH);
  Serial.println(F("injuries = "));
  Serial.print(injuries, DEC);
  Serial.println(F("police = "));
  Serial.print(police, DEC);
  Serial.println(F("fire = "));
  Serial.print(fire, DEC);
  Serial.println(F("ambulance = "));
  Serial.print(ambulance, DEC);
}
