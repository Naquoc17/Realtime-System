int ledPin[] = {5,6,7,8,9,10,11,12};
#define button 2


void setup() {
  // put your setup code here, to run once:
  Serial.begin(9600);
  for (int i = 0; i<=7; i++){
    pinMode(ledPin[i], OUTPUT);
  }
  pinMode(button, INPUT_PULLUP);
  
}

void loop() {
  int sensor = digitalRead(button);
  Serial.println(sensor);
  
  
  // put your main code here, to run repeatedly:
  if(sensor == HIGH){
    for (int i = 0; i<=7; i++){
      pinMode(ledPin[i], HIGH);
      }
   
    } else {
    for (int i = 0; i<=7; i++){
      pinMode(ledPin[i], LOW);
      }
    }

    for (int i = 0; i<=7; i++){
      digitalWrite(ledPin[i], LOW);
      delay(60);
      digitalWrite(ledPin[i], HIGH);
      delay(2);
    }
  
}
