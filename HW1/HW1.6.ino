int LED[] = {7, 8};
int daly1 = 10;
int daly2 = 15;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {

  for (int i = 0; i < 2; i++) {
    digitalWrite(LED[i], HIGH);
  }
  delay(daly1);
  for (int i = 0; i < 2; i++) {
    digitalWrite(LED[i], LOW);
  }
  delay(daly2);
}
