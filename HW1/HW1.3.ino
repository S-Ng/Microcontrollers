int LED[] = {2, 3, 4, 5, 6};
int counter = 0;
int oldNumLit = -1;
int currentTime = millis();

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  if ((millis() - currentTime) >= 10) { // if at least 10 ms have elapsed
    currentTime = millis();
    int numLit = log(counter) / log(4) - 1;
    digitalWrite(LED[numLit], HIGH); // turn new LED on
    if (numLit > oldNumLit) {
      Serial.println(counter);
      oldNumLit = numLit;
    }
    counter++; // increment counter
  }
  else {} // repeat until 10 ms have elapsed
}
