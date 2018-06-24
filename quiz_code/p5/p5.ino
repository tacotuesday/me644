// p5.ino -- Code for practice quiz 5, problem 5
const byte captain = 13;
const byte pilot = 12;
const byte atc = 11;
const byte authLED = 2;
byte sanityCheck = 0;

void setup() {
  Serial.begin(9600);
  pinMode(captain, INPUT);
  pinMode(pilot, INPUT);
  pinMode(atc, INPUT);
  pinMode(authLED, OUTPUT);
}

void loop() {
  byte capState = digitalRead(captain);
  byte pilotState = digitalRead(pilot);
  byte atcState = digitalRead(atc);
  if ((capState == 1 && pilotState == 1)||(atcState == 0 && (capState || pilotState == 1))) {
    digitalWrite(authLED, HIGH);
    sanityCheck = 1;
  }
  else {
    digitalWrite(authLED, LOW);
    sanityCheck = 0;
  }
  Serial.println(sanityCheck);
}
