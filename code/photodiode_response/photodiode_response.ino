/* 
* Based off of the arduino example code.
*
* This was written for use with the tutorial at:
* https://github.com/SLERP/slerp-base/wiki/Blinker-Button
*
* It will take a voltage level and send out a proportional
* PWM signal. Toggle the PWM with a digital input.
*
*/
 
// We name our pins with something sensible
int led = 11;
int button = 2;

// Some state variables
bool blinking = false;
int lastButton = 1;
int pwmPulseWidth;
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

  // Read a voltage level at the analog input
  int sensorValue = analogRead(A0);
  int photoValue = analogRead(A1);
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
  
  // PWM
  // Based on the voltage level input range, scale
  // the values so that the potentiometer gives us a full
  // 8 bits of PWM resolution.
  pwmPulseWidth = (float(sensorValue)/700) * 255 - 60;
  if (pwmPulseWidth > 255) {
   // 2^8 = 256
   pwmPulseWidth = 255;
  }
  if (pwmPulseWidth < 0 )  {
    pwmPulseWidth = 0;
  }
  
  // For debug (use Arduino serial monitor in IDE).
  Serial.println(photoValue);
  
  // Write the PWM value to the digital pin
  // analogWrite produces PWM output with 8bit resolution
  if(blinking)  {
      analogWrite(led,pwmPulseWidth);
  }
  
  // Repeat loop 1kHz
  delay(1);
}
