// Define pins
int potPin = A0; // Potentiometer connected to analog pin A1
int ledHighPin = 5; // First LED (active high) connected to PWM pin 9
int ledLowPin = 6; // Second LED (active low) connected to PWM pin 10
int digitalInputPin = 2;

void setup() {
  Serial.begin(2000000); // Initialize serial communication at 2000000 baud
  pinMode(ledHighPin, OUTPUT);
  pinMode(ledLowPin, OUTPUT);
  pinMode(9, OUTPUT);
  pinMode(digitalInputPin, INPUT); // Set digital pin as input
}

void loop() {
  // Read the analog value from the potentiometer
  int potValue = analogRead(potPin);
  
  // Display the potentiometer value in the Serial Monitor
  //Serial.println(potValue);
  
  // Map the potentiometer value (0-1023) to PWM range (0-255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  
  // Write the PWM value to the first LED (active high)
  analogWrite(ledHighPin, pwmValue);
  
  // Write the inverse PWM value to the second LED (active low)
  analogWrite(ledLowPin, 255 - pwmValue);

  analogWrite(9, pwmValue);
  // Read the digital input pin connected to the PWM output
  int digitalValue = digitalRead(digitalInputPin);
  
  // Write the digital value to the serial output
  Serial.println(digitalValue, DEC);
  
  // Small delay to stabilize the readings
  delay(10);
}

