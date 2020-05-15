#include <Wire.h>

int count = 0;

void setup() {
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);

  // Set resolution
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0b00001000)); // set pointer to go to temperature register
  Wire.write(byte(0b00000000)); // change resolution to +-0.5C
  Wire.endTransmission();     // stop transmitting

}

void loop() {
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0x05));            // set pointer to read temp
  Wire.endTransmission();     // stop transmitting
  
  Wire.requestFrom(0b0011000, 2); // request 2 bits from slave device
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    byte highByt = Wire.read(); // read first byte off buffer, high byte, largest number
    byte lowByt = Wire.read(); // read second byte off buffer, low byte, smallest nuber
    boolean negative = bitRead(highByt, 4); // we want the sign of the number
    highByt = highByt & 0b00001111; // bitwise And. leftmost 3 bits automatically become 0, and others remain the same. now high byte only contains number values
    int intTemperature = (highByt << 8) + lowByt;// int is two bytes. bitshift operator has order of operations
    float MCP9808Temp = intTemperature / 16.0;
    if (negative) MCP9808Temp = -1 * MCP9808Temp;

    // Thermister Temperature
    float V = 5.0 * analogRead(A0) / 1023.0;
    float R = V * 10000.0 / (5.0 - V); // resistance of thermister
    float thermisterT = 1.0 / (1.0 / 298.0 + 1.0 / 3950.0 * log(R / 10000.0)) - 273.15;

    // Print Temperatures from both methods
    /*//Serial.print("   MCP9808: ");
      Serial.print(MCP9808Temp, 6);
      //Serial.print("Thermister: ");
      Serial.print(" ");
      Serial.println(thermisterT, 6);
    */
    count++;
    //  if (count <50) {
    //Serial.print(MCP9808Temp, 6);
    //Serial.print(" ");
    //Serial.println(thermisterT, 6);
    //}
    delay(100);
  }

  byte tByte = 0b00000000;
  
  int testByte[8];//
  int count = 0;
  for (int i = 7; i > -1; i--){
    bitWrite(tByte, i, bitRead(25,i));
    testByte[count] = bitRead(25,i);
    count++;
   // testByte = testByte << 1;
      //Serial.print(testByte,BIN);

  }
  //for (int i = 0; i<8; i++)  Serial.print(testByte[i]);
  Serial.println(tByte);
  byte myByte = lowByte(25);
  //Serial.println(myByte);
}
