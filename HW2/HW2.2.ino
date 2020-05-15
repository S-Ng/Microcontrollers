unsigned long tStart;
unsigned long tPreLED;
unsigned long tElapsed;
#define btn1 3
#define btn2 6
#define LED 4

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); // pullup resistor for button 1
  pinMode(btn1, INPUT); // readout for button 1
  pinMode(LED, OUTPUT); // LED
  pinMode(5, INPUT_PULLUP); // pullup resistor for button 2
  pinMode(btn2, INPUT); // readout for button 2
  randomSeed(A0); // initialize random sequence
}
void loop() { // loop LED reflex game
  bool restart = false;
  Serial.println(); Serial.println("Both players press button to begin game.");
  while ((digitalRead(btn1) != false) || (digitalRead(btn2) != false)) {
    // wait until both of buttons are pressed (false)
  }
  while ((digitalRead(btn1) != true) || (digitalRead(btn2) != true)) {
    // wait until both of buttons are released (true)
  }
  Serial.println("Go!");
  tPreLED = millis() + random(0, 5000);
  while (millis() < tPreLED) { // wait random length of time 0-5 seconds
    if ((digitalRead(btn1) == false) || (digitalRead(btn2) == false)) {
      Serial.println("Too soon! The LED isn't on yet! Start Over.");
      restart = true; // button pressed too early, so restart game
      break; // don't print "Too soon..." more than once
    }
  }
  if (restart == false) {
    digitalWrite(LED, HIGH); // turn on LED
    tStart = millis(); // start timer
    while ((digitalRead(btn1) != false) && (digitalRead(btn2) != false)) {
      // wait until one of buttons is pressed (false)
    } // end of loop, so one of the buttons is pressed
    tElapsed = millis() - tStart; // time taken to press one of the buttons
    if (digitalRead(btn1) == false) { // button 1 was pressed
      Serial.print("Player w button on rows 24-26 pressed first with time of ");
      Serial.print(tElapsed); Serial.println(" ms");
    }
    else { // button 2 was pressed
      Serial.print("Player w button on rows 5-7 pressed first with time of ");
      Serial.print(tElapsed); Serial.println(" ms");
    }
    digitalWrite(LED, LOW); // turn off LED
    delay(500); // wait 0.5 seconds
  }
}
