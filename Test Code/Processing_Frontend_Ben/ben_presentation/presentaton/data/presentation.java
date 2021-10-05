/*************************************************************************
 *                      PROTECTION SYSTEM GUI                            *
 *      This is the Graphical User Interface (gui) for the Solar Panel   *
 *      Protection System. This file should be stored with assisted      *
 *      classes: (INSERT CLASSES HERE). This is heavily reliant on the   *
 *      controlP5 library for buttons and gui development.               *
 *      This code is reliant on a serial connection to the MAIN NODE.    *
 *                                                                       *
 *      Code should be ran in PROCESSING IDE                             *
 *                                                                       *
 *      Refer to (INSERT DOC HERE) for more info.                        *
 *      Author: Benjamin Olaivar                                         *
 *                                                                       *
 *        Last modified: 08/10/2021                                      *
 *                                                                       *
 *************************************************************************/

/* LIBRARIES */
import controlP5.*;
import processing.serial.*;  // including the serial object libarary

/* CONTANTS */
final int NUMNODES = 60;
final float defaultOverheatTemp = 10.0;            // Default Temperature (Farenheit) that triggers overheat status. Can revert to in 'Settings' tab
final float defaultVoltageThreshold = 0.0;         // Default Voltage (in Volts)   that triggers overVoltage status. Can revert to in 'Settings' tab
final float defaultCurrentThreshold = 0.0;         // Default Current (in Amps)    that triggers overCurrent status. Can revert to in 'Settings' tab

/* INITIALIZATION */
Serial mySerial;
PFont font;
ControlP5 cp5;
JSONObject parsedJson;
data[] dataArray = new data[NUMNODES];

/* COLORS */
color UOFI_BLUE = color(19, 41, 75);          // colors taken from official UIUC palette (https://www.uillinois.edu/OUR/brand/color_palettes)
color UOFI_BLUE_HOVER = color(30, 81, 150);
color UIC_RED = color(213, 0, 50);
color URBANA_ORANGE = color(232, 74, 39);
color COOL_GRAY_6 = color(166, 168, 171);

/* GENERAL VARIABLES */
int activeTab;           // the current 
int newLine = 10;        // 10 is binary for 'return' or 'new line'.

/* JSON VARIABLES */
String jsonPackage = null;    // string that stores serialized incoming json
boolean sent;

/* NODE/UPDATE VARIABLES */
int nodeDisplayed;
int nodeUpdated;
float overheatThreshold = defaultOverheatTemp;            // Modifiable Temperature (Farenheit) that triggers overheat status. Can be modified on 'Settings' tab
float overVoltageThreshold = defaultVoltageThreshold;     // Modifiable Voltage (in Volts)   that triggers overVoltage status. Can be modified on 'Settings' tab
float overCurrentThreshold = defaultCurrentThreshold;     // Modifiable Current (in Amps)    that triggers overCurrent status. Can be modified on 'Settings' tab

float temporaryOverheatThresh = overheatThreshold;        // Temporary values added from 'Settings' changes before confirming their changes
float temporaryOverVoltThresh = overVoltageThreshold;
float temporaryOverCurrThresh = overCurrentThreshold;


/************************************************************************************************* SETUP FUNCTIONS *************************************************************************************************/

/* Function: setup()
*   Initializes all buttons/tabs/serial connecections required
*/
void setup() {
  mySerial = new Serial(this, "COM4", 9600);
  
  /* General setup of the window */
  size(1600, 900);  // siza deez app windows (x and y dimensions of application window)
  font = loadFont("ArialMT-18.vlw");
  textFont(font);
  
  /* Creating array of data objects to record each node's data */
  for (int i = 0; i < NUMNODES; i++) {
    dataArray[i] = new data();
  }

  controlP5Setup();     // initiate all the buttons/tabs
}

/* Function: controlP5Setup()
*   initializes all the buttons and tabs displayed
*/
void controlP5Setup() {
  cp5 = new ControlP5(this);
  font = loadFont("ArialMT-19.vlw");
  ControlFont controlFont = new ControlFont(font);
  cp5.setFont(controlFont);
  
  /******************* cp5 OBJECTS FOR 'default' TAB *******************/

   cp5.addTab("default")      // The name of this tab is 'default'. You can change the display value with '.setLabel'
    .setLabel("Overview")     // The displayed text on the tab
    .setHeight(76)
    .setWidth(115)
    .setColorActive(URBANA_ORANGE)
    .setColorBackground(UOFI_BLUE)
    .setColorForeground(UOFI_BLUE_HOVER)
    .activateEvent(true)
    .setId(1)
    //.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
  cp5.addTab("Monitor")      // I didn't change the 'label' (displayed text) because I'm satisifed with what's on here
     .setHeight(76)
     .setWidth(115)
     .setColorActive(URBANA_ORANGE)
     .setColorBackground(UOFI_BLUE)
     .setColorForeground(UOFI_BLUE_HOVER)
     .activateEvent(true)
     .setId(2)
     //.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
     ;
  cp5.addTab("Settings")
    .setHeight(76)
    .setWidth(115)
    .setColorActive(URBANA_ORANGE)
    .setColorBackground(UOFI_BLUE)
    .setColorForeground(UOFI_BLUE_HOVER)
    .activateEvent(true)
    .setId(3)
    //.getCaptionLabel().align(ControlP5.CENTER, ControlP5.CENTER)
    ;
  
  /******************* cp5 OBJECTS FOR 'MONITOR' TAB *******************/
  int xpos = 120;
  int ypos = 120;                           //starting x and y point of buttons section
  int buttonsInRow = 5;                     // Number of columns per row
  for (int i = 0; i < NUMNODES; i++) {      // Loop that creates all 60 pannel buttons on the 'Monitor' with ID's 1-NUMNODES
    if(i % buttonsInRow == 0) {
      xpos = 100;
      ypos += 48;
    }
    
    cp5.addButton("panel" + (i + 1))                // linked to "controlEvent(ControlEvent theControlEvent)" function when clicked. See function description.
      .setLabel("" + (i + 1))
      .setPosition(xpos, ypos)
      .setSize(75, 45)
      .setId(i + 1)
      .moveTo("Monitor")
    ;
    xpos += 77;
  }

  cp5.addButton("enable")                           // linked to "enable()" function when clicked. See function description.
    .setPosition(800, 800)
    .setSize(120, 50)
    .setColorBackground(color(23, 207, 23))
    .setColorForeground(color(46, 184, 46))
    .setColorActive(color(0, 200, 0))
    .moveTo("Monitor")
  ;
  cp5.addButton("disable")                          // linked to "disable()" function when clicked. See function description.
    .setPosition(1000, 800)
    .setSize(120, 50)
    .setColorBackground(UIC_RED)
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
    .moveTo("Monitor")
  ;
  
  
  /******************* cp5 OBJECTS FOR 'Settings' TAB *******************/
  cp5.addTextfield("changeOverheatThresh")          // linked to "changeOverheatThresh(string inputValue)" function when you press 'enter'. See function description.
    .setLabel("Overheat Temperature Value (F)")
    .setPosition(1000, 500)
    .setSize(120, 50)
    .moveTo("Settings")
    .setColorCaptionLabel(color(0, 0, 0)) 
  ;
  cp5.addTextfield("changeOverVoltageThresh")       // linked to "changeOverVoltageThresh(string inputValue)" function when you press 'enter'. See function description.
    .setLabel("Over Voltage Value (V)")
    .setPosition(1000, 700)
    .setSize(120, 50)
    .moveTo("Settings")
    .setColorCaptionLabel(color(0, 0, 0)) 
  ;
  cp5.addTextfield("changeOverCurrentThresh")       // linked to "changeOverCurrentThresh(string inputValue)" function when you press 'enter'. See function description.
    .setLabel("Over Current Value (Amps)")
    .setPosition(1000, 300)
    .setSize(120, 50)
    .moveTo("Settings")
    .setColorCaptionLabel(color(0, 0, 0)) 
  ; 
  cp5.addButton("confirmChanges")                   // linked to "confirmChanges()" function when clicked. See function description.
    .setLabel("Confirm Changes")
    .setPosition(500, 800)
    .setSize(200, 50)
    .setColorBackground(UIC_RED)
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
    .hide()
    .moveTo("Settings")
  ;  
   cp5.addButton("defaultSettings")                 // linked to "defaultSettings()" function when clicked. See function description.
    .setLabel("Set to Defaults")
    .setPosition(1000, 800)
    .setSize(200, 50)
    .setColorBackground(UIC_RED)
    .setColorForeground(color(195, 34, 34))
    .setColorActive(color(220, 0, 0))
    .moveTo("Settings")
  ;
}


/************************************************************************************************* DRAW FUNCTIONS *************************************************************************************************/

/* Function: draw()
*   Acts as the 'loop' function from arduino IDE. creates all the shapes/text on the screen
*/
void draw() {
    background(232, 233, 234);
    stroke(UOFI_BLUE);    // The stroke is the OUTLINE of the shape/thing
    fill(UOFI_BLUE);      // the fill is the INSIDE COLOR of the shape/thing
    rect(0, 0, width, 75);        // Blue header rectangle

    updateData();

    // Will only display the temperatures on the default ('Monitor') tab
    if (activeTab == 2) {
        updateDisplayData();
    }
}


/* Function: controlEvent()
*   Handles all events from controlP5 items, such as buttons and tabs. Button Id's 1-NUMNODES are dedicated to the solar panel buttons
*/
void controlEvent(ControlEvent theControlEvent) {
  if (theControlEvent.isTab()) {                                        // Activated whenever a tab is clicked
    println("event from tab : "+theControlEvent.getTab().getName()+" with id "+theControlEvent.getTab().getId());
    activeTab = theControlEvent.getTab().getId();

  } else if (theControlEvent.isAssignableFrom(Textfield.class)) {       // Activated whenever a value is entered into a textfield (currently only on 'Settings' tab)    
    cp5.getController("confirmChanges").show();

  } else if (theControlEvent.getId() > 0 && theControlEvent.getId() <= NUMNODES) {      // Activated if not tab or textfield (only other objects are buttons w/ ID's 1-NUMNODES)
    nodeDisplayed = theControlEvent.getId();
    println("switched to node " + nodeDisplayed);

  }
}


/* Function updateDisplayData()
*   If on the 'Monitor' tab, this function displays the node data for the selected node
*/
void updateDisplayData() {
    if (nodeDisplayed > 0 && nodeDisplayed <= NUMNODES) {
        fill(UOFI_BLUE);
        stroke(UOFI_BLUE);
        rect(1400, height / 3, 125, height / 3);
        float[] temps = dataArray[nodeDisplayed - 1].getTempsFarenheit();

        fill(0, 0, 0);
        text("Temp 1: ", 700, 150); 
        text(temps[0], 800, 150);
        text("Temp 2: ", 700, 200);
        text(temps[1], 800, 200);
        text("Temp 3: ", 700, 250); 
        text(temps[2], 800, 250);
        text(nodeDisplayed, 900, 250);
    }
}


/* Function updateDisplayData()
*   Takes in the new data and saves it to the data array
*/
void updateData() {
    while (mySerial.available() > 0) {
        jsonPackage = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
    }

    if (jsonPackage != null) {
        String newJsonString = jsonPackage.substring(0, jsonPackage.length() - 2);

        try {
            parsedJson = parseJSONObject(newJsonString);
            nodeUpdated = parsedJson.getInt("NODE");
            sent = parsedJson.getBoolean("SENT");
        } 
        catch (Exception e) {
            println("Failed parsing JSON");
        }
        
        try {
            if (sent == true) {          // If the jsonPackage was parsed correnctly, this works
              dataArray[nodeUpdated - 1].setTemps(parsedJson.getFloat("T1"), parsedJson.getFloat("T2"), parsedJson.getFloat("T3"));
              // dataArray[nodeUpdated - 1].setVoltage(parsedJson.getFloat("V"));
              // dataArray[nodeUpdated - 1].setCurrent(parsedJson.getFloat("C"));
              
              float[] tempArray = {parsedJson.getFloat("T1"), parsedJson.getFloat("T2"), parsedJson.getFloat("T3")};
  
              boolean overHeat = tempArray[0] >= overheatThreshold || tempArray[1] >= overheatThreshold || tempArray[2] >= overheatThreshold;
              boolean overVolt = false;     //= parsedJson.getFloat("V") >= overVoltageThreshold;
              boolean overCurr = false;     //= parsedJson.getFloat("C") >= overCurrentThreshold;
              
              if (overHeat || overVolt || overCurr) {
                  cp5.getController("panel" + nodeUpdated).setColorBackground(UIC_RED);
              } else  {
                  cp5.getController("panel" + nodeUpdated).setColorBackground(UOFI_BLUE);
              }
              sent = false;
            } else {
                cp5.getController("panel" + nodeUpdated).setColorBackground(COOL_GRAY_6);
            }
        } catch (Exception e) {
            println("Failed updating Data");
        }
    
    }
}


/* FUNCTIONS FOR OBJECTS ON THE 'Monitor' TAB */
/* Function: enable()
*   Linked to 'enable' button under 'Monitor' tab. When button is clicked, this function
*   is automatically called. Sends out a JSON command telling which node to activate
*/
void enable() {
  println("on");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + false + "}");
}
/* Function: disable()
*   Linked to 'disable' button under 'Monitor' tab. When button is clicked, this function
*   is automatically called. Sends out a JSON command telling which node to deactivate
*/
void disable() {
  println("off");
  mySerial.write("{NODE:" + nodeDisplayed + ",SHUTDOWN:" + true + "}");
}


/* FUNCTIONS FOR OBJECTS ON THE 'Settings' TAB */

/* Function: defaultSettings()
*   Linked to 'defaultSettings' button under 'Settings' tab. When button is clicked, this function
*   is automatically called. Sets the overheat, overVoltage, and overCurrent thresholds to default values
*/
void defaultSettings() {
  overheatThreshold = defaultOverheatTemp;
  overVoltageThreshold = defaultVoltageThreshold;
  overCurrentThreshold = defaultCurrentThreshold;
  
  println("***reset settings to default***");
  println("overheat    threshold changed to " + overheatThreshold);
  println("overVoltage threshold changed to " + overVoltageThreshold);
  println("overCurrent threshold changed to " + overCurrentThreshold);
}

/* Function: changeOverheatThresh(String inputValue)
*   Linked to 'defaultSettings' textfield under 'Settings' tab. When the user hits 'enter', this function
*   is automatically called. Sets the overheat value to user's input (must be a valid int/float)
*/
void changeOverheatThresh(String inputValue) {
  inputValue = inputValue.trim();       // Cuts off any blank spaces in the inputValue
  
  if (inputValue.equals("") || inputValue == null) {                // intput should never be null. Added null check for good measure
    println("Empty or null entry. Please enter a valid value.");
  } else {
    try {
        temporaryOverheatThresh = Float.valueOf(inputValue);
        println("staged temporary Overheat val:  " + inputValue);
    } catch(Exception e) {
        println(e);
        println("Bad input: " + "\"" + inputValue + "\". " + "Please enter valid value.");
    }
  }
}

/* Function: changeOverheatThresh(String inputValue)
*   Linked to 'defaultSettings' textfield under 'Settings' tab. When the user hits 'enter', this function
*   is automatically called. Sets the overVoltage value to user's input (must be a valid int/float)
*/
void changeOverVoltageThresh(String inputValue) {
  inputValue = inputValue.trim();       // Cuts off any blank spaces in the inputValue
  
  if (inputValue.equals("") || inputValue == null) {                // intput should never be null. Added null check for good measure
    println("Empty or null entry. Please enter a valid value.");
  } else {
    try {
        temporaryOverVoltThresh = Float.valueOf(inputValue);
        println("staged temporary OverVoltage val:  " + inputValue);
    } catch(Exception e) {
        println(e);
        println("Bad input: " + "\"" + inputValue + "\". " + "Please enter valid value.");
    }
  }
}

/* Function: changeOverheatThresh(String inputValue)
*   Linked to the 'defaultSettings' textfield under the 'Settings' tab. When the user hits 'enter', this function
*   is automatically called. Sets the overCurrent value to user's input (valid int/float)
*/
void changeOverCurrentThresh(String inputValue) {
  inputValue = inputValue.trim();       // Cuts off any blank spaces in the inputValue
  
  if (inputValue.equals("") || inputValue == null) {                // intput should never be null. Added null check for good measure
    println("Empty or null entry. Please enter a valid value.");
  } else {
    try {
        temporaryOverCurrThresh = Float.valueOf(inputValue);
        println("staged temporary OverCurrent val:  " + inputValue);
    } catch(Exception e) {
        println(e);
        println("Bad input: " + "\"" + inputValue + "\". " + "Please enter valid value.");
    }
  }
  
}

/* Function: confirmChanges()
*   Linked to 'confirmChanges' button under 'Settings' tab. When clicked, this function is automatically called. Updates official thresholds, sends change command to serial, 
*   and hides the button until changes are made
*/
void confirmChanges() {
  overheatThreshold = temporaryOverheatThresh;
  overVoltageThreshold = temporaryOverVoltThresh;
  overCurrentThreshold = temporaryOverCurrThresh;
  println("overheat    threshold changed to " + overheatThreshold);
  println("overVoltage threshold changed to " + overVoltageThreshold);
  println("overCurrent threshold changed to " + overCurrentThreshold);
  cp5.getController("confirmChanges").hide();
  
//   mySerial.write("{OH:" + overheatThreshold + ",OV:" + overVoltageThreshold + ",OC:" + overCurrentThreshold + "}");
}