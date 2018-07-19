                                            #include <Servo.h>

// create servo object
Servo right_servo, left_servo;

// pin definition
const byte  left_encoder_pin = 10;
const byte right_encoder_pin =  9;
const byte    left_servo_pin = 11;
const byte   right_servo_pin = 12;

// servo center values & speeds
int right_center_value = 1511;
int  left_center_value = 1509;
int left_count;
int a;
double  left_spd       =  92;
double right_spd       = 100;

// encoder counter and desired travel distance
volatile int cc_left, cc_right;   // encoder counters
volatile long enc_count;
int cc;                           // # times out and back
// int distance = 3*(12*64/8);  // = # of 0.125" w fine encoder wheels = 3 foot distance
// int turn_180 = 51;                // # encoder counts to cause it to turn 180 deg. turn

void setup() {
  Serial.begin(9600);
  pinMode(right_encoder_pin, INPUT_PULLUP);
  pinMode( left_encoder_pin, INPUT_PULLUP);

  attachInterrupt( right_encoder_pin, right_counter, CHANGE);
  attachInterrupt(  left_encoder_pin,  left_counter, CHANGE);

  attach_servos(1);
  delay(1000);                    // time delay to situate robot
}

void loop() {
  enc_count = 0;
  drive(50, 50);
  Serial.print("enc_count: ");
  Serial.println(cc_left);
}


void attach_servos(int ats) {
  if ( ats == 0) {
    right_servo.detach();
     left_servo.detach();
  }
  else {
    right_servo.attach(right_servo_pin);
     left_servo.attach( left_servo_pin);
  }
}

void drive(double right_speed, double left_speed) {
  right_servo.writeMicroseconds(right_center_value - right_speed);
   left_servo.writeMicroseconds( left_center_value +  left_speed);
}

void right_counter() { cc_right++; }
void  left_counter() { cc_left++; enc_count = cc_left;}
// int count_left() {cc_left++; return left_count;}
