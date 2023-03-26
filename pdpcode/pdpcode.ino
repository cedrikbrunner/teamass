#include <CheapStepper.h>
int steps = 0;
int n=4000; // number of steps each intervall

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

  for (int s=0; s<n; s++){
    stepper.step(moveClockwise);
    steps ++;
    }
}

void print() {
  Serial.println(steps);
}

void countingsteps () {

}