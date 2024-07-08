/*
Name: Anh Quoc Nguyen
Matriculation number: 1397466
*/

int ledPins[] = {5, 6, 7, 8, 9, 10, 11, 12}; // Array of LED pins
int numLeds = 8;
unsigned long lightOnTime = 2; // Light-on phase in milliseconds
unsigned int stepDuration = 500; // Step duration in milliseconds (calculated manually to achieve 2 s^-1 rate)
int buttonPin = 2; // Pin for the button

void setup() {
  Serial.begin(9600);
  for (int i = 0; i < numLeds ; i++) {
    pinMode(ledPins[i], OUTPUT); // Set each LED pin as output
  }
  pinMode(buttonPin, INPUT); // Set button pin as input with internal pull-up resistor
}

void loop() {
  if(digitalRead(buttonPin) == LOW){
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH); // Turn on current LED
      delay(lightOnTime); // Light on for 2 ms
      digitalWrite(ledPins[i], LOW); // Turn off current LED
      delay(stepDuration); // Wait for step duration
    }
  } else {
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], LOW); // Turn off current LED
    }
  }
}
