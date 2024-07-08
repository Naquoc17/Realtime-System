/*
Name: Anh Quoc Nguyen
Matriculation number: 1397466
*/

int ledPins[] = {5, 6, 7, 8, 9, 10, 11, 12}; // Array of LED pins
int numLeds = 8;
int lightOnTime = 2; // Light-on phase in milliseconds
int stepDuration = 61; // Step duration in milliseconds (calculated manually to achieve 2 s^-1 rate)
int buttonPin = 2; // Pin for the button

volatile bool runningMode = true;
unsigned long lastDebounceTime = 0;
unsigned long debounceDelay = 50;

void setup() {
  Serial.begin(9600);
  Serial.print("13");
  for (int i = 0; i < numLeds ; i++) {
    pinMode(ledPins[i], OUTPUT); // Set each LED pin as output
  }
  pinMode(buttonPin, INPUT); // Set button pin as input with internal pull-up resistor
  attachInterrupt(digitalPinToInterrupt(buttonPin), toggleMode, RISING);
}

void loop() {
  if(runningMode){
    for (int i = 0; i < numLeds; i++) {
      digitalWrite(ledPins[i], HIGH); // Turn on current LED
      delay(lightOnTime); // Light on for 2 ms
      digitalWrite(ledPins[i], LOW); // Turn off current LED
      delay(stepDuration); // Wait for step duration
    }
  } else {
    digitalWrite(ledPins[0], HIGH);
    digitalWrite(ledPins[7], HIGH);
    for (int i = 1; i < numLeds - 1; i++) {
      digitalWrite(ledPins[i], LOW);
    }
    delay(100);
  }
}

void toggleMode(){
  unsigned long currentTime = millis();
  if((currentTime - lastDebounceTime) > debounceDelay){
    runningMode = !runningMode;
  }

  if (runningMode){
    Serial.println("running ...");
    digitalWrite(ledPins[0], LOW);
    digitalWrite(ledPins[7], LOW);
  } else {
    Serial.println("idle ...");
    for (int i = 0; i < numLeds; i++) {
        digitalWrite(ledPins[i], LOW);
    }
  }

  lastDebounceTime = currentTime;
}
