int LED[] = {2, 3, 4, 5, 6};

void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

void loop() {

  for (int i = 0; i < 5; i++) {
    digitalWrite(LED[i], HIGH);
    delay(1000);
    digitalWrite(LED[i], LOW);
    delay(1000);
  }
}
