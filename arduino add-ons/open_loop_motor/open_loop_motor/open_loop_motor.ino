// use 9 V to drive motor

int cc1;                  // counter for averaging speed
int spd;                  // shaft speed ~ time of a spoke passing (us)

// analog pin (potentiometer to control motor speed)
int pot_pin = 1;

// pin definition
int motor_pin  = 2;
int speed_pin  = 31;

void setup() {
  Serial.begin(9600);
  pinMode(motor_pin, OUTPUT);
  pinMode(speed_pin,  INPUT);
}

void loop() {
  // calculate average time of spoke passings
  spd = 0;
  for (cc1 = 0; cc1 < 16; cc1++) {
    spd = spd + pulseIn(speed_pin, HIGH);
  }
  spd = spd/16;

  Serial.print("analog input voltage = ");
  Serial.print(analogRead(pot_pin)*3.3/1023);
  Serial.print(" (V)     pulse length = ");
  Serial.print(spd);
  Serial.println(" (us)");
  analogWrite(motor_pin, analogRead(pot_pin));
}
