//p2.ino -- Practice for quiz 5, problem 2.

const byte grButton = 13;
const byte rButton = 12;
const byte yButton = 11;
const byte grLED = 5
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
    }
    delay(50);
  }
  lastGrState = grState;
  if (rState != lastRState) {
    if (rState == HIGH) {
      rCounter++;
    }
    delay(50);
  }
  lastRState = rState;
  if (yState != lastYState) {
    if (yState == HIGH) {
      grCounter++;
    }
    delay(50);
  }
  lastYState = yState;
}
