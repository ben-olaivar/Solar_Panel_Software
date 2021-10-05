https://www.youtube.com/watch?v=qxEclOy6jpI&ab_channel=Cooltronics

/* Libararies */
#include <Arduino.h>
#include <SPI.h>
#include <Arduino_JSON.h>
#include <Ethernet.h>
#include <OneWire.h>
#include <DallasTemperature.h>
#include <ArduinoJson.h>


/* CONSTANTS */
#define ONE_WIRE_BUS 8
#define NUM_TEMP_SENSORS 6      //report calls for 6 temp sensors. If you use more/less, you only need to change it here

/* ETHERNET SETUP */
byte mac[] = {0x00, 0xAA, 0xBB, 0xCC, 0xDE, 0x04};
IPAddress ip(128, 174, 186, 99);
IPAddress gateway(128, 174, 186, 1);
IPAddress subnet(255, 255, 254, 0);
EthernetServer server(23); // default is 23

/* JSON INSTANTIATION */
StaticJsonDocument<200> jsonObject;  // 200 works for both esp8266 and arduino boards
int T1 = jsonObject["T1"];
int T2 = jsonObject["T2"];
int T3 = jsonObject["T3"];
int T4 = jsonObject["T4"];
int T5 = jsonObject["T5"];
int T6 = jsonObject["T6"];

int V1 = jsonObject["V1"];
int V2 = jsonObject["V2"];
int V3 = jsonObject["V3"];

int C1 = jsonObject["C1"];

bool OVERHEAT = jsonObject["OVERHEAT"];
bool OVERVOLT = jsonObject["OVERVOLT"];
bool OVERCURR = jsonObject["OVERCURRENT"];

/* TEMPERATURE SETUP */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];
int TEMP_THRESH = 90;   //! not making constant (#define). When in overheat, thresh is lower number. Allows for autonomous restart. Consult Kevin about this. number is arbitrary rn
bool overheat = false;
int tempThresholdsBroken; /* Reads how many temp sensors have detected an exceeded temp thresh (don't want a faulty sensor setting it off) 
- Lab report displays 6 temp sensors. Shut down at 3 or 4 */


void setup() {
  Serial.begin(9600);
  tempSensorSetup();
  ethernetSetup();
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


/* SETUP TEMP SENSOR(s) */
void tempSensorSetup() {
  sensors.begin();

  //print the address of each sensor
  for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
    //searches for data wires. Stores device addresses in "tempDeviceAddress" variable
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Address ");
      Serial.print(i);
      Serial.print(": ");
      printAddress(tempDeviceAddress);
      Serial.println();

    } else {
      Serial.print("Could not find address for device at ");
      Serial.println(i);
    }
  }
}


/* PRINT THE HEX ADDRESS FOR TEMPERATURE SENSORS */
void printAddress(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16) 
      Serial.print("0");
      Serial.print(deviceAddress[i], HEX);   
    }
}



//TODO: convert loop and other functinos to use/export JSON. Also listen for SHUTDOWN command
void loop() {
  // wait for a new client:
  EthernetClient client = server.available();

  if (client) { //makes sure client is connected
    readTemperatures();

    //TODO: Reomove ALL print statements (or comment out) when code is finalized. Only need to send JSON 
    if (overheat) { //calls cooldown function if more than 3 sensors detect an overheat
        client.println();
        client.println("************SYSTEM OVERHEAT************");
        client.println("3 or more temp sensors have exceeded the temperature threshold...waiting for cooldown");
        client.println("***************************************");
        //TODO: add some function to shut things down when in overheat
    }
    for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
        if (tempArray[i] > TEMP_THRESH) {
            client.print("TEMP THRESH EXCEEDED AT SENSOR: ");
        } else {
            client.print("Temperature at sensor: ");
        }
        client.println(i);
        client.print("Farenheit: ");
        client.println(tempArray[i]);
    }

    client.println("-----------------------");
    //delay(1000);    //! Possibly use delay later. Code works with or without, just slower with delay
  }
}


/* READS TEMPERATURE */
void readTemperatures() {
    //TODO: Convert to JSON (possibly continue as is, just write array values into JSON in loop)
    sensors.requestTemperatures(); // reads temps from all sensors
    tempThresholdsBroken = 0;   //! Resets to 0 to allow for overheat setting to change. May modify later
        
    // loop through each device & assign temperature value to "tempArray" array
    for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
        if (sensors.getAddress(tempDeviceAddress, i)) {
            tempArray[i] = sensors.getTempF(tempDeviceAddress);  // reads temperature in degrees farenheit
            if (tempArray[i] >= TEMP_THRESH) {
                tempThresholdsBroken += 1;
            }
        }
    }
    if (tempThresholdsBroken >= 3) {    /* After initial overheat, other sensors may trigger as overheating due to TEMP_THRESH change. This is expected */
        overheat = true;
        TEMP_THRESH = 85;
    } else {     // does not wait for entire system cooldown. Again, going back to 1 or 2 faulty sensors ruining everything... this was an issue found in testing
        //TODO: find a way to get around faulty temperature sensors. Testing found that resetting the system worked...Maybe re-run tempSensorSetup occasionally
        overheat = false;
        TEMP_THRESH = 90;
    }

    jsonObject["T1"] = tempArray[0];
    jsonObject["T2"] = tempArray[1];
    jsonObject["T3"] = tempArray[2];
    jsonObject["T4"] = tempArray[3];
    jsonObject["T5"] = tempArray[4];
    jsonObject["T6"] = tempArray[5];
    jsonObject["OVERHEAT"] = overheat;
}