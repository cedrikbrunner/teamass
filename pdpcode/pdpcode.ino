#include <CheapStepper.h>
long steps = 0; //long for 32 bits
int n=100; // number of steps each intervall
int sensor = 2; //sensor at pin X
int s1 = 0;
long savesteps = 0;

CheapStepper stepper (8,9,10,11);  //pins for driver IN1, IN2, IN3, IN4
boolean moveClockwise = true; // false for CCW

//--------------------

void setup() {  
  Serial.begin(9600);
  Serial.println("Ready to start moving!");
  stepper.setRpm(17); //works fine untill 17, after that not enough torque to move
  pinMode(sensor, INPUT); //define the sensor pin as an INPUT
}

//------------------

void loop() {
  readsensor();
  steps = 0; //befor it enters the while loop always set steps back to 0
  Serial.println(steps);
  
  while (s1 == HIGH){ //while the sensor is high the stepper moves
    steppermove(); // n step at the time
    readsensor(); //then read sensor again
    if (steps <= 2000) { //just for checking if it works
      print();
    }
    if (steps >= 6000) {
      print();
    }
  }
  stepperstop(); //sensor LOW stepper stops
  savesteps += steps; // and write the steps which it drove into savesteps
  Serial.print("saved steps:"); Serial.println(savesteps);
}

//------------------

void steppermove () {

  for (int s=0; s<n; s++){ //n number of steps in the loop, we have to check for accuracy 
    stepper.step(moveClockwise);
    steps += 1;
    }
}
void stepperstop () {

  for (int s=0; s<0; s++){ // 0 steps = stopp :D
    stepper.step(moveClockwise);
    }
}
void print() {
  Serial.println(steps); 
}

void readsensor() { //reading sensor value and give it to s1 (HIGH or LOW)
  s1 = digitalRead (sensor); 
  Serial.print("Sensor: "); Serial.println(s1);
}
