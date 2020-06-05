/*
  Desk.cpp - Library for controlling motorized standing desk.
  Created by Simon Ng, May 29, 2020.
*/

#include "Desk.h"

/*==================== OBJECT SETUP ====================*/
Desk::Desk(byte PIN_servo1, byte PIN_servo2, byte PIN_servo3, byte PIN_servo4, byte PIN_joystick) { // should be 9, 5, 11, 3, 0
  _PIN_servo1 = PIN_servo1; // PWM pin
  _PIN_servo2 = PIN_servo2; // PWM pin
  _PIN_servo3 = PIN_servo3; // PWM pin
  _PIN_servo4 = PIN_servo4; // PWM pin
  _PIN_joystick = PIN_joystick; // analog pin
}

void Desk::initialize() { // to change which PWM pins the servos are attached to, change the following 4 lines
  servo1.attach(_PIN_servo1);
  servo2.attach(_PIN_servo2);
  servo3.attach(_PIN_servo3);
  servo4.attach(_PIN_servo4);

  cofigureAccelerometer(); // set up accelerometer
}

/*==================== SERVOS ====================*/
void Desk::moveArm(int servo, String dir, int distance, boolean useDistance) { // move an individual motor arm
  int dirValue = 90; // if input direction is incorrect, don't move CRS
  int forwardSpeed = 160; // for CRS (90 - 180)
  int reverseSpeed = 20; // for CRS (0 - 90)

  if (servo == 1) { // actual servo motor
    if (dir == "UP" || dir == "Up" || dir == "up") {
      dirValue = servo1.read() + distance / 20;
      Serial.println(servo1.read());
    }
    else if (dir == "DOWN" || dir == "Down" || dir == "down") {
      dirValue = servo1.read() - distance / 20;
      Serial.println(servo1.read());
    }
    else { // invalid entry, hold position
      dirValue = servo1.read(); // holds position
      Serial.println(servo1.read());
    }
    servo1.write(dirValue);
  }
  else if (servo == 2) { // continuous rotation servo
    if (dir == "UP" || dir == "Up" || dir == "up") {
      dirValue = forwardSpeed; // can be edited to adjust for motor inconsistencies
    }
    else if (dir == "DOWN" || dir == "Down" || dir == "down") {
      dirValue = reverseSpeed; // can be edited to adjust for motor inconsistencies
    }
    servo2.write(dirValue);
  }
  else if (servo == 3) { // continuous rotation servo
    if (dir == "UP" || dir == "Up" || dir == "up") {
      dirValue = reverseSpeed; // can be edited to adjust for motor inconsistencies
    }
    else if (dir == "DOWN" || dir == "Down" || dir == "down") {
      dirValue = forwardSpeed; // can be edited to adjust for motor inconsistencies
    }
    servo3.write(dirValue);
  }
  else if (servo == 4) { // continuous rotation servo
    if (dir == "UP" || dir == "Up" || dir == "up") {
      dirValue = forwardSpeed; // can be edited to adjust for motor inconsistencies
    }
    else if (dir == "DOWN" || dir == "Down" || dir == "down") {
      dirValue = reverseSpeed; // can be edited to adjust for motor inconsistencies
    }
    servo4.write(dirValue);
  }
  if (useDistance) { // can set moveArm to execute individual arm move or just turn motor on to move all at once for movePlatform
    delay(abs(distance));
  }
}

void Desk::movePlatform(String dir, int distance) { // move entire platform at once
  int t_spin = abs(distance);
  moveArm(1, dir, abs(distance), false);
  moveArm(2, dir, abs(distance), false);
  moveArm(3, dir, abs(distance), false);
  moveArm(4, dir, abs(distance), false);
  delay(t_spin);
  stopPlatform();
}

void Desk::movePlatformGradual(String dir, int velocity) { // doesn't work very well. based on moving each arm until accelerometer reads okay
  int cycles = round(velocity / 10);
  for (int i = 0; i < cycles ; i++) {
    moveArm(1, dir, 100, false); // needs tuning
    while (getAccelIn('X') > 0.1) {
      moveArm(2, dir, 0, false);
    }
    while (getAccelIn('Y') < -0.1) {
      moveArm(3, dir, 0, false);
      moveArm(4, dir, 0, false);
    }
  }
}

void Desk::stopPlatform() { // halt movement of all arms
  servo1.write(servo1.read()); // this holds its position
  servo2.write(90); // holds by torque
  servo3.write(90); // holds by torque
  servo4.write(90); // holds by torque
}

void Desk::levelPlatform() { // attempt to level platform based on accelerometer reading
  int tiltFactor = 600; // how large movements are to level platform
  float threshold = 0.07; // how flat the desk should be
  getAccel(_accel);
  float xAccel = _accel[2]; // Z according to accelerometer chip
  float yAccel = _accel[0]; // X according to accelerometer chip
  String mode = "down"; // should leveling mechanism be to raise low arms or to lower high arms?

  if (mode == "up") {
    if (xAccel > threshold) {
      moveArm(2, "up", abs(xAccel) * tiltFactor, true);
      moveArm(3, "up", abs(xAccel) * tiltFactor, true);
    }
    else if (xAccel < -threshold) {
      moveArm(1, "up", abs(xAccel) * tiltFactor, true);
      moveArm(4, "up", abs(xAccel) * tiltFactor, true);
    }
    if (yAccel > threshold) {
      moveArm(1, "up", abs(yAccel) * tiltFactor, true);
      moveArm(2, "up", abs(yAccel) * tiltFactor, true);
    }
    else if (yAccel < -threshold) {
      moveArm(3, "up", abs(yAccel) * tiltFactor, true);
      moveArm(4, "up", abs(yAccel) * tiltFactor, true);
    }
  }
  else if (mode == "down") {
    if (xAccel < -threshold) {
      moveArm(2, "down", abs(xAccel) * tiltFactor, true);
      moveArm(3, "down", abs(xAccel) * tiltFactor, true);
    }
    else if (xAccel > threshold) {
      moveArm(1, "down", abs(xAccel) * tiltFactor, true);
      moveArm(4, "down", abs(xAccel) * tiltFactor, true);
    }
    if (yAccel < -threshold) {
      moveArm(1, "down", abs(yAccel) * tiltFactor, true);
      moveArm(2, "down", abs(yAccel) * tiltFactor, true);
    }
    else if (yAccel > threshold) {
      moveArm(3, "down", abs(yAccel) * tiltFactor, true);
      moveArm(4, "down", abs(yAccel) * tiltFactor, true);
    }
  }
}

/*==================== ACCELEROMETER ====================*/
void Desk::cofigureAccelerometer() {
  Wire.begin(); // join i2c bus (address optional for master)

  // CTRL_REG1
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address
  Wire.write(byte(0b0100000)); // set pointer to go to control register 1
  Wire.write(byte(0b01110111)); // set to High Res / Normal / Low-power mode (400 Hz)
  Wire.endTransmission();     // stop transmitting

  // CTRL_REG4
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address
  Wire.write(byte(0b0100011)); // set pointer to go to control register 1
  Wire.write(byte(0b10001000)); // set to High Res / Normal / Low-power mode (400 Hz)
  Wire.endTransmission();     // stop transmitting
}

void Desk::getAccel(float * ptrToArray) { // passing pointer to a float
  byte XL;
  byte XH;
  byte YL;
  byte YH;
  byte ZL;
  byte ZH;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address
  Wire.write(0b10101000); // set to read desired address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 6);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    XL = Wire.read(); // this is being read
    XH = Wire.read(); // this is being read
    YL = Wire.read(); // this is being read
    YH = Wire.read(); // this is being read
    ZL = Wire.read(); // this is being read
    ZH = Wire.read(); // this is being read
  }
  *ptrToArray = ((XH << 8) + XL) / pow(2, 15) * 2.0;
  ptrToArray++;
  *ptrToArray = ((YH << 8) + YL) / pow(2, 15) * 2.0;
  ptrToArray++;
  *ptrToArray = ((ZH << 8) + ZL) / pow(2, 15) * 2.0;
}

void Desk::printAccel() { // should take in array
  getAccel(_accel); // passes pointer to beginning of array
  float X = _accel[2]; // Z according to accelerometer chip
  float Y = _accel[0]; // X according to accelerometer chip
  float Z = _accel[1]; // Y according to accelerometer chip

  Serial.print("X ");
  Serial.println(X, 6);
  Serial.print("Y ");
  Serial.println(Y, 6);
  Serial.print("Z ");
  Serial.println(Z, 6);
  Serial.println();
}

float Desk::getAccelIn(char accelDir) { // return just the direction of acceleration specified
  byte XL;
  byte XH;
  byte YL;
  byte YH;
  byte ZL;
  byte ZH;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
  Wire.write(0b10101000); // set to read desired address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 6);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    XL = Wire.read(); // this is being read
    XH = Wire.read(); // this is being read
    YL = Wire.read(); // this is being read
    YH = Wire.read(); // this is being read
    ZL = Wire.read(); // this is being read
    ZH = Wire.read(); // this is being read
  }
  if (accelDir == 'X' || accelDir == 'x')      return (((ZH << 8) + ZL) / pow(2, 15) * 2.0); // Z according to accelerometer chip
  else if (accelDir == 'Y' || accelDir == 'y') return (((XH << 8) + XL) / pow(2, 15) * 2.0); // X according to accelerometer chip
  else if (accelDir == 'Z' || accelDir == 'z') return (((YH << 8) + YL) / pow(2, 15) * 2.0); // Y according to accelerometer chip
  return 0;
}


/*==================== JOYSTICK ====================*/
int Desk::getDeltaHeight() {
  int joyOffset = 10; // controls how much joystick must move before activating platform move
  int joyPosNew = analogRead(_PIN_joystick) - 518;
  int joyPosTemp;
  if (abs(joyPosNew) > abs(_joyPosOld) + joyOffset) { // joystick has increased since last time
    joyPosTemp = _joyPosOld; // store old position before updating
    _joyPosOld = joyPosNew; // updated old position
    //Serial.print("Joystick pos: "); Serial.println(_joyPosOld);
    return (0.5 * (joyPosNew - joyPosTemp)); // amount to move, including correct sign
  }
  else if (abs(joyPosNew) < 30) { // joystick at center
    _joyPosOld = 0; // don't reset reference position until joystick is at center
  }
  return 0; // otherwise return no height change
}

int Desk::getContDeltaHeight() { // take current joystick position for move command
  int sensitivity = 5; // low number is non-responsive, high number is very responsive
  int joyPos = analogRead(_PIN_joystick) - 518;
  if (abs(joyPos) < 30) { // joystick at center
    return 0; // don't move anything
  }
  return joyPos / sensitivity;
}

/*==================== THERMISTOR ====================*/
void Desk::configureThermistor() {
  int t_limit = 30; // alert temperature
  
  // Activate Alarm
  Wire.beginTransmission(0b0011001);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0x01)); // set pointer to go to configuration register
  Wire.write(byte(0b00000000)); // leave high byte defaults
  Wire.write(byte(0b00001000)); // turn on Alert, leave rest of bits as defaults (nothing locked and in comparator mode)
  Wire.endTransmission();     // stop transmitting

  // Set resolution
  Wire.beginTransmission(0b0011001);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0b00001000)); // set pointer to go to temperature register
  Wire.write(byte(0b00000000)); // change resolution to +-0.5C
  Wire.endTransmission();     // stop transmitting

  // Set T_upper
  int intT_up = round(t_limit * 4);
  byte highT_up = intT_up >> 6;
  byte lowT_up = intT_up << 2;
  Wire.beginTransmission(0b0011001);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0b0000010)); // set pointer to go to upper temp limit register
  Wire.write(highT_up); // set high byte
  Wire.write(lowT_up); // set low byte
  Wire.endTransmission();     // stop transmitting

  // Set T_crit
  float t_crit = t_limit; //C . Max resolution is 0.25 deg
  int intT_crit = round(t_crit * 4);
  byte highT_crit = intT_crit >> 6;
  byte lowT_crit = intT_crit << 2;
  Wire.beginTransmission(0b0011001);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0b0000100)); // set pointer to go to critical temp limit register
  Wire.write(highT_crit); // set high byte
  Wire.write(lowT_crit); // set low byte
  Wire.endTransmission();     // stop transmitting
}

void Desk::printThermistorDiagnostics() { // print digital input and CONFIG register for reference and troubleshooting
  Serial.println();
  Serial.print("AlertPin: ");
  Serial.println(digitalRead(2));
  
  // Calculate Temp for troubleshooting/reference
  Wire.beginTransmission(0b0011001);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0x05));            // set pointer to read temp
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011001, 2); // request 2 bits from slave device
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    byte highByt = Wire.read(); // read first byte off buffer, high byte, largest number
    byte lowByt = Wire.read(); // read first byte off buffer, high byte, largest number
    boolean negative = bitRead(highByt, 4); // we want the sign of the number
    highByt = highByt & 0b00001111; // bitwise And
    int intTemperature = (highByt << 8) + lowByt;// int is two bytes. bitshift operator has order of operations
    float MCP9808Temp = intTemperature / 16.0;
    if (negative) MCP9808Temp = -1 * MCP9808Temp;
    Serial.print("Temperature: "); Serial.println(MCP9808Temp, 6);
  }
}

void Desk::printByte(byte myByte) {
  for (int i = 7; i > -1; i--) {
    Serial.print(bitRead(myByte, i));
  }
  Serial.println();
}

/*==================== MISC ====================*/
void Desk::Troubleshoot() { // likely problems I've forgotten about
  Serial.println("MOTORS NOT WORKING");
  Serial.println("Are pin wires contacting the stripped wire?");
  Serial.println("Are the power blocks plugged into the outlet?");
  Serial.println("Is the power bank turned on?");
}
