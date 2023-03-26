#include <CheapStepper.h>


CheapStepper stepper (8,9,10,11); 
boolean moveClockwise = true; // false for CCW

//--------------------

void setup() {  
  Serial.begin(9600);
  Serial.println("Ready to start moving!");
  stepper.setRpm(17);
}

//------------------

void loop() {
  steppermove();
  print();
  
}

//------------------

void steppermove () {

  for (int s=0; s<100; s++){
    stepper.step(moveClockwise);
    }
}

void print() {
  int nStep = stepper.getStep();
  Serial.println(nStep);
}
