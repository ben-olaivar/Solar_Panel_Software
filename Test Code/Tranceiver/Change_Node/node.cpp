/*************************************************************************
   Remote node - nRF24L01+ radio communications
        A program to operate a remote-node slave device that sends
        data to a command unit on a given request message. The radio
        transceiver used is the nRF24L01+, and it operates using the
        TMRh20 RF24 library.
 *                                                                       *
        Author: B.D. Fraser
 *                                                                       *
          Last modified: 27/06/2017
 *                                                                       *
 *************************************************************************/

/* LIBRARIES */
#include <SPI.h>
#include <RF24.h>
#include <nRF24L01.h>
#include <printf.h>
#include <OneWire.h>
#include <DallasTemperature.h>

/* CONSTANTS */
#define ONE_WIRE_BUS 9
#define NUM_TEMP_SENSORS 3
#define CE_PIN 7
#define CSN_PIN 8
#define LEDPIN 3

/* GENERAL VARIABLES */
int TEMP_THRESH = 90;       // When in overheat, thresh is lower number. Allows for autonomous restart. Val is arbitrary
int tempThresholdsBroken;   // Reads how many temp sensors have detected an exceeded temp thresh
bool overheat = false;

/* RADIO VARIABLES */
RF24 radio(CE_PIN, CSN_PIN);
const byte nodeAddress[5] = {'N', 'O', 'D', 0, 2}; // setup radio pipe addresses for each sensor node
int remoteNodeData[2] = {1, 1,};                    // simple integer array for remote node data, in the form [node_id, returned_count]

/* TEMP SENSOR VARIABLES */
OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature sensors(&oneWire);
DeviceAddress tempDeviceAddress;
float tempArray[NUM_TEMP_SENSORS];

struct Data_From_Master {
  bool shutdown;
};
struct Data_Package {
  bool overheat = false;
  bool overVolt = false;
  bool overCurr = false;

  float V1 = 0.0;
  float V2 = 0.0;
  float V3 = 0.0;

  float C = 0.0;

  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
};
Data_Package data;
Data_From_Master dataFromMaster;



/* Function: setup
      Initialises the system wide configuration and settings prior to start
*/
void setup() {
  Serial.begin(9600);
  while (!Serial) {}

  pinMode(LEDPIN, OUTPUT);

  radioSetup();
  tempSensorSetup();
}


/* Function: radioSetup()
  setup function for NRF24L01 transceiver. This version sets up an acknowledgment payload (AckPayload) so we don't
  have to manually switch between listening/writing modes. Also prints transceiver data (may romove)
*/
void radioSetup() {
  radio.begin();

  radio.setPALevel(RF24_PA_LOW);    // set power level of the radio
  radio.setDataRate(RF24_250KBPS);  // set RF datarate for max range

  radio.setChannel(0x66);                 // set radio channel to use - ensure it matches the target host
  radio.openReadingPipe(1, nodeAddress);  // open a reading pipe on the chosen address - matches the master tx

  radio.enableAckPayload();                                           // enable ack payload - slave replies with data using this feature
  //TODO: check if below line does anything
  radio.writeAckPayload(1, &remoteNodeData, sizeof(remoteNodeData));  // preload the payload with initial data - sent after an incoming message is read

  // print radio config details to console
  printf_begin();
  radio.printDetails();

  radio.startListening();         // start listening on radio
}


/* Function: tempSensorSetup
  initializes the temperature sensors (quantitiy determined by NUM_TEMP_SENSORS) and calls 'printTempSensorAddresses'
*/
void tempSensorSetup() {
  sensors.begin();

  //print the address of each sensor
  for (int i = 0; i < NUM_TEMP_SENSORS; i++) {
    //searches for data wires. Stores device addresses in "tempDeviceAddress" variable
    if (sensors.getAddress(tempDeviceAddress, i)) {
      Serial.print("Address ");
      Serial.print(i);
      Serial.print(": ");
      printTempSensorAddresses(tempDeviceAddress);
      Serial.println();

    } else {
      Serial.print("Could not find address for device at ");
      Serial.println(i);
    }
  }
}

/* Function: printTempSensorAddresses
  Prints the addresses of each detected temperature device for debugging purposes
*/
void printTempSensorAddresses(DeviceAddress deviceAddress) {
  for (uint8_t i = 0; i < 8; i++) {
    if (deviceAddress[i] < 16)
      Serial.print("0");
    Serial.print(deviceAddress[i], HEX);
  }
}


/* Function: loop
      main loop program for the slave node - repeats continuously during system operation
*/
void loop() {

  radioCheckAndReply();   // transmit current preloaded data to master device if message request received
}


/* Function: updateNodeData
      updates the temperature and voltage (later) variables for the node and stores it in the nRF24L01+ radio
      preloaded ack payload ready for sending on next received message
*/
void updateNodeData(void) {
  readTemperatures();

  // set the ack payload ready for next request from the master device
  radio.writeAckPayload(1, &data, sizeof(Data_Package));
}


/* Function: readTemperatures
  Cycles through each temp sensor and saves the readings to the 'data' struct. Also takes note of
  an overheat (currently waits for 2 sensors to overheat. This number is arbitrary)
  - may make separate function for overheat readings (for simplification)
*/
void readTemperatures() {
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
  if (tempThresholdsBroken >= 2) {    /* After initial overheat, other sensors may trigger as overheating due to TEMP_THRESH change. This is expected */
    overheat = true;
    TEMP_THRESH = 85;
  } else {     // does not wait for entire system cooldown. Again, going back to 1 or 2 faulty sensors ruining everything... this was an issue found in testing
    //TODO: find a way to get around faulty temperature sensors. Testing found that resetting the system worked...Maybe re-run tempSensorSetup occasionally
    overheat = false;
    TEMP_THRESH = 90;
  }

  data.T1 = tempArray[0];   // updating readings in our data struct
  data.T2 = tempArray[1];
  data.T3 = tempArray[2];
  // data.overTemp = overheat;
}


/* Function: radioCheckAndReply
  sends the preloaded node data over the nrf24l01 radio when
  a message is received by the master
*/
void radioCheckAndReply(void) {
  // check for radio message and send sensor data using auto-ack
  if ( radio.available() ) {
    radio.read( &dataFromMaster, sizeof(dataFromMaster) );
    if (dataFromMaster.shutdown == true) {
      Serial.println("SHUTDOWN SHUTDOWN SHUTDOWN");
      digitalWrite(LEDPIN, LOW);
    } else {
      digitalWrite(LEDPIN, HIGH);
    }
    Serial.println("Received request from master - sending preloaded data.");

    // update the node count after sending ack payload - provides continually changing data
    updateNodeData();
  }
}