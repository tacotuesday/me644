char myStr[] = "this is a test";  // length = 14 + 1 = 15
int i;

void setup(){
  Serial.begin(9600);
}

void loop() { 
// run to illustrate extra character string:
  for (i = 0; i < sizeof(myStr); i++) {
//  for (i = 0; i < sizeof(myStr) - 1; i++) {
    Serial.print(i, DEC);
    Serial.print(" = ");
    Serial.print(myStr[i]);
    Serial.println();       // carriage return
  }
  delay(5000); // slow down the program (ms)
}
