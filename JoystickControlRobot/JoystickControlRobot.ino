/*+++++++++++++++++++++++++++++++++++++++++++++++++++
Author      : Fahmi Ghani
Date        : 11 July 2015 
Project     : Joystick control differential drive dc motor robot
Component   : Analog Joystick
              2Amp motor driver shield
              DC Motor
Description : Control DC motor direction using Joystick
Video :https://www.youtube.com/watch?v=kfT3eoNAM-Q
+++++++++++++++++++++++++++++++++++++++++++++++++++*/

//Set pin numbers:
const byte joyStickYPin = A2;
const byte joyStickXPin = A1;

//This pin is depend on what motor driver you use, and how you wired it.
const byte motorLSpeedPin = 5; //Left Speed Pin
const byte motorLDirPin = 4;   //Left Direction Pin
const byte motorRSpeedPin = 6; //Right Speed Pin
const byte motorRDirPin = 7;   //Right Direction Pin

//variables
//Joystick input variables
int joyXValue = 0;
int joyYValue = 0;
int joyValueMax = 2000;
int joyValueMin = 1000;
int joyValueMid = 1500; //Joystick value when in default position.
int joyValueMidUpper = joyValueMid + 100; //Add +-100 as joystick deadzone
int joyValueMidLower = joyValueMid - 100;
//The "joystick deadzone" is the area around the center of a joystick that does not respond to movement. 
//It is designed to cut down on accidental movement caused by "jitter"

//DC motor variables
int speedFwd = 0;
int speedTurn = 0;
int speedLeft = 0;
int speedRight = 0;

byte motorSpeed = 0;	
byte motorSpeedMax = 255; //Maximum Speed
byte motorSpeedMin = 90; //set to smallest value that make motor move (default 0)
                         // DC motor that I use start to move at 90 pwm value

void setup() 
{
    Serial.begin(9600);
    Serial.print("START" );
     
    pinMode(joyStickXPin, INPUT);
    pinMode(joyStickYPin, INPUT);
    pinMode(motorLSpeedPin, OUTPUT);
    pinMode(motorLDirPin, OUTPUT);
    pinMode(motorRSpeedPin, OUTPUT);
    pinMode(motorRDirPin, OUTPUT);
}

void loop() 
{
   // joyXValue = analogRead(joyStickXPin); //Turn
   // joyYValue = analogRead(joyStickYPin); //Forward/backward
    
    //Read PWM value form receiver
    joyXValue = pulseIn(joyStickXPin,HIGH); 
    joyYValue = pulseIn(joyStickYPin,HIGH);
    
    //Map joystick value to speed value
    if(joyYValue > joyValueMidUpper)//forward
    {
        speedFwd = map(joyYValue, joyValueMidUpper, joyValueMax, motorSpeedMin, motorSpeedMax);
    }
    else if(joyYValue < joyValueMidLower) //backward
    {
        speedFwd = map(joyYValue, joyValueMidLower, joyValueMin, -motorSpeedMin, -motorSpeedMax);
    }
    else 
    {
       speedFwd =0;
    }
    
    if(joyXValue > joyValueMidUpper) //right
    {
        speedTurn = map(joyXValue, joyValueMidUpper, joyValueMax, motorSpeedMin, motorSpeedMax);
    }
    else if(joyXValue < joyValueMidLower) //left
    {
        speedTurn = map(joyXValue, joyValueMidLower, joyValueMin, -motorSpeedMin, -motorSpeedMax);
    }
    else 
    {
       speedTurn =0;
    }

    //Convert Forward speed and Turn Speed to Motor left and Motor Right speed
    speedLeft = speedFwd + speedTurn;
    speedRight = speedFwd - speedTurn;

    speedLeft = constrain(speedLeft, -255, 255);
    speedRight = constrain(speedRight, -255, 255);
    
    //Send the Motor speed value to Motor Driver
    MoveRobot(speedLeft,speedRight);
  
    //Display the value in Serial Monitor.
    Serial.print(speedFwd);
    Serial.print("\t" );
    Serial.print(speedTurn);
    Serial.print("\t" );
    Serial.print(speedLeft);
    Serial.print("\t" );
    Serial.print(speedRight);
    Serial.println(" ");
    
    delay(100);

}

void MoveRobot(int spdL, int spdR) 
{
     //Set Motor Direction
     if(spdL>0)
     {
        digitalWrite(motorLDirPin, HIGH);
     }
     else
     {
        digitalWrite(motorLDirPin, LOW);
     }
     if(spdR>0)
     {
        digitalWrite(motorRDirPin, HIGH);
     }
     else
     {
        digitalWrite(motorRDirPin, LOW);
     }
     
     //Set Motor Speed
     analogWrite(motorLSpeedPin, abs(spdL));
     analogWrite(motorRSpeedPin, abs(spdR));     
}