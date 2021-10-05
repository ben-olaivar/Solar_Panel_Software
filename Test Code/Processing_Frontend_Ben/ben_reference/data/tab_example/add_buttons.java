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
  size(1600, 900);  // siza deez app windows (x and y dimensions of application window)
  font = loadFont("ArialMT-48.vlw");
  textFont(font);
  
  /* Creating array of data objects to record each node's data */
  data[] dataArray = new data[60];
  for (int i = 0; i < 60; i++) {
    dataArray[i] = new data();
  }
  
  controlP5Setup();
}

void draw() {
  background(232, 233, 234);
  stroke(color(19, 41, 75));    // The stroke is the OUTLINE of the shape/thing
  fill(color(19, 41, 75));        // the fill is the INSIDE COLOR of the shape/thing
  rect(0, 0, width, 75);
}


void controlP5Setup() {
  cp5 = new ControlP5(this);
  font = loadFont("ArialMT-19.vlw");
  ControlFont controlFont = new ControlFont(font);
  cp5.setFont(controlFont);
  
   cp5.addTab("default")
    .setLabel("Monitor")
    .setHeight(76)
    .setWidth(115)
    .setColorActive(color(232, 74, 39))
    .setColorBackground(color(19, 41, 75))
    .setColorForeground(color(30, 81, 150))
    ;
  cp5.getTab("Overview")
     .activateEvent(true)
     .setId(1)
     .setHeight(76)
     .setWidth(115)
     .setColorActive(color(232, 74, 39))
     .setColorBackground(color(19, 41, 75))
     .setColorForeground(color(30, 81, 150))
     ;
  cp5.addTab("Settings")
    .setHeight(76)
    .setWidth(115)
    .setColorActive(color(232, 74, 39))
    .setColorBackground(color(19, 41, 75))
    .setColorForeground(color(30, 81, 150))
    ;
  
  int xpos = 120;
  int ypos = 70;
  for (int i = 0; i < 60; i++) {
    
    if(i % 4 == 0) {
      xpos = 100;
      ypos += 48;
    }
    
    cp5.addButton("panel" + (i + 1))
      .setLabel("" + (i + 1))
      .setPosition(xpos, ypos)
      .setSize(75, 45);
    ;
    xpos += 77;
  }
  
  cp5.addButton("enable")    //"Node 1" is the name of the button
    .setPosition(800, 800)
    .setSize(120, 50)
    .setColorBackground(color(23, 207, 23))
    .setColorForeground(color(46, 184, 46))
  ;
  cp5.addButton("disable")    //"Node 1" is the name of the button
    .setPosition(1000, 800)
    .setSize(120, 50)
    .setColorBackground(color(230, 6, 6))
    .setColorForeground(color(195, 34, 34))
  ;
}

void enable() {
  println("on");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + false + "}");
}

void disable() {
  println("off");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + true + "}");
}

/* EVERYTHING BEYOND THIS POINT IS DEDICATED TO ALL 60 PANEL BUTTONS. I DON'T KNOW HOW TO DO THIS IN A BETTER WAY AT THE MOMENT */
void panel1() {
  nodeDisplayed = 1;
  println("switched to node " + nodeDisplayed);
}
void panel2() {
  nodeDisplayed = 2;
  println("switched to node " + nodeDisplayed);
}
void panel3() {
  nodeDisplayed = 3;
  println("switched to node " + nodeDisplayed);
}
void panel4() {
  nodeDisplayed = 4;
  println("switched to node " + nodeDisplayed);
}
void panel5() {
  nodeDisplayed = 5;
  println("switched to node " + nodeDisplayed);
}
void panel6() {
  nodeDisplayed = 6;
  println("switched to node " + nodeDisplayed);
}
void panel7() {
  nodeDisplayed = 7;
  println("switched to node " + nodeDisplayed);
}
void panel8() {
  nodeDisplayed = 8;
  println("switched to node " + nodeDisplayed);
}
void panel9() {
  nodeDisplayed = 9;
  println("switched to node " + nodeDisplayed);
}

// ---------------10---------------

void panel10() {
  nodeDisplayed = 10;
  println("switched to node " + nodeDisplayed);
}
void panel11() {
  nodeDisplayed = 11;
  println("switched to node " + nodeDisplayed);
}
void panel12() {
  nodeDisplayed = 12;
  println("switched to node " + nodeDisplayed);
}
void panel13() {
  nodeDisplayed = 13;
  println("switched to node " + nodeDisplayed);
}
void panel14() {
  nodeDisplayed = 14;
  println("switched to node " + nodeDisplayed);
}
void panel15() {
  nodeDisplayed = 15;
  println("switched to node " + nodeDisplayed);
}
void panel16() {
  nodeDisplayed = 16;
  println("switched to node " + nodeDisplayed);
}
void panel17() {
  nodeDisplayed = 17;
  println("switched to node " + nodeDisplayed);
}
void panel18() {
  nodeDisplayed = 18;
  println("switched to node " + nodeDisplayed);
}
void panel19() {
  nodeDisplayed = 19;
  println("switched to node " + nodeDisplayed);
}

// ---------------20---------------

void panel20() {
  nodeDisplayed = 20;
  println("switched to node " + nodeDisplayed);
}
void panel21() {
  nodeDisplayed = 21;
  println("switched to node " + nodeDisplayed);
}
void panel22() {
  nodeDisplayed = 22;
  println("switched to node " + nodeDisplayed);
}
void panel23() {
  nodeDisplayed = 23;
  println("switched to node " + nodeDisplayed);
}
void panel24() {
  nodeDisplayed = 24;
  println("switched to node " + nodeDisplayed);
}
void panel25() {
  nodeDisplayed = 25;
  println("switched to node " + nodeDisplayed);
}
void panel26() {
  nodeDisplayed = 26;
  println("switched to node " + nodeDisplayed);
}
void panel27() {
  nodeDisplayed = 27;
  println("switched to node " + nodeDisplayed);
}
void panel28() {
  nodeDisplayed = 28;
  println("switched to node " + nodeDisplayed);
}
void panel29() {
  nodeDisplayed = 29;
  println("switched to node " + nodeDisplayed);
}

// ---------------30---------------

void panel30() {
  nodeDisplayed = 30;
  println("switched to node " + nodeDisplayed);
}
void panel31() {
  nodeDisplayed = 31;
  println("switched to node " + nodeDisplayed);
}
void panel32() {
  nodeDisplayed = 32;
  println("switched to node " + nodeDisplayed);
}
void panel33() {
  nodeDisplayed = 33;
  println("switched to node " + nodeDisplayed);
}
void panel34() {
  nodeDisplayed = 34;
  println("switched to node " + nodeDisplayed);
}
void panel35() {
  nodeDisplayed = 35;
  println("switched to node " + nodeDisplayed);
}
void panel36() {
  nodeDisplayed = 36;
  println("switched to node " + nodeDisplayed);
}
void panel37() {
  nodeDisplayed = 37;
  println("switched to node " + nodeDisplayed);
}
void panel38() {
  nodeDisplayed = 38;
  println("switched to node " + nodeDisplayed);
}
void panel39() {
  nodeDisplayed = 39;
  println("switched to node " + nodeDisplayed);
}

// ---------------40---------------

void panel40() {
  nodeDisplayed = 40;
  println("switched to node " + nodeDisplayed);
}
void panel41() {
  nodeDisplayed = 41;
  println("switched to node " + nodeDisplayed);
}
void panel42() {
  nodeDisplayed = 42;
  println("switched to node " + nodeDisplayed);
}
void panel43() {
  nodeDisplayed = 43;
  println("switched to node " + nodeDisplayed);
}
void panel44() {
  nodeDisplayed = 44;
  println("switched to node " + nodeDisplayed);
}
void panel45() {
  nodeDisplayed = 45;
  println("switched to node " + nodeDisplayed);
}
void panel46() {
  nodeDisplayed = 46;
  println("switched to node " + nodeDisplayed);
}
void panel47() {
  nodeDisplayed = 47;
  println("switched to node " + nodeDisplayed);
}
void panel48() {
  nodeDisplayed = 48;
  println("switched to node " + nodeDisplayed);
}
void panel49() {
  nodeDisplayed = 49;
  println("switched to node " + nodeDisplayed);
}

// ---------------50---------------

void panel50() {
  nodeDisplayed = 50;
  println("switched to node " + nodeDisplayed);
}
void panel51() {
  nodeDisplayed = 51;
  println("switched to node " + nodeDisplayed);
}
void panel52() {
  nodeDisplayed = 52;
  println("switched to node " + nodeDisplayed);
}
void panel53() {
  nodeDisplayed = 53;
  println("switched to node " + nodeDisplayed);
}
void panel54() {
  nodeDisplayed = 54;
  println("switched to node " + nodeDisplayed);
}
void panel55() {
  nodeDisplayed = 55;
  println("switched to node " + nodeDisplayed);
}
void panel56() {
  nodeDisplayed = 56;
  println("switched to node " + nodeDisplayed);
}
void panel57() {
  nodeDisplayed = 57;
  println("switched to node " + nodeDisplayed);
}
void panel58() {
  nodeDisplayed = 58;
  println("switched to node " + nodeDisplayed);
}
void panel59() {
  nodeDisplayed = 59;
  println("switched to node " + nodeDisplayed);
}
void panel60() {
  nodeDisplayed = 60;
  println("switched to node " + nodeDisplayed);
}