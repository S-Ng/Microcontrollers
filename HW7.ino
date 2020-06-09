#include <Wire.h>
#include "AccelerometerLib.h"

byte XL;
byte XH;
byte YL;
byte YH;
byte ZL;
byte ZH;

void setup() {
  Serial.begin(9600);
  cofigureAccelerometer();
}

void loop() {
  //printByte(readByte(byte(0x0F))); // WHO_AM_I

  /*
    // Read Accelerations
    Wire.beginTransmission(0b0011000);  // this is our 7bit device address w/ SD0 to GND
    Wire.write(0b10101000); // set to read desired address (also set bit 7 to 1 for multiple byte reads)
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

    byte[] acc = readByte(0b10101000, 6);
    for (int i = 0 ; i < 6, i++) {
      Serial.println(acc(i));
    }
    // Convert to g's
    float X = ((XH << 8) + XL) / pow(2, 15) * 2.0;
    float Y = ((YH << 8) + YL) / pow(2, 15) * 2.0;
    float Z = ((ZH << 8) + ZL) / pow(2, 15) * 2.0;
  */

  float accel[3];
  getAccel(accel); // passes pointer to beginning of array
  float X = accel[0];
  float Y = accel[1];
  float Z = accel[2];

  /*
    float X = readAccel('x');
    float Y = readAccel('y');
    float Z = readAccel('z');
  */

  // Print for Processing input
  Serial.print(X, 6);
  Serial.print(" ");
  Serial.print(Y, 6);
  Serial.print(" ");
  Serial.print(Z, 6);
  Serial.println(" ");
  delay(100);
}
