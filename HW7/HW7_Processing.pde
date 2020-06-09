import processing.serial.*;
float xangle = 0;
float yangle = 0;
float zangle = 0;
PShape myObject;
Serial myPort; // create object from serial class

void setup() {
  size(300, 300, P3D);
  myObject = loadShape("DalaHorse_try2.obj");
  myObject.setFill(color(255,20,80));
  myPort = new Serial(this, "COM8", 9600);
  myPort.bufferUntil('\n');
}

void draw() {
  background(204);
  translate(width/2, height/2, 0);
  println(xangle + " " + yangle + " " + zangle); // print accelerations to check accelerometer
  rotateX(yangle+3.0); // manually adjusting defualt angles to get horse upright
  rotateY(xangle+1.0); // manually adjusting defualt angles to get horse upright
  rotateZ(zangle-1.0); // manually adjusting defualt angles to get horse upright
  shape(myObject, 0, 0, 50, 100);
}

void serialEvent(Serial port) {
  String inputString = port.readStringUntil('\n');
  float[] nums = float(split(inputString, " "));
  float X = nums[0]; // this converts characters into floats
  float Y = nums[1]; // this converts characters into floats
  float Z = nums[2]; // this converts characters into floats
  xangle = X;
  yangle = Y;
  zangle = Z;
}

// arduino sends sring of characters 0-1023 with new line at end. we told port to buffer until we get a new line
