# define PWMpin 3
float num_dataPts = 50.0; // data points per 1/2 wave
float freq = 10.0; //Hz
float halfPeriod = 1 / freq / 2 * 1000; // milliseconds per half period
float ms_dataPt = halfPeriod / num_dataPts; // milliseconds per data point
void setup() {
  Serial.begin(115200);
  pinMode(PWMpin, OUTPUT); // pwm output
}
unsigned long t_previous = millis();
void loop() {
  if (millis() >= t_previous + ms_dataPt) { // should record new data point
    t_previous = millis();
    int amp = 127.5 * (sin(millis()*freq/1000) + 1); // amplitude with adjusted frequency
    analogWrite(PWMpin, amp); // write duty cycle
    Serial.print("2.5");
    Serial.print(" ");
    Serial.println(5.0 * analogRead(A2) / 1023.0); // read and store duty cycle
  }
}
