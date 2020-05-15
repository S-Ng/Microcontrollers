import processing.serial.*;
import cc.arduino.*;

float V[] = {0, 0}; // Voltages (unamplified and amplified)
float R; // resistance
float T[] = {0, 0}; // temperature (kelvin)
float R_i = 1000.0; // inverting op-amp resistor 1
float R_f = 10000.0; // inverting op-amp resistor 2
String data[] = {"Time:" + TAB + "Direct:" + TAB + "Amplified:"}; // Processing storage file
int count = 0; // overall count of data points collected
int numPts = 500; // number of points before saving/resaving data to .txt file
int xsize = 600; // x-axis of plotting window
int ysize = 400; // y-axis of plotting window
int j_start = 1; // start at 1 to skip data[] header
int lBound = 22; // lower temp on plot
int uBound = 27; // upper temp on plot

int t_start; // timer start for delay
int t_delay = 100; // delay between temperature reads (ms)

Arduino arduino; // create arduino object

void setup() {
  size(600, 400); // make these the same as x and ysize
  background(220);
  arduino = new Arduino(this, Arduino.list()[0], 57600);
  println("Start");
}

void draw() {
  // CALCULATE + STORE TEMPERATURE
  if (count == 0) { // first temp reading
    t_start = millis()-t_delay; // don't wait
  }
  if (millis() >= (t_start + t_delay) && arduino.analogRead(0) != 0) { // delay & don't read bogus 0 analogRead
    t_start = millis(); // start timer
    count++; // increment data point counter
    for (int i = 0; i < 2; i++) {
      V[i] = 5.0 * arduino.analogRead(i) / 1023.0; // read voltage from arduino
      R = 5.0*10000.0 / V[i] - 10000.0; // unamplified thermister resistance (thermister located near 5V terminal)
      if (i == 1) { // use different calculation for R
        R = 5.0*10000.0 / ((2.5-V[i])*R_i/R_f+2.5)-10000.0; // amplified thermister resistance
      }
      T[i] = (1.0 / (1.0 / 298.0 + 1.0 / 3950.0 * log(R / 10000.0))); // Steinhart-Hart Eqn (Celcius)
    }
    String temp = str(t_start) + TAB+ str(T[0]-273.15) + TAB + str(T[1]-273.15); // add time and both temperatures to string
    data = append(data, temp); // store new temp string in data array

    // SAVE DATA TO COMPUTER
    if (count%numPts==0) {
      saveStrings("Temperature.txt", data);
      println("Saved");
    }

    // PREPARE PLOT
    background(220); // reset screen to plot shifted points
    fill(0);
    textSize(20);
    text(str(uBound), 30.0, map(float(uBound), lBound, uBound, ysize, 0)+15.0);
    text(str(lBound), 30.0, map(float(lBound), lBound, uBound, ysize, 0)-7.0);
    text(str(25), 30.0, map(25.0, lBound, uBound, ysize, 0)-2.0);
    textSize(15);
    text("Time >>", xsize/2, ysize-10.0);
    rotate(PI/2.0);
    textAlign(CENTER);
    text("<< Calculated Temperature [C]", ysize/2, -10);
    rotate(-PI/2.0);
    textAlign(LEFT);
    text("Unamplified Temp (C)", xsize-200, 30.0);
    text("Amplified Temp (C)", xsize-200, 50.0);
    strokeWeight(6); 
    stroke(255, 0, 0);
    point(xsize-210, 24.0);
    stroke(0, 0, 255);
    point(xsize-210, 44.0);

    // PLOT SCROLLING DATA
    if (count > xsize) {
      j_start = count-xsize;
    }
    for (int j = j_start; j < count; j++) {
      String[] dataPt = split(data[j], TAB); // split data by TAB delimeter
      strokeWeight(3); 
      stroke(255, 0, 0);
      point(j-j_start, map(float(dataPt[1]), lBound, uBound, ysize, 0));//red unamplified temp
      stroke(0, 0, 255);
      point(j-j_start, map(float(dataPt[2]), lBound, uBound, ysize, 0));//blue amplified temp
      stroke(0); 
      strokeWeight(1);
      point(j-j_start, map(25, lBound, uBound, ysize, 0)); //black 25C reference line
    }
  }
}
