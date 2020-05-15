float Vout;
float Vamp;
float R;
float T;
void setup() {
  Serial.begin(9600);
}

void loop() {
  Vout = 5.0 * analogRead(A0) / 1023.0;
  Vamp = 5.0 * analogRead(A1) / 1023.0;
  R = 50000.0 / Vout - 10000.0; // resistance of thermister (when thermister is closer to 5V)
  T = 1.0 / (1.0 / 298.0 + 1.0 / 3950.0 * log(R / 10000.0)); // Steinhart-Hart Eqn
  Serial.println(T - 273.0, 6); // Temp (C)
}
