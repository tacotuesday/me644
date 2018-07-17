int keybrd = 0;   // for incoming serial data

void setup() {
  Serial.begin(9600);
}

void loop() {
  // send data only when you receive data:
  if (Serial.available() > 0) {
    keybrd = Serial.read(); // read the incoming byte
 
    Serial.print("keybrd = ");
    Serial.println(keybrd, DEC);
    Serial.print("is this an ASCII character? (0 no/1 yes) ");
    Serial.println(isAscii(keybrd));
    keybrd = keybrd - 48;
    
    switch (keybrd) {
      case 0:
        Serial.println("you typed a 0");
        break;
      case 1:
        Serial.println("you typed a 1");
        break;
      case 2:
        Serial.println("you typed a 2");
        break;
      default:
        Serial.println("you typed a # > 2 or non-numeric");
        break;
    }
    Serial.println();
  }
}
