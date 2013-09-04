/* 
* Based off of the arduino example code.
*/
 
// Pin 13 has an LED connected on most Arduino boards.
// give it a name:
int led = 12;
int button = 2;
bool ledOn = true;
bool blinking = false;
int lastButton = 1;
int counter = 0;

long lastTime = 0;
long debounce = 50;

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);
  pinMode(button, INPUT);           // set pin to input
  digitalWrite(button, HIGH);       // turn on pullup resistors  
  Serial.begin(9600); 
}

// the loop routine runs over and over again forever:
void loop() {
  int sensorValue = analogRead(A0);
  int buttonState = digitalRead(button);
  
  
  Serial.println(lastButton == HIGH && buttonState == LOW);
 
  if(lastButton == HIGH && buttonState == LOW && millis() - lastTime > debounce)  {
    
    if(blinking)  {
      blinking = false;
      digitalWrite(led,LOW);
    }
    else  {
      blinking = true;
      counter = 0;
      digitalWrite(led,HIGH);
    }
    lastTime = millis(); 
  } 
  lastButton = buttonState;
  
  if(blinking)  {
    if (counter > sensorValue/2)  {
      counter = 0;
      if(ledOn)  {
        digitalWrite(led,LOW);
        ledOn = false;
      }
      else  {
        digitalWrite(led,HIGH);
        ledOn = true;
      }
    }
    counter = counter + 1;
  }
  
  delay(1);
}
