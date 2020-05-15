# define PWMpin 3
int dAmp = 31;
float ms_dataPt = 1000 / 160; // time for 16 datapoints in 10 Hz wave
int amp = 0;
int previous_amp = 0;
int data[400];
int count = 0;

void setup() {
  Serial.begin(115200);
  pinMode(PWMpin, OUTPUT); // pwm output
}

unsigned long t_previous = millis();

void loop() {
  if (millis() >= (t_previous + ms_dataPt)) { // should send and read next datapoint
    t_previous = millis(); // update reference time
    if ((amp >= previous_amp) && (amp < 255)) { // amplitude should still grow
      amp += dAmp;
      Serial.println("amp up");
    }
    else if (amp < previous_amp) { // amplitude should shrinking
      amp -= dAmp;
      Serial.println("amp down 1");
    }
    else { // amplitude was growing but is greater than 255 and should start shrinking
      amp -= dAmp;
      Serial.println("amp down 2");
    }
    Serial.println(amp);
    analogWrite(PWMpin, amp); // write duty cycle
    data[count] = 5.0 * analogRead(A2) / 1023.0; // read and store duty cycle
    previous_amp = amp; // update amp
    count++;
  }
  else {} // do nothing and wait
  /*if (count == 400) { // data is full, print
    for (int a = 0; a < count; a++) {
      Serial.println(data[a]);
    }}*/
}
