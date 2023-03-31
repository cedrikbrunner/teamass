//-----Libraries-----
#include <CheapStepper.h> //not sure if we still plan to use this library

//-----Pins-----
//put variables that control Arduino pins here
int sensor1_pin = 2;
int sensor2_pin = 3;
CheapStepper stepper (8,9,10,11);

//-----Variables-----
//put other variables here
long stepper_steps_total = 0;
long board_length_steps = 0;

//-----Setup-----
void setup() {
  Serial.begin(9600) //this sets up the Serial Monitor for use later in the code
}

//-----Main Loop-----
void loop() {
  read_sensor(1); //calls function ans passes it value of one, this 
}

//-----Functions------
int read_sensor(int sensor_num) { //
  Serial.print(sensor_num);
}