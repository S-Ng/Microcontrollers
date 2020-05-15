#define R 3
#define G 5
#define B 6
void setup() {
  Serial.begin(9600);
  pinMode(R, OUTPUT); // pwm output
  pinMode(G, OUTPUT); // pwm output
  pinMode(B, OUTPUT); // pwm output
}

void loop() {
  for (int Rval = 0; Rval < 255 ; Rval+=10) {
    for (int Gval = 0; Gval < 255 ; Gval+=10) {
      for (int Bval = 0; Bval < 255 ; Bval+=10) {
        analogWrite(R, Rval);
        analogWrite(G, Gval);
        analogWrite(B, Bval);
        Serial.print(Rval);         
        Serial.print(" ");
        Serial.print(Gval);
        Serial.print(" ");
        Serial.println(Bval);
      }
    }
  }
}
