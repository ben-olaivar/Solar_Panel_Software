/* Libararies */
#include <Arduino.h>
#include <SPI.h>
#include <Arduino_JSON.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 8

/* TEMPERATURE SETUP */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress;

/* ETHERNET SETUP */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23
boolean alreadyConnected = false;


void setup() {
  Serial.begin(9600);
  tempSensorSetup();
  ethernetSetup();
}

void loop() {
  // wait for a new client:
  EthernetClient client = server.available();
  
  if (client) {
    if (!alreadyConnected) {
    client.flush();
    Serial.println("New client recognized");
    client.println("--- Type command and hit Enter/Return ---"); 
    alreadyConnected = true;
    }

    sensors.requestTemperatures(); // reads temps from all sensors

    // loop through each device & print out each temp
    for (int i = 0; i < numberOfDevices; i++) {

      if (sensors.getAddress(tempDeviceAddress, i)) {
        client.print("Temperature from Sensor number: " );
        client.println(i);
        
        //print temp
        float tempF = sensors.getTempF(tempDeviceAddress); //temp in degrees farenheit
        client.print("Farenheit: ");
        client.println(tempF);
      }
    }
    client.println("----------------------------------");
    delay(1000);
  }
}

/* SETUP TEMP SENSOR(s) */
void tempSensorSetup() {
  sensors.begin();
  
  numberOfDevices = sensors.getDeviceCount();
  Serial.print(numberOfDevices, DEC);
  Serial.println(" devices found");

  //print the address of each sensor
  for (int i = 0; i < numberOfDevices; i++) {

    //searches for data wires. Stores device addresses in "tempDeviceAddress" variable
    if (sensors.getAddress(tempDeviceAddress, i)) {

      Serial.print("Address ");
      Serial.print(i);
      printAddress(tempDeviceAddress);
      Serial.println();

    } else {
      Serial.print("Could not find address for device at ");
      Serial.println(i);
    }
  }
}


/* SETUP ETHERNET 
  By calling ethernet.begin(mac), it first tries to make a unique ip address. If that fails, it uses the fallback ip address (made above).
  begin(mac) keeps failing. fallback is always used for some reason. IT may give IP's
*/
void ethernetSetup() {
  Serial.println("Trying to get an IP address using DHCP");
  //! This way for sake of testing... delete next line and uncomment 'if' statement
  Ethernet.begin(mac, ip, gateway, subnet);
//  if (Ethernet.begin(mac) == 0) {
//    Serial.println("Failed to configure Ethernet using DHCP");
////    initialize the ethernet device not using DHCP:
//    Ethernet.begin(mac, ip, gateway, subnet);
//  }

  ip = Ethernet.localIP();
  //checking valid ip (not 0.0.0.0)
  if (ip[0] == 0 && ip[1] == 0 && ip[2] == 0 && ip[3] == 0) {
    Serial.print(Ethernet.localIP());
    Serial.println(" is not a valid IP Address. No connection made.");
    Serial.println("Stopping Program................");
    while (true) {}
  }
  // print your local IP address:
  Serial.print("My IP address: ");
  Serial.print(Ethernet.localIP());
  Serial.println();
  server.begin();
}

// function to print a sensor address
void printAddress(DeviceAddress deviceAddress) {
  
  for (uint8_t i = 0; i < 8; i++) {
    
    if (deviceAddress[i] < 16) 
      Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      
  }
  
}