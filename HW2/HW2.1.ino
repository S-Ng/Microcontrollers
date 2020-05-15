int count = 0;
bool previous = true;

void setup() {
  Serial.begin(9600);
  pinMode(2, INPUT_PULLUP); // pullup resistor for button
  pinMode(3, INPUT);
}
void loop() {
  Serial.print(previous); Serial.print(" "); Serial.println(count);
  if ((digitalRead(3) != previous) && (digitalRead(3) == false)) {
    count++; // button's state has changed from before and is pressed
  }
  previous = digitalRead(3); // update comparison value
}
