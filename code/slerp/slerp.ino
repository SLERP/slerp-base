/*
*
*  This was written for use with the tutorial at:
*  https://github.com/SLERP/slerp-base/wiki/Creating-a-Drink-Mixer!
*  
*  This is the culminating code that brings everything together. You'll notice much of the code
*  from previous tutorials is reused. The thing to notice is more of a use of functions, which
*  help simplify the logic in the main loop.
*
*  The user selects their sugar amount by moving the potentiometer to the desired setting. Once the
*  button is pressed, sugar is dumped into the cup and stirred in "one unit" increments until the 
*  desired transmittance (solution concentration) is reached. It is up to the design teams to decide
*  what "one unit" is. See my implementation below.
*
*  Once int.0 (an interrupt on PIN2) recognizes a falling edge (transition from HIGH to LOW),
*  boolean intrcv is set HIGH. This flag will cause the mixer to start its automated mixing process.
*
*/
#include <Servo.h>

Servo myservo;
//Pin Configuration
int led = 8;
int button = 2;
int PWMsignal = 9;
int IN1 = 12;
int IN2 = 13;
int photoMix = 0;
int potMix = 0;
// photodiode is connected to A0 (how to specify?)
// potentiometer is connected to A1

// Some state variables
bool intrcv = LOW;                        //flag for interrupt
int velocity = 1500;                      //slow, CW movement
int onerot = 9000;                        //time for full rotation
int STOP = 1510;                          //stops servo
int lighthreshold = 180;                  //based on ambient lighting conditions
int timerotate = 5000;                        //5 second delay
int timedelay = 1000;                         //1 second delay
int mixThreshold = 180;                  // The light setting for "no sugar" mix (pot == 0)

// the setup routine runs once when you press reset 
void setup() 
{ 
    
  // DC motor
  pinMode(IN1, OUTPUT);                       //set IN1 to output
  pinMode(IN2, OUTPUT);                       //set IN2 to output
  digitalWrite(IN1, LOW);                     //set IN1 & IN2 LOW
  digitalWrite(IN2, LOW);                     //so motor is stopped

  // Servo
  myservo.attach(PWMsignal, 1300, 1700);      // designates the pin for the servo &
                                              // provides the min & max for PWM signal
         
  // LED                                 
  pinMode(led, OUTPUT);                       // set LED pin to power LED
  digitalWrite(led, LOW);
  
  // Photodiode
  photoMix = getPhotoValue();
  
  // Potentiometer
  potMix = getPotValue();
  
  //Button
  digitalWrite(button, HIGH);                 // turn on pullup resistors
  attachInterrupt(0, inthandler, FALLING);    //set button to interrupt
  Serial.begin(9600);                         // Serial output for debug
}

void loop()
{
    // only start once button pressed, then start program
    if(intrcv)
    {
    // Mix the drink
        
        // get mix ratio setting (0-255 from pot)
        potMix = getPotValue();   
        photoMix = getMixValue(mixThreshold);
        
        // add "mix units" until desired mix is achieved
        // while loop is dangerous! -- how can we avoid an infinite loop?
        // may need to change the direction of the inequality!
        while(photoMix < potMix)
        {
            dropMix();
            delay(100);
            spinBlade();
            delay(100);
            photoMix = getMixValue(mixThreshold);
        }
        
        // end of button sequence
        intrcv = LOW;
    }

}

// ********** Helper Functions ***********

//  the interrupt service routine will run when the button is pressed:
void inthandler()
{
  intrcv = HIGH;                               //set interrupt flag high
}

// drop in "one unit" of mix (spin once)
// ** This function may need to be modified based on the design **
void dropMix()
{
    myservo.writeMicroseconds(velocity); 
    delay(onerot);
    myservo.writeMicroseconds(STOP); 
}

// get photodiode value and scale to 0-255
int getPhotoValue()
{
    int photoValue = (float(analogRead(A0))/100) * 255;
    if (photoValue > 255) {
    // 2^8 = 256
    photoValue = 255;
    }
    if (photoValue < 0 )  {
        photoValue = 0;
    }
    //Serial.println(photoValue);                  //Output brightness for debug
    
    return photoValue;
}

// get potentiometer value and scale to 0-255
int getPotValue()
{   
    // from empirical testing
    int potValue = (float(analogRead(A1))/700) * 255 - 60;
    
    if (potValue > 255) {
    // 2^8 = 256
    potValue = 255;
    }
    if (potValue < 0 )  {
        potValue = 0;
    }
    //Serial.println(potValue);                  //Output brightness for debug
    
    return potValue;
}

// spin the blade two directions for maximum mixing (TM)!
void spinBlade()
{
    digitalWrite(IN1, HIGH);                  //Set IN1 HIGH, motor rotate
    delay(timerotate);                        //rotate for set time
    digitalWrite(IN2, HIGH);                  //Set IN2 HIGH, motor BRAKE
    delay(timedelay);                         //settling time
  
    digitalWrite(IN1, LOW);                   //Set IN1 LOW, motor rotate opposite
    delay(timerotate);                        //rotate for set time
    digitalWrite(IN2, LOW);                   //Set IN2 LOW, motor STOP
    delay(timedelay);                         //settling time
}

// get the measured (scaled) photodiode response to an LED flash
// scale by a maximum value
int getMixValue(int photoThresh)
{
     // get sensor readings
     digitalWrite(led, HIGH);
     delay(10); //just in case we're running on a *really* fast processor
     int photoValue = getPhotoValue();
     digitalWrite(led, LOW);
     
     if (photoValue > photoThresh)
     {
       return 0;
     }
     
     // find mix completion from 0-255 (where 255 is all sugar, and 0 is no mix)
     // here we know photoValue < photoThresh and want small photoValues to report
     // a better mix.
     int mixVal = (float(photoThresh - photoValue)/float(photoThresh))*255;
     Serial.println(mixVal);    // Output for debug
     
     return mixVal;
}
