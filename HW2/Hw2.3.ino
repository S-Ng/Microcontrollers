#define midLED 2
int rowLED[] = {2, 3, 4, 5, 6}; // row LEDs are pins 2-6
int colLED[] = {7, 8, 4, 9, 10}; // column LEDs are pins 7-10 sharing 4
#define LR_pin A0 // row joystick is pin A0
#define UD_pin A1 // column joystick is pin A1
bool midLED_on = false;

void setup() {
  Serial.begin(9600);
  pinMode(rowLED, OUTPUT);
  pinMode(colLED, OUTPUT);
}

void loop() {
  Serial.println(map(analogRead(UD_pin), 0, 1023, 0, 5));
  for (int i = 0; i < 5; i++) {
    if (i == map(analogRead(LR_pin), 0, 1023, 0, 5)) {
      if (i == midLED) {
        midLED_on = true; // middle should be on according to row
      } // but don't turn it on yet in case the column doesn't agree
      else {
        digitalWrite(rowLED[i], HIGH); // not middle LED, turn on
        midLED_on = false; // middle shouldn't be on according to row
      }
    }
    else {
      digitalWrite(rowLED[i], LOW);
    }
    if ((i == map(analogRead(UD_pin), 0, 1023, 0, 5))) {
      if (i != midLED) { // not middle LED, turn on
        digitalWrite(colLED[i], HIGH);
        midLED_on = false; // middle shouldn't be on according to row
      }
      else if (midLED_on) { // middle should also be on according to col
        digitalWrite(colLED[midLED], HIGH);
      }
      else {
        digitalWrite(colLED[midLED], LOW);
      }
    }
    else {
      digitalWrite(colLED[i], LOW);
    }
  }
}
