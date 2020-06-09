/*
  deskMain.ino - Main script for controlling motorized standing desk.
  Created by Simon Ng, May 29, 2020. Last edit June 5, 2020
*/
#include "Desk.h"

Desk myDesk(9, 5, 11, 3, 0); // create a desk object called "myDesk"

String dir;
byte PIN_btn1 = 4;
byte PIN_btn2 = 7;
byte PIN_btn3 = 8;
byte PIN_btn4 = 12;
byte PIN_ThermAlert = 2;
byte PIN_ThermLED = 13;

volatile boolean state = LOW;

void setup() {
  Serial.begin(9600);
  myDesk.initialize();
  pinMode(PIN_btn1, INPUT_PULLUP); // button servo 1
  pinMode(PIN_btn2, INPUT_PULLUP); // button CRS 2
  pinMode(PIN_btn3, INPUT_PULLUP); // button CRS3
  pinMode(PIN_btn4, INPUT_PULLUP); // button CRS4

  myDesk.configureThermistor();
  pinMode(PIN_ThermAlert, INPUT_PULLUP);
  pinMode(PIN_ThermLED, OUTPUT);
  attachInterrupt(digitalPinToInterrupt(PIN_ThermAlert), ThermAlert, CHANGE); // when voltage changes
}

void loop() {
  delay(500); // this is necessary to keep motors from small steps each loop
  int dHeight = myDesk.getContDeltaHeight(); // get change in desk height
  if (dHeight <= 0) dir = "Down"; // get direction
  else dir = "Up";
  myDesk.movePlatform(dir, dHeight);
  delay(500);
  readButtons();
  //myDesk.printThermistorDiagnostics();
  //myDesk.printAccel();
  myDesk.levelPlatform(); // adjust motors until accelerometer reads level
  // myDesk.Troubleshoot();
}

void ThermAlert() { // temperature interrupt
  digitalWrite(PIN_ThermLED, state);
  state = !state;
}

void readButtons() { // manual lift buttons
  if (!digitalRead(PIN_btn1)) {
    myDesk.moveArm(1, "up", 50, true);
  }
  else if (!digitalRead(PIN_btn2)) {
    myDesk.moveArm(2, "up", 50, true);
  }
  else if (!digitalRead(PIN_btn3)) {
    myDesk.moveArm(3, "up", 50, true);
  }
  else if (!digitalRead(PIN_btn4)) {
    myDesk.moveArm(4, "up", 50, true);
  }
}
