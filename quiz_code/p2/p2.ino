//p2.ino -- Practice for quiz 5, problem 2.

const byte grButton = 13;
const byte rButton = 12;
const byte yButton = 11;
const byte grLED = 5;
const byte rLED = 4;
const byte yLED = 3;

int grCounter = 0;
byte grState = 0;
byte lastGrState = 0;
int yCounter = 0;
byte yState = 0;
byte lastYState = 0;
int rCounter = 0;
byte rState = 0;
byte lastRState = 0;

void setup() {
  pinMode(grButton, INPUT);
  pinMode(yButton, INPUT);
  pinMode(rButton, INPUT);
  pinMode(grLED, OUTPUT);
  pinMode(rLED, OUTPUT);
  pinMode(yLED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  grState = digitalRead(grButton);
  rState = digitalRead(rButton);
  yState = digitalRead(yButton);
  if (grState != lastGrState) {
    if (grState == HIGH) {
      grCounter++;
      Serial.print("number of green pushes: ");
      Serial.println(grCounter);
    }
    delay(50);
  }
  lastGrState = grState;
  if (rState != lastRState) {
    if (rState == HIGH) {
      rCounter++;
      Serial.print("number of red pushes: ");
      Serial.println(rCounter);
    }
    delay(50);
  }
  lastRState = rState;
  if (yState != lastYState) {
    if (yState == HIGH) {
      yCounter++;
      Serial.print("number of yellow pushes: ");
      Serial.println(yCounter);
    }
    delay(50);
  }
  lastYState = yState;
  if (grCounter > rCounter && grCounter > yCounter) {
    digitalWrite(rLED, LOW);
    digitalWrite(yLED, LOW);
    digitalWrite(grLED, HIGH);
   }
  else if (rCounter > grCounter && rCounter > yCounter) {
    digitalWrite(grLED, LOW);
    digitalWrite(yLED, LOW);
    digitalWrite(rLED, HIGH);
  }
   else if (yCounter > grCounter && yCounter > rCounter) {
    digitalWrite(grLED, LOW);
    digitalWrite(rLED, LOW);
    digitalWrite(yLED, HIGH);
  }
}
