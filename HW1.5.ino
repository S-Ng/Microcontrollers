int LED[] = {2, 3, 4, 5, 6};
int num = 1;
int litLEDs[5]; // binary pattern
int oldLitLEDs[5] = {0, 0, 0, 0, 0}; // old binary pattern
int method = 2; // set method to change which way of calculating is chosen

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  if (method == 1) {// get binary code by converting desired numerical value to binary array
    int tempNum = num;
    if (num < 32) {
      for (int i = 4; i > -1 ; i--) { // get binary code
        int power = pow(2.0, i + 0.01);
        if ( (tempNum % power) < tempNum ) { // number contains given power of 2
          litLEDs[i] = 1;
          tempNum -= power; //
        }
        else {
          litLEDs[i] = 0;
        }
      }
      Serial.print(num);  Serial.print(" : "); Serial.print(litLEDs[4]); Serial.print(litLEDs[3]); Serial.print(litLEDs[2]); Serial.print(litLEDs[1]); Serial.println(litLEDs[0]);

      for (int j = 0; j < 5 ; j++) {
        if (litLEDs[j] == 1) {
          digitalWrite(LED[j], HIGH); // turn LEDs on
        }
      }
      delay(1000);

      for (int k = 0; k < 5 ; k++) {
        if (litLEDs[k] == 1) {
          digitalWrite(LED[k], LOW); // turn LEDs off
        }
      }
      num++; // increment number
    }
  }

  else if (method == 2) { // get binary code by updating from previous code
    for (int i = 0; i < 5 ; i++) {
      if (oldLitLEDs[i] == 0) {
        litLEDs[i] = 1; // fill in spot
        break;
      }
      else {
        litLEDs[i] = 0; // clear spot
      }

    }

    for (int j = 0; j < 5 ; j++) {
      if (litLEDs[j] == 1) {
        digitalWrite(LED[j], HIGH); // turn LEDs on
      }
    }

    delay(500);

    for (int k = 0; k < 5 ; k++) {
      if (litLEDs[k] == 1) {
        digitalWrite(LED[k], LOW); // turn LEDs off
      }
    }
    Serial.print(litLEDs[4]); Serial.print(litLEDs[3]); Serial.print(litLEDs[2]); Serial.print(litLEDs[1]); Serial.println(litLEDs[0]);

    for (int m = 0; m < 5 ; m++) {
      oldLitLEDs[m] = litLEDs[m]; // update old binary list
    }
  }
}
