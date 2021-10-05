// reference: https://www.youtube.com/watch?v=lSVOtoMqtrI&ab_channel=DPVTECHNOLOGY

//LIBARARIES
#include <OneWire.h>
#include <DallasTemperature.h>

/* Constants */
#define ONE_WIRE_BUS 8

/* TEMPERATURE variables */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
int numberOfDevices;
DeviceAddress tempDeviceAddress;


void setup() {
  Serial.begin(9600);
  sensorSetup();
}


void loop() {
  sensors.requestTemperatures(); // reads temps from all sensors

  // loop through each device & print out each temp
  for (int i = 0; i < numberOfDevices; i++) {

    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Temperature from Sensor number: " );
      Serial.println(i);
      
      //print temp
      float tempF = sensors.getTempF(tempDeviceAddress); //temp in degrees farenheit
      Serial.print("Farenheit: ");
      Serial.println(tempF);
    }
  }
  delay(1000);
}

/* TEMPERATURE SENSOR SETUP */
void sensorSetup() {
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

// function to print a sensor address
void printAddress(DeviceAddress deviceAddress) {
  
  for (uint8_t i = 0; i < 8; i++) {
    
    if (deviceAddress[i] < 16) 
      Serial.print("0");
      Serial.print(deviceAddress[i], HEX);
      
  }
  
}