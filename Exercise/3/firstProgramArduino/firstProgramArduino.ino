/*
  Name: Anh Quoc Nguyen
  Matriculation number: 1397466

  

*/

int LED_PIN[] = {2, 3, 4, 5};
bool ledState[ sizeof(LED_PIN) / sizeof(LED_PIN[0]) ] = {};

void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i < sizeof(LED_PIN) / sizeof(LED_PIN[0]); i++){
    pinMode(LED_PIN[i], OUTPUT);
  }

}

void loop() {
  // Turn the leds on
  for (int i = sizeof(LED_PIN)/sizeof(LED_PIN[0]) - 1; i >= 0; i--){
    digitalWrite(LED_PIN[i], HIGH);
    ledState[i] = true;
    delay(200);
  }

  // Delay 1s before turning off
  delay(200);

  // Turn the leds off
  digitalWrite(LED_PIN[2], LOW);
  ledState[2] = false;
  delay(200);

  for (int i = 0; i < sizeof(LED_PIN) / sizeof(LED_PIN[0]); i++){
    if(ledState[i] == true){
      digitalWrite(LED_PIN[i], LOW);
      ledState[i] = false;
      delay(200);
    }
  }
}
