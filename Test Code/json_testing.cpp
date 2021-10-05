/* LIBRARIES */
#include <SPI.h>
#include <nRF24L01.h>
#include <RF24.h>

RF24 radio(7, 8); // CE, CSN
const byte address[6] = "00001";

StaticJsonDocument<200> root;

int T1 = root["T1"];
int T2 = root["T2"];
int T3 = root["T3"];
int T4 = root["T4"];
int T5 = root["T5"];
int T6 = root["T6"];

int V1 = root["V1"];
int V2 = root["V2"];
int V3 = root["V3"];

int C1 = root["C1"];

void setup() {
    Serial.begin(9600);
    root["T1"] = 1;
    root["T2"] = 2;
    root["T3"] = 3;
    root["T4"] = 4;
    root["T5"] = 5;
    root["T6"] = 6;

    radio.begin();
    radio.openWritingPipe(address);
    radio.setPALevel(RF24_PA_MIN);
    radio.stopListening();
}


void loop() {
    Serial.println(root["T1"]);
}