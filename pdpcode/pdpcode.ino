#include <CheapStepper.h>
long steps = 0; //long for 32 bits
int n=100; // number of steps each intervall

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
    steps += 1;
    }
}

void print() {
  Serial.println(steps);
}
