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
  // Check if the button has been pressed
  int buttonState = digitalRead(button);
 
  // Debounce/Toggle Logic for the button
  if(lastButton == HIGH && buttonState == LOW && millis() - lastTime > debounce)  {
    // If Led is on, turn it off
    if(blinking)  {
      blinking = false;
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
  
  // Output the voltage across the photodiode resistor in mV.
  Serial.println(photoValue);
  
  
  // Repeat loop 1kHz
  delay(1);
}
