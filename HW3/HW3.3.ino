float Vsum = 0;
int count = 0;
void setup() {
  Serial.begin(9600);
}
void loop() {
  float Vin = 5.0 * analogRead(A5) / 1023.0;
  Vsum += Vin;
  count++;
  if (count == 10000) {
    Serial.println(Vsum / 10000, 6);
    count = 0;
    Vsum = 0;
  }
}
