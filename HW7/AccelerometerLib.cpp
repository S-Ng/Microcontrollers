# include<Arduino.h>
#include<Wire.h>

void printByte(byte myByte) { // function to print whole bytes at once
  for (int i = 7; i > -1; i--) {
    Serial.print(bitRead(myByte, i));
  }
  Serial.println();
}

void cofigureAccelerometer() {
  Wire.begin(); // join i2c bus (address optional for master)

  // CTRL_REG1
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address with SD0 to GND
  Wire.write(byte(0b0100000)); // set pointer to go to control register 1
  Wire.write(byte(0b01110111)); // set to High Res / Normal / Low-power mode (400 Hz)
  Wire.endTransmission();     // stop transmitting

  // CTRL_REG4
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address with SD0 to GND
  Wire.write(byte(0b0100011)); // set pointer to go to control register 1
  Wire.write(byte(0b10001000)); // set to High Res / Normal / Low-power mode (400 Hz)
  Wire.endTransmission();     // stop transmitting
}

byte readByte(byte address) { // function to read 1 byte at a time from this device
  byte outputByte;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
  Wire.write(address); // set to read desired address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 1);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    outputByte = Wire.read(); // this is being read
  }
  return outputByte;
}

float readX() {
  byte XL;
  byte XH;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
  Wire.write(0b10101000); // set to read desired address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 2);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    XL = Wire.read(); // this is being read
    XH = Wire.read(); // this is being read
  }
  float X = ((XH << 8) + XL) / pow(2, 15) * 2.0;
  return X;
}

float readY() {
  byte YL;
  byte YH;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
  Wire.write(0b10101010); // set to read desired address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 2);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    YL = Wire.read(); // this is being read
    YH = Wire.read(); // this is being read
  }
  float Y = ((YH << 8) + YL) / pow(2, 15) * 2.0;
  return Y;
}

float readZ() {
  byte ZL;
  byte ZH;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
  Wire.write(0b10101100); // set to read desired address
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 2);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    ZL = Wire.read(); // this is being read
    ZH = Wire.read(); // this is being read
  }
  float Z = ((ZH << 8) + ZL) / pow(2, 15) * 2.0;
  return Z;
}

float readAccel(char dir) {
  byte accL;
  byte accH;
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
  if (dir == 'X' || 'x')
    Wire.write(0b10101000); // set to read desired address
  else if (dir == 'Y' || 'y')
    Wire.write(0b10101010); // set to read desired address
  else if (dir == 'Z' || 'z')
    Wire.write(0b10101100); // set to read desired address

  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 2);
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    accL = Wire.read(); // this is being read
    accH = Wire.read(); // this is being read
  }
  float acc = ((accH << 8) + accL) / pow(2, 15) * 2.0;
  return acc;
}


void getAccel(float *ptrToArray) { // passing pointer to a float
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
  *ptrToArray = ((XH << 8) + XL) / pow(2, 15) * 2.0;
  ptrToArray++;
  *ptrToArray = ((YH << 8) + YL) / pow(2, 15) * 2.0;
  ptrToArray++;
  *ptrToArray = ((ZH << 8) + ZL) / pow(2, 15) * 2.0;
}
