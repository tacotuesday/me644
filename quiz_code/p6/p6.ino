//p2.ino -- Practice for quiz 5, problem 2.

const byte opSwitch = 13;
const byte buttonOne = 12;
const byte buttonTwo = 11;
const byte ledPin = 5;

int switchState = 0;
byte oneState = 0;
byte lastOneState = 0;
int oneCounter = 0;
int oneTotal = 0;
byte twoState = 0;
byte lastTwoState = 0;
int twoCounter = 0;
int twoTotal = 0;
int grandTotal = 0;

void setup() {
  pinMode(opSwitch, INPUT);
  pinMode(buttonTwo, INPUT);
  pinMode(buttonOne, INPUT);
  Serial.begin(9600);
}

void loop() {
  switchState = digitalRead(opSwitch);
  oneState = digitalRead(buttonOne);
  twoState = digitalRead(buttonTwo);
  if (oneState != lastOneState) {
    if (oneState == HIGH) {
      oneCounter++;
    }
    oneTotal = oneCounter;
    delay(50);
  }
  lastOneState = oneState;
  if (twoState != lastTwoState) {
    if (twoState == HIGH) {
      twoCounter++;
    }
    twoTotal = twoCounter;
    delay(50);
  }
  lastTwoState = twoState;
  if (switchState > 0) { digitalWrite(ledPin, HIGH); }
  switch (switchState) {
    case 0:
      grandTotal = 0;
      grandTotal = oneTotal + twoTotal;
      Serial.println(grandTotal);
      break;
    case 1:
      grandTotal = 0;
      grandTotal = oneTotal - twoTotal;
      Serial.println(grandTotal);
      break;
  }
}
