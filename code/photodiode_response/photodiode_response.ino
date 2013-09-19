/* 
* Based off of the arduino example code.
*
* This was written for use with the tutorial at:
* https://github.com/SLERP/slerp-base/wiki/Photodiode-Response
* 
* It will take a in an analog voltage input and toggle a digital on and off.
*
*/
 
// We name our pins with something sensible
int led = 11;
int button = 2;

// Some state variables
bool blinking = false;
int lastButton = 1;
long lastTime = 0;
long debounce = 50;
int stepLevel = 0;
int count = 0;
int averager[100];

// the setup routine runs once when you press reset:
void setup() {                
  // initialize the digital pin as an output.
  pinMode(led, OUTPUT);             // set LED pin to power LED
  pinMode(button, INPUT);           // set pin to input
  digitalWrite(button, HIGH);       // turn on pullup resistors  
  Serial.begin(9600);               // Serial output for debug
}

// the loop routine runs over and over again forever:
void loop() {

  int photoValue = analogRead(A0);
  photoValue = (float(photoValue)/100) * 255;
  if (photoValue > 255) {
   // 2^8 = 256
   photoValue = 255;
  }
  if (photoValue < 0 )  {
    photoValue = 0;
  }
  // Check if the button has been pressed
  int buttonState = digitalRead(button);
 
  // Debounce/Toggle Logic for the button
  if(lastButton == HIGH && buttonState == LOW && millis() - lastTime > debounce)  {
    // If Led is on, turn it off
    if(blinking)  {
      blinking = false;
      stepLevel = 0;
      count=0;
      digitalWrite(led,LOW);
    }
    // If LED is off, turn it on
    else  {
      blinking = true;
      digitalWrite(led,HIGH);
    }
    lastTime = millis(); 
  } 
  lastButton = buttonState;
  
  if (blinking)  {
  
    if (count > 99)  {
      // Output the voltage across the photodiode resistor in mV.
      int averageInput = 0;
      for (int i = 0; i < 99; i++)  {
        averageInput+=averager[i];
      }
      averageInput = averageInput/100;
      Serial.println(averageInput);
      
      stepLevel = stepLevel + 25;
      if (stepLevel > 249)  {
        stepLevel = 0;
      }
      analogWrite(led,stepLevel);
      
      count = 0;
    }
    averager[count] = photoValue;
    count=count+1;
  }
  
  
  
  
  
  // Repeat loop 1kHz
  delay(1);
}
