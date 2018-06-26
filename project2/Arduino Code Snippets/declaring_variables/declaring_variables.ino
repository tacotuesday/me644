word x1 = 1000;
word x2;
byte y;
byte y2;
int X[6] = { 2, 3, 4, 5 };

// void - used only in function declarations. 
// It indicates that the function is expected to return 
// no information to the function from which it was called.
void setup() {
  x2 = 2000;
  y  = 256;
  y2 = 257;
  
  Serial.begin(9600);

  Serial.print("x1 = ");
  Serial.print(x1, DEC);
  Serial.println();       // carriage return
  
  Serial.print("x2 = ");
  Serial.print(x2, DEC);
  Serial.println();       // carriage return

  Serial.print("y = ");
  Serial.print(y, DEC);
  Serial.print(" = ");
  Serial.print(y, BIN);
  Serial.println();       // carriage return

  Serial.print("y2 = ");
  Serial.print(y2, DEC);
  Serial.print(" = ");
  Serial.print(y2, BIN);
  Serial.println();       // carriage return

  Serial.print("X(4) = ");
  Serial.print(X[4], DEC);
}

void loop() {
  // put your main code here, to run repeatedly:
}
