// IMPORTANT: this code works best with the MEGA as the transmitter and the UNO as the receiver
//https://howtomechatronics.com/projects/diy-arduino-rc-transmitter/

/* LIBRARIES */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

struct Data_Package {
  float temp1 = 12.7;
  float temp2 = 95.5;
  float temp3 = 112.2;
  float temp4 = 123.4;
  float temp5 = 115.3;
  float temp6 = 15.4;
  bool ok = true;
};

Data_Package data;

void setup() {
    Serial.begin(9600);

    radio.begin();
    radio.openWritingPipe(address);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}


void loop() {
    radio.write(&data, sizeof(data));
    delay(500);
}