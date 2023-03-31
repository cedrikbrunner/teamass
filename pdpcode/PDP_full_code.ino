//-----Libraries-----
#include <CheapStepper.h> //not sure if we still plan to use this library

//-----Pins-----
//put variables that control Arduino pins here
int sensor1_pin = 7;
int sensor2_pin = 8;

//-----Variables-----
//put other variables here
int sensor1_val = 0;
int sensor2_val = 0;
int read_sensor_output = 0;
long stepper_steps_total = 0;
long board_length_steps = 0;

//-----Setup-----
void setup() {
  Serial.begin(9600); //this sets up the Serial Monitor for use later in the code
  pinMode (sensor1_pin, INPUT); //sets the pins for the sensors as input pins
  pinMode (sensor2_pin, INPUT);
}

//-----Main Loop-----
void loop() {
  sensor1_val = read_sensor(sensor1_pin); //gets value from sensor 1
  sensor2_val = read_sensor(sensor2_pin);
}

//-----Functions------
int read_sensor(int sensor_pin) { //takes integer input of sensor number and checks that sensor
  read_sensor_output = digitalRead(sensor_pin);
  return read_sensor_output; //return a 1 or 0, 0 means something is detected and 1 means that nothing is detected
}