import controlP5.*;
import processing.serial.*;  // including the serial object libarary

ControlP5 cp5;
Serial mySerial;
PFont font;


/* GENERAL VARIABLES */
JSONObject parsedJson;
String jsonPackage = null;    // string that stores serialized incoming json
int nodeDisplayed;
int nodeUpdated;
float temp1 = 0.0;  // test variable
float temp2 = 0.0;  // test variable
float temp3 = 0.0;  // test variable
boolean sent;
boolean confirmUpdate;

int newLine = 10;        // 10 is binary for 'return' or 'new line'.


void setup() {
  /* General setup of the window */
  mySerial = new Serial(this, "COM4", 9600);
  size(1500, 800);  // siza deez app windows (x and y dimensions of application window)
  font = loadFont("SansSerif.plain-16.vlw");
  textFont(font);
  
  /* Creating array of data objects to record each node's data */
  data[] dataArray = new data[60];
  for (int i = 0; i < 60; i++) {
    dataArray[i] = new data();
  }
  
  controlP5Setup();
}

void draw() {

  while (mySerial.available() > 0) {
    jsonPackage = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
  }

  if (jsonPackage != null) {
    String newJsonString = jsonPackage.substring(0, jsonPackage.length() - 2);
    println(newJsonString);
    try {
      background(255, 255, 255);
      fill(0, 0, 0);
      parsedJson = parseJSONObject(newJsonString);
      println(parsedJson);
      nodeUpdated = parsedJson.getInt("NODE");
      sent = parsedJson.getBoolean("SENT");
      print("NODE: ");
      println(nodeUpdated);
      text(nodeDisplayed, 600, 150);
    } 
    catch (Exception e) {
      println("failed parsing JSON");
    }
    try {
      confirmUpdate = true;
      temp1 = parsedJson.getFloat("T1");
      temp2 = parsedJson.getFloat("T2");
      temp3 = parsedJson.getFloat("T3");
    } catch (Exception e) {
      confirmUpdate = false;
      println("failed updating data");
    }
    
    if (sent == true && nodeUpdated == nodeDisplayed) {
        background(255, 255, 255);
        fill(0, 0, 0);
        text("Temp 1: ", 400, 90); 
        text(temp1, 500, 90);
        text("Temp 2: ", 400, 120);
        text(temp2, 500, 120);
        text("Temp 3: ", 400, 150); 
        text(temp3, 500, 150);
        text(nodeDisplayed, 600, 150);
      }
    
    println("---------------------------------------------");
    
  }
}


void controlP5Setup() {
  cp5 = new ControlP5(this);
  PFont p = createFont("Verdana",16); 
  ControlFont controlFont = new ControlFont(p);
  cp5.setFont(controlFont);
  
  cp5.getTab("default")
     .activateEvent(true)
     .setLabel("Overview")
     .setId(1)
     .setHeight(40)
     .setWidth(100)
     ;
  cp5.addTab("Nodes")
    .setHeight(40)
    .setWidth(100)
    ;
  
  DropdownList droplist = cp5.addDropdownList("Nodes")
    .setPosition(0, 40).setSize(90, 700)
    .setBarHeight(40)
    .setItemHeight(50)
    .setBackgroundColor(color(213, 216, 220))
    .align(0, 100, 100, 0);
  
  for (int i = 0; i < 60; i++) {
    droplist.addItem("Node: " + i,i);
  }
  
  cp5.addButton("node1")    //"Node 1" is the name of the button
    .setPosition(100, 50)
    .setSize(100, 50);
  ;
  cp5.addButton("node2")    //"Node 1" is the name of the button
    .setPosition(100, 150)
    .setSize(100, 50)
    .setColorBackground(color(255, 0, 0));
  ;
  cp5.addButton("on")    //"Node 1" is the name of the button
    .setPosition(800, 50)
    .setSize(100, 50);
  ;
  cp5.addButton("shutdown")    //"Node 1" is the name of the button
    .setPosition(800, 150)
    .setSize(120, 50)
    .setColorBackground(color(255, 0, 0));
  ;
}

void on() {
  println("on");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + false + "}");
}

void shutdown() {
  println("off");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + true + "}");
}

void node1() {
  nodeDisplayed = 1;
  println("switched to node 1");
}

void node2() {
  nodeDisplayed = 2;
  println("switched to node 2");
}
