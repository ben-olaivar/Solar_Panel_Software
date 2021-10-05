  /* LIBRARIES */
#include <ArduinoJson.h>
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8);
const byte address[6] = "00001";

struct Data_Package {
  float temp1;
  float temp2;
  float temp3;
  float temp4;
  float temp5;
  float temp6;
  bool ok;
};

Data_Package data;

void setup() {
    Serial.begin(9600);

    radio.begin();
    radio.openReadingPipe(0, address);
    radio.setDataRate(RF24_250KBPS);
    radio.setPALevel(RF24_PA_MIN);
    radio.startListening();
}


void loop() {
  if (radio.available()) {

    radio.read(&data, sizeof(Data_Package));
    Serial.println(data.temp1);
    Serial.println(data.temp2);
    Serial.println(data.temp3);
    Serial.println(data.temp4);
    Serial.println(data.temp5);
    Serial.println(data.temp6);
    Serial.println(data.ok);
    Serial.println("------------------------");
  
  }
}