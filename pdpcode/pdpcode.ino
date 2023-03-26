#include <CheapStepper.h>
long steps = 0; //long for 32 bits
int n=100; // number of steps each intervall
int sensor = 2; //sensor at pin X
int s1 = 0;
int savesteps = 0;

CheapStepper stepper (8,9,10,11); 
boolean moveClockwise = true; // false for CCW

//--------------------

void setup() {  
  Serial.begin(9600);
  Serial.println("Ready to start moving!");
  stepper.setRpm(17); //works fine untill 17, after that not enough torque to move
  pinMode(sensor, INPUT);
}

//------------------

void loop() {
  readsensor();
  steps = 0;
  Serial.println(steps);
  
  while (s1 == HIGH){
    steppermove();
    readsensor();
    if (steps <= 2000) {
      print();
    }
    if (steps >= 6000) {
      print();
    }
  }
  stepperstop();
  savesteps += steps;
  Serial.print("saved steps:"); Serial.println(savesteps);
}

//------------------

void steppermove () {

  for (int s=0; s<n; s++){
    stepper.step(moveClockwise);
    steps += 1;
    }
}
void stepperstop () {

  for (int s=0; s<0; s++){
    stepper.step(moveClockwise);
    }
}
void print() {
  Serial.println(steps);
}

void readsensor() {
  s1 = digitalRead (sensor);
  Serial.print("Sensor: "); Serial.println(s1);
}
