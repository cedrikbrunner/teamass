//-----Libraries-----

//-----Pins-----
int sensor1_pin = 7;
int sensor2_pin = 8;

//-----Variables-----
int sensor1_val = 0;
int sensor2_val = 0;
int output = 0;

//-----Setup-----
void setup() {
  Serial.begin(9600);
  pinMode (sensor1_pin, INPUT);
  pinMode (sensor2_pin, INPUT);  
}

//-----Main Loop-----
void loop() {
  sensor1_val = read_sensor(1);
  sensor2_val = read_sensor(2);
  if (sensor1_val == 1) {
    Serial.println("Obstacle at sensor 1");
  }    
  else {
    Serial.println("Nothing detected at sensor 1");
  }
  if (sensor2_val == 1) {
    Serial.println("Obstacle at sensor 2");
  }
  else {
    Serial.println("Nothing detected at sensor 2");
  }
  Serial.println("----------");
  delay(1000);
}

//-----Functions------
int read_sensor(int sensor_num) { //takes integer input of sensor number and checks that sensor
  output = digitalRead(sensor_num);
  Serial.println(sensor_num);
  Serial.println(output);
  return output;
}