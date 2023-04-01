//-----Pins-----
int sensor1_pin = 7; //creates a variable where the pin number is stored for later use
int sensor2_pin = 8;

//-----Variables-----
int sensor1_val = 0; //creates other variables that will be used in the code
int sensor2_val = 0;
int output = 0;

//-----Setup-----
void setup() {
  Serial.begin(9600); //sets up the Serial Monitor, which is used to read the sensor values
  pinMode (sensor1_pin, INPUT); //sets the pins for the IR sensors to input mode
  pinMode (sensor2_pin, INPUT);  
}

//-----Main Loop-----
void loop() {
  sensor1_val = read_sensor(sensor1_pin); //calls the "read_seensor" function to find the sensor value
  sensor2_val = read_sensor(sensor2_pin);
  if (sensor1_val == 0) { //prints in the Serial Monitor if either sensor has detected anything
    Serial.println("Obstacle at sensor 1");
  }    
  else {
    Serial.println("Nothing detected at sensor 1");
  }
  if (sensor2_val == 0) {
    Serial.println("Obstacle at sensor 2");
  }
  else {
    Serial.println("Nothing detected at sensor 2");
  }
  Serial.println("----------");
  delay(1000); //creates a delay so that the output from the sensor is actaully legible, as normally these  sensors upddate very quickly
}

//-----Functions------
int read_sensor(int sensor_pin) { //takes integer input of sensor number and checks that sensor
  output = digitalRead(sensor_pin);
  Serial.println(sensor_pin); //prints the pin of the sensor for testing purposes
  Serial.println(output); //prints that sensor's value for testing purposes
  return output;
}