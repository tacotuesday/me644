void setup() {
  Serial.begin(9600);
  // NOTICE the for loop counter is initialized within the statement
  for (int cc = 0; cc < 4; cc++) {
    switch (cc) {
      // can have a case 0! run with case 1 -> case 0
      case 2:
        Serial.println("operating inside case 2");
        // println is print with a carriage return
        break;
      case 1:
        Serial.println("operating inside case 1");
        break;
      default: 
        Serial.print("operating inside default case\n");
        // "\n" is also a carriage return
        break;
    }
  }
  Serial.println();
}

void loop() {
  // put your main code here, to run repeatedly:
}
