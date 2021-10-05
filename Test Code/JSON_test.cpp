#include <ArduinoJson.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 8
#define NUM_TEMP_SENSORS 6      //report calls for 6 temp sensors. If you use more/less, you only need to change it here

StaticJsonDocument<200> doc;  // 200 works for both esp8266 and arduino boards

/* TEMPERATURE SETUP */
//TODO: find realistic values for these temperatures
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];
int TEMP_THRESH = 90;   //! not making constant (#define). When in overheat, thresh is lower number. Allows for autonomous restart. Consult Kevin about this. number is arbitrary rn
bool overheat = false;
int tempThresholdsBroken; /* Reads how many temp sensors have detected an exceeded temp thresh (don't want a faulty sensor setting it off) 
- Lab report displays 6 temp sensors. Shut down at 3 or 4 */

void setup() {
  // Initialize Serial port
  Serial.begin(9600);
  tempSensorSetup();
}

void loop() {
  readTemperatures();

  doc["T1"] = tempArray[0];
  doc["T2"] = tempArray[1];
  doc["T3"] = tempArray[2];
  doc["T4"] = tempArray[3];
  doc["T5"] = tempArray[4];
  doc["T6"] = tempArray[5];

  float temp1 = doc["T1"];
  float temp2 = doc["T2"];
  float temp3 = doc["T3"];
  float temp4 = doc["T4"];
  float temp5 = doc["T5"];
  float temp6 = doc["T6"];

  Serial.println(temp1);
  Serial.println(temp2);
  Serial.println(temp3);
  Serial.println(temp4);
  Serial.println(temp5);
  Serial.println(temp6);

  Serial.println("---------------------------------------");

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
}