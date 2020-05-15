# define PWMpin 3
int amp = 0;
# define numPts 400
float data[numPts];
int count = 0;

float num_dataPts = 50.0; // data points per 1/2 wave
float freq = 0.10; //Hz
float halfPeriod = 1 / freq / 2 * 1000; // milliseconds per half period
float ms_dataPt = halfPeriod / num_dataPts; // milliseconds per data point
int dAmp = 255.0 / num_dataPts; // change in "amplitude" per point (really change in duty cycle)

void setup() {
  Serial.begin(115200);
  pinMode(PWMpin, OUTPUT); // pwm output
}
unsigned long t_begin = millis();
unsigned long t_previous = millis();

void loop() {
  if (millis() >= t_previous + ms_dataPt) { // should record new data point
    t_previous = millis();
    if (millis() < t_begin + halfPeriod) { // should be increasing amplitude
      amp += dAmp;
    }
    else if (millis() < t_begin + 2 * halfPeriod) { // should be decreasing amplitude
      amp -= dAmp;
    }
    else { // new period
      t_begin = millis();
      amp = 0;
    }
    analogWrite(PWMpin, amp); // write duty cycle
    Serial.println(5.0 * analogRead(A2) / 1023.0); // read and store duty cycle
    //count++;
  }
/*
  if (count == numPts) { // data is full, print
    for (int a = 0; a < numPts; a++) {
      Serial.print("2.5");
      Serial.print(" ");
      Serial.println(data[a]);
    }
    t_begin = millis();
    t_previous = millis();
    count = 0;
  }*/
}
