import controlP5.*;
import processing.serial.*;  // including the serial object libarary
ControlP5 cp5;
Serial mySerial;
//Ccolor t = ;

void setup() {
  size(1200, 600);  // siza deez app windows (x and y dimensions of application window)
  mySerial = new Serial(this, "COM6", 9600);
  
  fill(0, 0, 0);
  cp5 = new ControlP5(this);
  cp5.addButton("on")    //"Node 1" is the name of the button
    .setPosition(100, 50)
    .setSize(100, 50);
  ;
  cp5.addButton("off")    //"Node 1" is the name of the button
    .setPosition(100, 150)
    .setSize(100, 50)
    .setColorBackground(color(255, 0, 0));
  ;
  
}

void draw() {
  background(255, 255, 255);
  
  
}


void on() {
  println("on");
  mySerial.write('o');
}

void off() {
  println("off");
  mySerial.write('f');
}
