#include <CheapStepper.h>
int sensor=0;

// next, declare the stepper
// and connect pins 8,9,10,11 to IN1,IN2,IN3,IN4 on ULN2003 board

CheapStepper stepper (8,9,10,11);  


 // let's also create a boolean variable to save the direction of our rotation
 // and a timer variable to keep track of move times

bool moveClockwise = false;


void setup() {

  // let's run the stepper at 12rpm (if using 5V power) - the default is ~16 rpm

  stepper.setRpm(12);

  // let's print out the RPM to make sure the setting worked
  
  Serial.begin(9600);
  Serial.print("stepper RPM: "); Serial.print(stepper.getRpm());
  Serial.println();

  // now let's set up our first move...
  // let's move a half rotation from the start point

  
  /* this is the same as: 
   * stepper.newMoveToDegree(clockwise, 180);
   * because there are 4096 (default) steps in a full rotation
   */
 stepper.newMove(moveClockwise,1);
}

void loop() {

  // we need to call run() during loop() 
  // in order to keep the stepper moving
  // if we are using non-blocking moves
 
  stepper.run();
 
  
  // let's check how many steps are left in the current move:
  int stepsLeft = stepper.getStepsLeft();
  // if the current move is done...
  if (stepsLeft == 0){
  Serial.println("Da bini");
    // let's print the position of the stepper to serial
    Serial.print("stepper position: "); Serial.print(stepper.getStep());
    Serial.println();
    // let's start a new move 
    stepper.newMove (moveClockwise, 100); // move 180 degrees from current position
    if (sensor = 1){
        Serial.println("bini ready!");
    }
   

  }
}