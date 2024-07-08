/*
Name: Anh Quoc Nguyen
Matriculation number: 1397466
*/


const int buttonPin1 = 2; // Pin connected to push-button S1
const int buttonPin2 = 3; // Pin connected to push-button S2

// Variables to store time values
unsigned long time1 = 0;
unsigned long time2 = 0;

void setup() {
  // Initialize serial communication
  Serial.begin(9600);

  // Initialize push-button pins as input with internal pull-up resistors
  pinMode(buttonPin1, INPUT_PULLUP);
  pinMode(buttonPin2, INPUT_PULLUP);

  // Display the initial message
  Serial.println();
  Serial.println("Hello this is a reaction Test of your name");
}

void loop() {
  // Wait until S1 is pressed
  while (digitalRead(buttonPin1) == HIGH) {
    // Do nothing, just wait
  }

  // Get the internal time1 using micros()
  time1 = micros();

  // Wait until S2 is pressed
  while (digitalRead(buttonPin2) == HIGH) {
    // Do nothing, just wait
  }

  // Get the internal time2 using micros()
  time2 = micros();

  // Display time1 in microseconds
  Serial.print("time1 in microseconds: ");
  Serial.println(time1);

  // Display time2 in microseconds
  Serial.print("time2 in microseconds: ");
  Serial.println(time2);

  // Calculate and display the time difference in microseconds
  unsigned long timeDifference = time2 - time1;
  Serial.print("time difference in microseconds: ");
  Serial.print(timeDifference);
  Serial.println(" ms");

  // Calculate and display the time difference in seconds as a float with 6 digits after the comma
  float timeDifferenceInSeconds = timeDifference / 1000000.0;
  Serial.print("time difference in seconds: ");
  Serial.print(timeDifferenceInSeconds, 6);
  Serial.println(" s");

  // Reset the times for the next loop
  time1 = 0;
  time2 = 0;
  
  // Wait a bit before next test
  delay(1000); 
}
