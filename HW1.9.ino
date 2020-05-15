int LED[] = {7, 8};
int daly1 = 30;
int daly2 = 10;

// the setup function runs once when you press reset or power the board
void setup() {
  pinMode(LED, OUTPUT);
  Serial.begin(9600);
}

// the loop function runs over and over again forever
void loop() {
  digitalWrite(LED[1], HIGH); // turn on 1k resistor LED
  //digitalWrite(LED[0], HIGH); // turn on 1k resistor LED
  digitalWrite(LED[0], HIGH);
  delay(daly1);
  digitalWrite(LED[0], LOW);
  delay(daly2);
}
