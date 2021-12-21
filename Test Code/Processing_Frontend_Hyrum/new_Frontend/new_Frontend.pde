import processing.serial.*;  // including the serial object libarary
Button nodeOne;
Button nodeTwo;
String tempOne;
String tempTwo;
String tempThree;
String jsonMidParsed;
JSONObject jsonParsed;

/* SERIAL VARIABLES */
Serial mySerial;  // creating new serial object
String jsonPreParsed = null;    // string that stores serialized incoming json
int newLine = 10;        // 10 is binary for 'return' or 'new line'.
float temperature;  // test variable

/* BUTTON VARIABLES */

void setup() {
  size(1200, 600);  // siza deez app windows (x and y dimensions of application window)
  mySerial = new Serial(this, "COM3", 9600);
  // Button(int xposition, int yposition, int width, int height, String text, int red, int green, int blue)
  nodeOne = new Button(100, 100, 100, 50, "Node 1", 0, 200, 200);
  nodeTwo = new Button(100, 200, 100, 50, "Node 2", 0, 200, 200);
}

void draw() {
  nodeOne.update();
  nodeTwo.update();
  nodeOne.render();
  nodeTwo.render();
  text("Temp 1: ", 400, 90); 
  fill(0, 102, 153);
  text("Temp 2: ", 400, 120);
  fill(0, 102, 153);
  text("Temp 3: ", 400, 150); 

  // these functions cause buttons to highlight when clicked
  if (nodeOne.isClicked() == true) {
    nodeOne.Color = color(213, 216, 220);
    delay(50);
  } else {
    nodeOne.Color = color(0, 200, 200);
  }
  if (nodeTwo.isClicked() == true) {
    nodeTwo.Color = color(213, 216, 220);
    delay(50);
  } else {
    nodeTwo.Color = color(0, 200, 200);
  }
  
  while (mySerial.available() > 0) {
    jsonPreParsed = mySerial.readStringUntil(newLine);    // reads until it receives the 'new line' (10) signal
    if (jsonPreParsed != null) {
      println("preparsed: " + jsonPreParsed);
      jsonMidParsed = jsonPreParsed.substring(0, jsonPreParsed.length() - 2);
      println("midparsed: " + jsonMidParsed);
      jsonParsed = parseJSONObject(jsonPreParsed);
      println ("parsed: " + jsonParsed);
      tempOne = jsonParsed.T1;
      tempTwo = jsonParsed.T2;
      tempThree = jsonParsed.T3;
      println("tempOne: " + tempOne);
      println("tempTwo: " + tempTwo);
      println("tempThree: " + tempThree);
    }
    
    // String data = "{ \"id\": 0, \"species\": \"Panthera leo\", \"name\": \"Lion\"}";
    //try {
    //  json = parseJSONObject(data);
    //  String species = json.getString("species");
    //  println(species);
    //} catch (Exception e) {
    //  print("failed");
    //}
    //JSONObject json = parseJSONObject(jsonPackage);
    // print(jsonPackage);
    //if (jsonPackage != null) {
    //  String newData = jsonPackage.substring(0, jsonPackage.length() - 2);
    //  println(newData);
    //  try {
    //    json = parseJSONObject(newData);
    //    float NODE = json.getFloat("NODE");
    //    print("NODE: ");
    //    println(NODE);
    //      text(NODE, 600, 150);
    //  } catch (Exception e) {
    //    println("failed");
    //  }
    //}
  }
}
