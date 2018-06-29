unsigned long testTime = 0;
int cc = 0;

void setup() {
  Serial.begin(9600);
  while(cc < 250) {
    cc++;
    testTime = millis();
    Serial.print("cc: ");
    Serial.println(cc);
    Serial.println(" ");
    Serial.print("Test time: ");
    Serial.println(testTime);
  }
}

void loop() {
}
