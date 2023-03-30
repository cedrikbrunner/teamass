#define motorStepPin 3
#define motorDirPin 4
#define sensor1Pin A0
#define sensor2Pin A1

#include <Stepper.h>

const int stepsPerRevolution = 200;  // Change this to your motor's number of steps per revolution
Stepper myStepper(stepsPerRevolution, motorStepPin, motorDirPin);

void setup() {
  pinMode(sensor1Pin, INPUT);
  pinMode(sensor2Pin, INPUT);
  myStepper.setSpeed(50);  // Change this to the appropriate speed for your motor
}

void loop() {
  int sensor1Val = digitalRead(sensor1Pin);
  int sensor2Val = digitalRead(sensor2Pin);

  if (sensor1Val == HIGH) {
    myStepper.step(1);
    while (digitalRead(sensor1Pin) == HIGH) {}  // Wait until the object is no longer detected by sensor 1
    int steps = 1;
    while (digitalRead(sensor2Pin) == LOW) {  // Keep counting steps until object is detected by sensor 2
      myStepper.step(1);
      steps++;
    }
    float l = steps * y + x;  // Calculate value for l
    float m = (z - l/s) / y;  // Calculate value for m
    myStepper.step(m);  // Move the motor m steps
  }
}
