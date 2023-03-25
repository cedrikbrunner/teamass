int Sensor = 1;
int Sensor2 = 4;

#include <CheapStepper.h>
CheapStepper stepper (8,9,10,11);  
// here we declare our stepper using default pins:
// arduino pin <--> pins on ULN2003 board:
// 8 <--> IN1
// 9 <--> IN2
// 10 <--> IN3
// 11 <--> IN4

 // let's create a boolean variable to save the direction of our rotation




void setup() {
  
  // let's just set up a serial connection and test print to the console
  stepper.setRpm(17);
  Serial.begin(9600);
  Serial.println("Ready to start moving!");
  pinMode(Sensor2, INPUT);
}

void loop() {

  // let's move a full rotation (4096 mini-steps)
  // we'll go step-by-step using the step() function
  if (Sensor == 1){
    
    stepper.stepCW();
    //you could also say stepper.stepCW(); or stepper.stepCCW();
    // now let's get the current step position of motor

    int nStep = 0;
    
 
    while (digitalRead(Sensor2)==LOW) { 

      // let's print the position to the console
      int nStep =+ stepper.getStep();
      Serial.print("current step position: "); Serial.print(nStep);
      Serial.println();
      
    }
}
}

