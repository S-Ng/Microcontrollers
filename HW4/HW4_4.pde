import processing.serial.*;
import cc.arduino.*;

float V[] = {0, 0};
float R;
float T[] = {0, 0};
float R_i = 1000.0;
float R_f = 10000.0;
String data[] = {"Time:" + TAB + "Direct:" + TAB + "Amplified:"};
int count = 0;
int numPts = 5;
int t_delay = 1000; //(ms)
int t_start;
Arduino arduino;

void setup() {
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  println("Start");
}

void draw() {
  if (count == 0) {
    t_start = millis()-t_delay;
  }
  if (millis() >= (t_start + t_delay) && arduino.analogRead(0) != 0) {
    t_start = millis();
    count++;
    for (int i = 0; i < 2; i++) {
      V[i] = 5.0 * arduino.analogRead(i) / 1023.0;
      R = 5.0*10000.0 / V[i] - 10000.0; // unamplified thermister resistance (thermister located near 5V terminal)
      if (i == 1) {
        R = 5.0*10000.0 / ((2.5-V[i])*R_i/R_f+2.5)-10000.0; // amplified thermister resistance
      }
      T[i] = (1.0 / (1.0 / 298.0 + 1.0 / 3950.0 * log(R / 10000.0))); // Steinhart-Hart Eqn (Celcius)
    }
    String temp = str(t_start) + TAB+ str(T[0]-273.15) + TAB + str(T[1]-273.15);
    data = append(data, temp);
    if (count == numPts) {
      saveStrings("Temperature.txt", data);
      println("Saved");
    }
  }
}
