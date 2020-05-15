int LED[] = {2, 3, 4, 5, 6};
int counter = 0;
int oldNumLit = -1;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
  randomSeed(analogRead(0));

}

// the loop function runs over and over again forever
void loop() {
    int numLit = random(0, 6); // random number (0-5)
    Serial.println(numLit);
    for (int i = 0; i < numLit ; i++){
    digitalWrite(LED[i], HIGH); // turn LEDs on
    }
    delay(2000); // wait 2 sec  
    for (int i = -1; i < numLit ; i++){
    digitalWrite(LED[i], LOW); // turn LEDs off
    }  
}
