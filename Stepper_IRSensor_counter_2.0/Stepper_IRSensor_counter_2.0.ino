#include <AccelStepper.h>

// 1 full rotation = 200 Steps

const int stepPin = 2;        // Yellow driver cable
const int dirPin = 3;         // Blue driver cable
const int sensor1Pin = 7;     // Pin for IR Sensor 1
const int sensor2Pin = 8;     // Pin for IR Sensor 2

// n is the stepsize for one step
int n = 5;                    // if n = 1, every single step is printed (massive lag) 
// if n = 5, every fifth step is printed a lot less lag 

int steps = 0; // variable for measuring steps, actual amount of steps = steps*n

int mminsteps = 10; // 1 mm = 10 steps (needs to be determined)
int sensordistancemm = 100; // distance between the sensors in mm

int totaldistancemm = 450; // distance in mm from sensor 1 to where middle of board needs to be 

bool motorMoved = false; // Variable to save motorposition so that stepsToMove are moved only once


AccelStepper feedinstepper(1, stepPin, dirPin);  // definition of first stepper motor 


// --------------------------------------------- Setup 

void setup() {
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  Serial.begin(9600);
  feedinstepper.setMaxSpeed(1000);  // Sets max speed for stepper motor, needed for the library
  feedinstepper.setAcceleration(0); // if 0 Acceleration is deactivated because it causes inaccuracy but the line is needed for the library
  
}


// --------------------------------------------- Loop 


void loop() {
  // If first sensor senses board the motor starts rolling 
  while (digitalRead(sensor1Pin) == LOW && digitalRead(sensor2Pin) == HIGH ) {
    
    steppermove();
    motorMoved = false;  // is needed for tests so we can reset the motorposition without having to reupload
  }


  
  // Both sensors LOW, starts to count steps 
  int steps = 0; // first steps need to be reset 
  while (digitalRead(sensor1Pin) == LOW && digitalRead(sensor2Pin) == LOW) {
 
    steppermove();
    steps++;

    Serial.print("Amount of steps: "); // steps actually don't need to be printed 
    Serial.println(steps);           // if more accuracy is needed decrease n and dont print every step 

  
  }  


  // this is actually the calculation, it kind of needs to be here   

  int sensordistance = sensordistancemm * mminsteps; // distance between the sensors in steps
  int totaldistance = totaldistancemm * mminsteps; // total distance in steps

  int boardlength = steps * n + sensordistance; // boardlength in steps
  int boardlengthmm = boardlength / mminsteps; // boardlength in mm to print 

  int stepsToMove = totaldistance - boardlength / 2;  // calculated amount of steps that need to be taken



// as soon as sensor 1 looses contact, length measurement is done, steps to move is being calculated and the motor moves the amount of steps 

  if (digitalRead(sensor1Pin) == HIGH && digitalRead(sensor2Pin) == LOW && !motorMoved) {  // s1 no board s2 board and motor did not already move 
    feedinstepper.move(-stepsToMove); // needs to be negative for the right direction
    steppermovesteps (); 
    Serial.print("StepsToMove = ");
    Serial.print(stepsToMove);
    
    motorMoved = true; //important so that motor only moves this distance once 
  }

}

// --------------------------------------------- Functions


void steppermove () { //n number of steps in the loop, we have to check for accuracy 

  for(int x = 0; x < n; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(2000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(2000);
  }
}

void stepperstop () { //n number of steps in the loop, we have to check for accuracy 

  for(int x = 0; x < 0; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(1000);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(1000);
  }

}



void steppermovesteps() {  // stepper moves the calculated amount of steps, saved in "stepsToMove" to align the board

  while (feedinstepper.distanceToGo() != 0) {
    feedinstepper.setSpeed(250);
    feedinstepper.runSpeedToPosition();
  } 


}
