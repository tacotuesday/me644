//COOK.ino -- Submission for Quiz 5

//Define pinout
const byte armSwitch = 2;
const byte targetButton = 3;
const byte triggerButton = 4;
const byte launchLED = 5;

void setup() {
  Serial.begin(9600);
  // Read pin inputs
  pinMode(armSwitch, INPUT);
  pinMode(targetButton, INPUT);
  pinMode(triggerButton, INPUT);
  pinMode(launchLED, OUTPUT);
}

void loop() {
  // Assign variables based on state of input device
  byte armed = digitalRead(armSwitch);
  byte target = digitalRead(targetButton);
  byte trigger = digitalRead(triggerButton);
  byte launch = digitalRead(launchLED);

  // Set launch conditions
  if (trigger == 1 && (armed == 1 || target == 1)) {
    digitalWrite(launchLED, HIGH);
  } else { digitalWrite(launchLED, LOW); }

  // In case of emergency: what is the Arduino reading?
  // Serial.println(F("armed = "));
  // Serial.print(armed, DEC);
  // Serial.println(F("target = "));
  // Serial.print(target, DEC);
  // Serial.println(F("trigger = "));
  // Serial.print(trigger, DEC);
  // Serial.println(F("launch = "));
  // Serial.print(launch, DEC);
}
