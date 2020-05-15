import processing.serial.*;
import cc.arduino.*;

float V[] = {0 ,0 };
float R;
float T;
float R_i = 1000.0;
float R_f = 10000.0;
String[] output = {"Direct T: ", "Amplified T: "};

Arduino arduino;
void setup() {
  println(Arduino.list());
  size(400, 400);
  background(0);
  arduino = new Arduino(this, Arduino.list()[0], 57600);
}

void draw() {
  background(0);
  for (int i = 0; i < 2; i++) {
    V[i] = 5.0 * arduino.analogRead(i) / 1023.0;
    R = 5*10000.0 / V[i] - 10000.0; // resistance of thermistor (when located closer to 5V)
    if (i == 1) {
    R = 5*10000.0 / ((2.5-V[i])*R_i/R_f+2.5)-10000.0;
    }
    T = 1.0 / (1.0 / 298.0 + 1.0 / 3950.0 * log(R / 10000.0)); // Steinhart-Hart Eqn
    String myString = output[i] + (T-273) + "C";
    textSize(20);
    text(myString, 50, 200+i*100);
  }
}
