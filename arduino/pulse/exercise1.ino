// Define pins
const int potPin = A0; // Potentiometer connected to analog pin A1
const int ledHighPin = 5; // First LED (active high) connected to PWM pin 9
const int ledLowPin = 6; // Second LED (active low) connected to PWM pin 10

void setup() {
  // Initialize serial communication for monitoring
  Serial.begin(9600);
  
  // Set the LED pins as output
  pinMode(ledHighPin, OUTPUT);
  pinMode(ledLowPin, OUTPUT);
}

void loop() {
  // Read the analog value from the potentiometer
  int potValue = analogRead(potPin);
  
  // Display the potentiometer value in the Serial Monitor
  Serial.println(potValue);
  
  // Map the potentiometer value (0-1023) to PWM range (0-255)
  int pwmValue = map(potValue, 0, 1023, 0, 255);
  
  // Write the PWM value to the first LED (active high)
  analogWrite(ledHighPin, pwmValue);
  
  // Write the inverse PWM value to the second LED (active low)
  analogWrite(ledLowPin, 255 - pwmValue);
  
  // Small delay to stabilize the readings
  delay(10);
}
