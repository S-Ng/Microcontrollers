#include <Wire.h>

#define ALERTPIN 2

void setup() {
  pinMode(ALERTPIN, INPUT_PULLUP);
  Wire.begin(); // join i2c bus (address optional for master)
  Serial.begin(9600);

  // Get temp limits in binary
  float roomTemp = 19.5;
  float t_up = roomTemp + 4.0; //C . Max resolution is 0.25 deg
  float t_low = 4.0; //C . Max resolution is 0.25 deg
  int intT_up = round(t_up * 4);
  int intT_low = round(t_low * 4);
  byte highT_up = intT_up >> 6;
  byte lowT_up = intT_up << 2;
  byte highT_low = intT_low >> 6;
  byte lowT_low = intT_low << 2;

  // Set resolution
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0b00001000)); // set pointer to go to temperature register
  Wire.write(byte(0b00000000)); // change resolution to +-0.5C
  Wire.endTransmission();     // stop transmitting

}
boolean runn = true;
void loop() {
  if (runn) {
    for (int myBigByte = 0; myBigByte < 10 ; myBigByte++) {
      for (int myByte = 0; myByte < 255 ; myByte++) {
        //Serial.println(myByte, BIN);

        // Reset Alarm
        Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
        Wire.write(byte(0x01)); // set pointer to go to configuration register
        Wire.write(byte(0b00000000)); // leave high byte defaults
        Wire.write(byte(0b00000000)); // turn on bit4 Alert Status, leave rest of bits as defaults (nothing locked and comparator mode)
        Wire.endTransmission();     // stop transmitting

        int t_norm;
        int t_alert;
        for (int t_low = 4; t_low < 30; t_low += 24) {
          float roomTemp = 19.5;
          float t_up = roomTemp + 4.0; //C . Max resolution is 0.25 deg
          int intT_up = round(t_up * 4);
          int intT_low = round(t_low * 4);
          byte highT_up = intT_up >> 6;
          byte lowT_up = intT_up << 2;
          byte highT_low = intT_low >> 6;
          byte lowT_low = intT_low << 2;

          // Set T_upper
          Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
          Wire.write(byte(0b0000010)); // set pointer to go to upper temp limit register
          Wire.write(highT_up); // set high byte
          Wire.write(lowT_up); // set low byte
          Wire.endTransmission();     // stop transmitting

          // Set T_lower
          Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
          Wire.write(byte(0b0000011)); // set pointer to go to lower temp limit register
          Wire.write(highT_low); // set high byte
          Wire.write(lowT_low); // set low byte
          Wire.endTransmission();     // stop transmitting

          // Activate Alarm
          Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
          Wire.write(byte(0x01)); // set pointer to go to configuration register
          Wire.write(myBigByte); // leave high byte defaults
          Wire.write(myByte); // iterate through possibilities
          Wire.endTransmission();     // stop transmitting

          if (t_low < 15)
          {
            t_norm = digitalRead(ALERTPIN);
          }
          else if (t_low > 20) {
            t_alert = digitalRead(ALERTPIN);
          }
        }
        if (t_norm != t_alert) {
          Serial.print("This byte combo might work: ");
          Serial.println(myByte, BIN);
        }
      }
    }
  }
  runn = false;
  Serial.println("Done");
}

/*
  Serial.print("DigitalRead: ");
  Serial.println(digitalRead(ALERTPIN));
  if (digitalRead(ALERTPIN)) { // temp alarm tripped
    Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
    Wire.write(byte(0x05));            // set pointer to read which limit was exceeded
    Wire.endTransmission();     // stop transmitting
    Wire.requestFrom(0b0011000, 1); // request 1 byte from slave device
    if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
      byte highByt = Wire.read(); // read first byte off buffer, high byte, largest number
      if (bitRead(highByt, 6)) { // temp is too hot
        // red LED
        //Serial.println("RED");
      }
      else if (bitRead(highByt, 5)) {// temp is too cold
        // Blue LED
        //Serial.println("BLUE");
      }
    }
    Serial.println("HIGH");
  }
  else {
    Serial.println("LOW");

  }

  // Calculate Temp for reference
  Wire.beginTransmission(0b0011000);  // this is our 7bit device address 0011000 default mcp9808 . Write address byte
  Wire.write(byte(0x05));            // set pointer to read temp
  Wire.endTransmission();     // stop transmitting
  Wire.requestFrom(0b0011000, 2); // request 2 bits from slave device
  if (Wire.available()) { // is there data to be read? If more than 0 bytes, read it
    byte highByt = Wire.read(); // read first byte off buffer, high byte, largest number
    byte lowByt = Wire.read(); // read first byte off buffer, high byte, largest number
    boolean negative = bitRead(highByt, 4); // we want the sign of the number
    boolean exceedsTupper = bitRead(highByt, 6); // is T_A>T_Upper tripped
    boolean exceedsTlower = bitRead(highByt, 5); // we want the sign of the number
    highByt = highByt & 0b00001111; // bitwise And. leftmost 3 bits automatically become 0, and others remain the same. now high byte only contains number values
    int intTemperature = (highByt << 8) + lowByt;// int is two bytes. bitshift operator has order of operations
    float MCP9808Temp = intTemperature / 16.0;
    if (negative) MCP9808Temp = -1 * MCP9808Temp;
    Serial.print(exceedsTupper);
    Serial.print(" ");
    Serial.println(exceedsTlower);
    Serial.println(MCP9808Temp, 6);
  }
  }//*/
