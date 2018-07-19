// SimpleRx - the slave or the receiver

#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

#define CE_PIN   9
#define CSN_PIN 53

const byte thisSlaveAddress[5] = {'R','x','A','A','A'};

RF24 radio(CE_PIN, CSN_PIN);

int dataReceived[3]; // this must match dataToSend in the TX
int test_var, test_var2;    // declare example variables to store received values
bool newData = false;

//===========

void setup() {

    Serial.begin(9600);

    Serial.println("SimpleRx Starting");
    radio.begin();
    radio.setDataRate( RF24_250KBPS );
    radio.openReadingPipe(0, thisSlaveAddress);
    radio.startListening();

//    getData();
//    showData();
}

//=============

void loop() {
    getData();
    showData();
    test_var = dataReceived[1];
    test_var2 = dataReceived[0];
    Serial.print("dataReceived[1] is ");
    Serial.println(test_var);
    Serial.print("dataReceived[0] is ");
    Serial.println(test_var2);
}

//==============

void getData() {
    if ( radio.available() ) {
        radio.read( &dataReceived, sizeof(dataReceived) );
        newData = true;
    }
}

void showData() {
    if (newData == true) {
        Serial.print("Data received ");
        Serial.println(dataReceived[0]);
        Serial.println(dataReceived[1]);
        Serial.println(dataReceived[2]);
        newData = false;
    }
}
