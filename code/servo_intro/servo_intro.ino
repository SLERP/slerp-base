/*
*  Based off arduino example code "sweeper".
*
*  This was written for use with the tutorial at:
*  https://github.com/SLERP/slerp-base/wiki/Introduction-to-the-Servo
*
*  It will continue to rotate the servo (via PWM) until the voltage on A0 goes
*  below the threshold. Using a digital input, the sequence will be reset
*
*/

#include <Servo.h>        // file that tells servo_intro code about servos
 
Servo myservo;            // create servo object to control w/ arduino commands

/*
*  Servo Behavior (Parallax Continuous Rotation)
*
*  Use myservo.writeMicroseconds(uS) to enact motion
*  
*      uS  |  1300  ->  1500  |  1509  ->  1516  |  1520  ->  1700
*    Speed |  HIGH      LOW   |    STOPPED       |  LOW       HIGH
* Direction|    Clockwise     |                  |    Counter-CW
*
*/

//Pin Configuration
int led = 8;
int button = 2;
int PWMsignal = 9;

// Some state variables
bool intrcv = HIGH;                       //flag for interrupt
int velocity = 1500;                      //slow, CW movement
int onerot = 9000;                        //time for full rotation
int STOP = 1510;                          //stops servo
int lighthreshold = 180;                  //based on ambient lighting conditions

// the setup routine runs once when you press reset 
void setup() 
{ 
  myservo.attach(PWMsignal, 1300, 1700);      // designates the pin for the servo &
                                              // provides the min & max for PWM signal
                                          
  pinMode(led, OUTPUT);                       // set LED pin to power LED
  digitalWrite(button, HIGH);                 // turn on pullup resistors
  attachInterrupt(0, inthandler, FALLING);    //set button to interrupt
  Serial.begin(9600);                         // Serial output for debug
}

// the loop routine runs over and over again forever; 
void loop () 
{
  //This conditional statement will be true if the button is pressed
  if (intrcv)
  {
    digitalWrite(led, HIGH);                    //Turn LED on
    delay(1500);                                //Wait 1.5secs for LED
    
    //rotate for 2 seconds to clear block from light path
    myservo.writeMicroseconds(velocity);        //command to rotate motor (see note)
    delay(2000);                                //rotate for 2 seconds
    intrcv = LOW;
  }

  int photoValue = analogRead(A0);              //Check photodiode brightness
  
  //format photoValue
  photoValue = (float(photoValue)/100) * 255;
  if (photoValue > 255) {
   // 2^8 = 256
   photoValue = 255;
  }
  if (photoValue < 0 )  {
    photoValue = 0;
  }
  Serial.println(photoValue);                  //Output brightness for debug
  
  //This conditional statement will be true if the light path is covered
  if (photoValue < lighthreshold)
  {
    myservo.writeMicroseconds(STOP);           //stops servo
    digitalWrite(led, LOW);                    //turns off LED
   }
  //If the light path is not covered, rotate the motor
  else
  {
    myservo.writeMicroseconds(velocity);       // command to rotate motor (see note)
  }
}

//  the interrupt service routine will run when the button is pressed:
void inthandler()
{
  intrcv = HIGH;                               //set interrupt flag high
}
