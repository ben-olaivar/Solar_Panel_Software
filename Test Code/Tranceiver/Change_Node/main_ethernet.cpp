/*************************************************************************
 * Remote node - nRF24L01+ radio communications                          *
 *      Program acts as master and cycles through each node to           *
 *      send a data request command. This code runs regardless           *
 *      of connection through ethernet to ensure panel safety.           *
 *      
 *      Refer to (INSERT DOC HERE) for more info.                         *
 *      Author: Benjamin Olaivar                                         *
 *                                                                       *
 *        Last modified: 07/22/2021                                      *
 *                                                                       *
 *************************************************************************/

//https://medium.com/@benjamindavidfraser/arduino-nrf24l01-communications-947e1acb33fb

/* LIBRARIES */
#include <RF24.h> 
#include <SPI.h> 
#include <nRF24L01.h>
#include <ArduinoJson.h>

/* CONSTANTS */
#define CE_PIN 7        // set Chip-Enable (CE) and Chip-Select-Not (CSN) pins (for UNO)
#define CSN_PIN 8
#define NUM_NODES 3
#define CAPACITY 200    // the number of bytes required for our JSON (we use roughly 200)

/* GENERAL VARIABLES */
int node = 1;
bool shutdownArray[60];

float oh = 70.0;
float ov = 0.0;
float oc = 0.0;

/* JSON VARIABLES */
bool sent = true;          // indicates if a message has been sent

/* RADIO VARIABLES */
RF24 radio(CE_PIN,CSN_PIN);     // create RF24 radio object using selected CE and CSN pins

// each node has its own node address ranging from 1-NUM_NODES. node address must be manually set on the node's end
byte nodeAddress[5] = {'N','O','D', 0, 1};      // setup radio pipe address for remote sensor node

struct Command_Package {
    bool shutdown = false;
    float ohthresh;
    float ovthresh;
    float octhresh;
};
struct Data_Package {
  float V = 0.0;
  float C = 0.0;

  float T1 = 0.0;
  float T2 = 0.0;
  float T3 = 0.0;
};
Command_Package commandToNode;    //TODO: add commands
Data_Package dataFromNode;


/* Function: setup
*   calls various setup functions
*/
void setup() {
  Serial.begin(9600);
  while(!Serial) {}
  pinMode(3, OUTPUT);
  radioSetup();
}

/* Function: radioSetup
*   sets up transceiver settings. Allows for 5 retries before moving on to the next node.
*   NOTE: the enableAckPayload is crucial for this to work. It automatically switches between reading and listening modes.
*   The Acknowledgement payload is the response from each node filled with data.
*/
void radioSetup() {
    radio.begin();// begin radio object
  
    radio.setPALevel(RF24_PA_LOW);      // set power level of the radio
    radio.setDataRate(RF24_250KBPS);    // set RF datarate - lowest rate for longest range capability
   
    radio.setChannel(0x66);     // set radio channel to use - ensure all slaves match this
    radio.setRetries(5, 50);    // set time between retries and max no. of retries (time )
    
    radio.enableAckPayload();               // enable ackpayload - enables each slave to reply with data 
    radio.openWritingPipe(nodeAddress);     // setup write pipe to remote node - must match node listen address
}


/* Function: loop
 *    main loop program for the slave node - opens a new reading pipe when switching to a new node
 */
void loop() {
    if (Serial.available()) {
//    {"NODE":2,"SHUTDOWN":false}
      String val = Serial.readString();
      StaticJsonDocument<200> jsonFromFrontend;
      deserializeJson(jsonFromFrontend, val);
      if (jsonFromFrontend["COMMAND"] == 1) {
            changeNode(jsonFromFrontend["NODE"]);
            shutdownArray[node] = jsonFromFrontend["SHUTDOWN"];

      } else if (jsonFromFrontend["COMMAND"] == 2) {
          oh = jsonFromFrontend["ohThresh"];
          ov = jsonFromFrontend["ovThresh"];
          oc = jsonFromFrontend["ocThresh"];
      }
    }


    /*  Creating JSON doc to store data from nodes. 
        successful data retreival:       {"NODE":#,"SENT":true,"T1":#.##,"T2":#.##, ...}
        unsuccessful data retreival:    {"NODE":#,"SENT":false}*/
    StaticJsonDocument<200> sendJson;
    sendJson["NODE"] = node;
    // Serial.print("Node ");   //debugging prints
    // Serial.print(node);
    // Serial.println(":");
    radio.openWritingPipe(nodeAddress);     // switches to the current node address to request data
    readRadio();
    sendJson["SENT"] = sent;
    if (sent) {                     //ONLY UPDATE DATA ON SUCCESSFUL DATA RETREIVAL
        sendJson["T1"] = dataFromNode.T1;
        sendJson["T2"] = dataFromNode.T2;
        sendJson["T3"] = dataFromNode.T3;
        //TODO: add other variables, such as voltage, current, and others
    }

    String sendValue;                       // Serialize JSON Object to a string, which can be printed
    serializeJson(sendJson, sendValue);     // would use serializeJson(sendJson, Serial), but this allows us to modify later if we want
    Serial.println(sendValue);              // ALSO processing identifies when message is over by waiting for the '\n' (new line) note 

    changeNode();           // go to next node

    delay(250);
}


/* Function: readRadio
*   Communicates to whichever node is currently being read from, and collects data through an acknowledgement response.
*   the acknowledgement response automatically switches between listening and writing modes, which proved better than manually doing so
*/
void readRadio() {
    // Sends the shutdown command for that node, which is stored in a boolean array
    commandToNode.shutdown = shutdownArray[node];
    commandToNode.ohthresh = oh;
    commandToNode.ovthresh = ov;
    commandToNode.octhresh = oc;
    // boolean to indicate if radio.write() tx was successful
    bool tx_sent;
    tx_sent = radio.write(&commandToNode, sizeof(commandToNode));

    // if tx success - receive and read smart-post ack reply
    if (tx_sent) {
        if (radio.isAckPayloadAvailable()) {
            // read acknowledgement payload and copy message to remoteNodeData 
            radio.read(&dataFromNode, sizeof(dataFromNode));
            // sendJson["T1"] = dataFromNode.T1;
            // sendJson["T2"] = dataFromNode.T2;
            // sendJson["T3"] = dataFromNode.T3;

            // Serial.print("[+] Successfully received data from node.");
            // Serial.print("  ---- Temp was: ");
            // Serial.println(data.T1);
            // if (dataFromNode.T1 > 80) {
            //     commandToNode.shutdown = true;
            // } else {
            //     commandToNode.shutdown = false;
            // }
        }
        sent = true;
    } 
    else {
        // Serial.println("[-] The transmission to the node failed.  ***********************************");

        //TODO: update json accordingly. May not want to change temperature variables though
        sent = false;
    }
    // Serial.println("------------------------------------------");
}


/* Function: changeNode()
*   Updates the nodeAddress to the next node by changing the last 2 variables of nodeAddress. (recall nodeAddress[5] = {'N','O','D', 0, 1};)
*   KEEP THE LAST 2 VARIABLES OF nodeAddress AS INTS FOR THIS FUNCTION TO WORK 
*   To change max # of nodes, just change the NUM_NODES variable
*/
void changeNode() {
    node = node + 1;
    if (node > NUM_NODES) {
        node = 1;
    }
    nodeAddress[3] = node / 10;
    nodeAddress[4] = node % 10;
    // sendJson["NODE"] = node;
}
/* Function: changeNode(int n)
*   Updates the node and nodeAddress to specified node. Doesn't allow for nonexistent nodes
*/
void changeNode(int n) {
  if (n > NUM_NODES || n < 0) {
    Serial.println("Node does not exist");
  } else {
    node = n;
    nodeAddress[3] = n / 10;
    nodeAddress[4] = n % 10;
  }
}