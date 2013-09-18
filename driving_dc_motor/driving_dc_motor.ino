/*
*
*  This was written for use with the tutorial at:
*  https://github.com/SLERP/slerp-base/wiki/Driving-a-DC-Motor
*  
*  Once int.0 (an interrupt on PIN2) recognizes a falling edge (transition from HIGH to LOW),
*  boolean intrcv is set HIGH. This flag will send the necessary signals (IN1 & IN2) to the
*  driver, which will cause the motor to rotate each direction for five seconds.
*
*/

/* The motor driver performs with the following logic
*
*    IN1  |  IN2  |  OUT1  |  OUT2  |  Movement
*--------------------------------------------------
*    LOW  |  LOW  |  OFF   |  OFF   |    STOP
*    HIGH |  LOW  |  HIGH  |  LOW   |    CW/CCW
*    LOW  |  HIGH |  LOW   |  HIGH  |    CCW/CW
*    HIGH |  HIGH |  LOW   |  LOW   |    BRAKE
*
*/

// We name our pins with something sensible
int button = 2;
int IN1 = 12;
int IN2 = 13;

// Some state variables
bool intrcv;                                  //flag for interrupt
int timerotate = 5000;                        //5 second delay
int timedelay = 1000;                         //1 second delay

// the setup routine runs once when you press reset:
void setup() {
  pinMode(IN1, OUTPUT);                       //set IN1 to output
  pinMode(IN2, OUTPUT);                       //set IN2 to output
  digitalWrite(IN1, LOW);                     //set IN1 & IN2 LOW
  digitalWrite(IN2, LOW);                     //so motor is stopped
  digitalWrite(button, HIGH);                 //turn on pullup resisitors
  attachInterrupt(0, inthandler, FALLING);    //set button to interrupt
}

// the loop routine runs over and over again forever:
void loop() {
  
  //This conditional statment will only be true if an interrupt is recieved 
  if(intrcv)
  {
    digitalWrite(IN1, HIGH);                  //Set IN1 HIGH, motor rotate
    delay(timerotate);                        //rotate for set time
    digitalWrite(IN2, HIGH);                  //Set IN2 HIGH, motor BRAKE
    delay(timedelay);                         //settling time
  
    digitalWrite(IN1, LOW);                   //Set IN1 LOW, motor rotate opposite
    delay(timerotate);                        //rotate for set time
    digitalWrite(IN2, LOW);                   //Set IN2 LOW, motor STOP
    delay(timedelay);                         //settling time
    
    intrcv = LOW;                             //interrupt flag set low
  }
}

//  the interrupt service routine will run when the button is pressed:
void inthandler()
{
  intrcv = HIGH;                              //interrupt flag set high
}
