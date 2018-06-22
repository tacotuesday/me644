//p3.ino -- Practice for Quiz 5, problem 3.

//Define pinout
bool injuries = 2;
bool police = 3;
bool fire = 4;
bool ambulance = 5;

void setup() {
  Serial.begin(9600);
  pinMode(injuries, INPUT);
  pinMode(police, INPUT_PULLUP);
  pinMode(fire, INPUT_PULLUP);
  pinMode(ambulance, OUTPUT);
}

void loop() {
  if ((police == 0 && (injuries == 1 || fire == 1)) || fire == 0) {
    digitalWrite(ambulance, HIGH);
  }
  Serial.println("injuries = ");
  Serial.print(injuries, DEC);
  Serial.println("police = ");
  Serial.print(police, DEC);
  Serial.println("fire = ");
  Serial.print(fire, DEC);
  Serial.println("ambulance = ");
  Serial.print(ambulance, DEC);
}
