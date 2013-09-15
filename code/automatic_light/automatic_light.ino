/* 
*
* This was written for use with the tutorial at:
* https://github.com/SLERP/slerp-base/wiki/Automatic-Light
* 
* It will take a in an analog voltage input and toggle a digital
* PWM in negative feedback.
*
*/
 
// We name our pins with something sensible
int led = 11;
int button = 2;

// Some state variables
bool blinking = true;
int lastButton = 1;
long lastTime = 0;
long debounce = 50;
double pwmPulseWidth = 0;
int error;
int averagedValue;
int count;
int averager[20];

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
  int setPoint = 30;
  double k = 0.01;
  
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
  
 // PWM
  // Based on the voltage level input range, scale
  // the values so that the potentiometer gives us a full
  // 8 bits of PWM resolution.

  photoValue = (float(photoValue)/20) * 255;
  if (photoValue > 255) {
   // 2^8 = 256
   photoValue = 255;
  }
  if (photoValue < 0 )  {
    photoValue = 0;
  }
  if (count > 19)  {
    int averageInput = 0;
    for (int i = 0; i < 19; i++)  {
      averageInput+=averager[i];
    }
    averagedValue = averageInput/20;
    count = 0;
  }
  averager[count] = photoValue;
  count=count+1;
  
  error = averagedValue - setPoint;
  
  
  // For debug (use Arduino serial monitor in IDE).
  pwmPulseWidth = pwmPulseWidth - double(error)*k;
  Serial.println(pwmPulseWidth);
  if (pwmPulseWidth > 255) {
   // 2^8 = 256
   pwmPulseWidth = 255;
  }
  if (pwmPulseWidth < 0 )  {
    pwmPulseWidth = 0;
  }
  
  
  
  // Write the PWM value to the digital pin
  // analogWrite produces PWM output with 8bit resolution
  if(blinking)  {
      analogWrite(led,int(pwmPulseWidth));
  }
  
  
  // Repeat loop 1kHz
  delay(1);
}
