int cc;  // the counter cc can be initiallized within the for statement:
         // for (int cc = ...
         // However, if I want to recycle cc as a counter for multiple for
         // loops, it has to be initialized outside of the for statement.

void setup() {
  Serial.begin(9600);
  for (cc = 0; cc < 6; cc ++) {
    Serial.print("cc = ");
    Serial.print(cc, DEC);
    Serial.print("; cc^2 = ");
    Serial.println(pow(cc, 2), DEC);
    // pow(cc, 2) == cc^2 (double precision result)
  }
  Serial.println();

  for (cc = -8; cc < 10; cc += 2) {
    Serial.print("cc = ");
    Serial.print(cc, DEC);
    Serial.print("; sqrt(cc) = ");
    Serial.println(sqrt(cc), DEC);
    // C, the Arduino programming language backbone, does not handle complex #'s
  }
  Serial.println();
}

int PWMpin = 9;
void loop() {
  int x = 1;
  for (cc = 0; cc > -1; cc = cc + x){
    analogWrite(PWMpin, cc);
    if (cc == 255) x = -1;             // switch direction at peak
    delay(10);                         // argument for delay is ms
  } 
}
