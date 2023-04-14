#include <AccelStepper.h>
#include <Adafruit_GFX.h>
#include <Adafruit_SSD1306.h>

#define SCREEN_WIDTH 128 /*OLED screen width in pixels*/
#define SCREEN_HEIGHT 64 /*OLED screen height in pixels*/
Adafruit_SSD1306 oled(SCREEN_WIDTH, SCREEN_HEIGHT, &Wire, -1);

// 1 full rotation = 200 Steps

const int stepPin = 2;        // Yellow driver cable
const int dirPin = 3;         // Blue driver cable
const int relayC1 = 4;        // Pin for relay pneumatic cylinder C1
const int relayC2 = 5;        // Pin for relay pneumatic cylinder C2
const int relayC3 = 6;        // Pin for relay pneumatic cylinder C3
const int S1 = 7;             // Pin for IR Sensor 1
const int S2 = 8;             // Pin for IR Sensor 2
const int S3 = 9;             // Pin for pneumatic cylinder C1 sensor 3
const int S4 = 10;            // Pin for pneumatic cylinder C1 sensor 4
const int S5 = 11;            // Pin for pneumatic cylinder C2 sensor 5
const int S6 = 10;            // Pin for pneumatic cylinder C2 sensor 6
const int S7 = 11;            // Pin for pneumatic cylinder C3 sensor 7
const int S8 = 12;            // Pin for pneumatic cylinder C3 sensor 8
const int relayfan = 13;      // Pin for relay to start/stop the fan

// n is the stepsize for one step
int n = 5;                    
// if n = 1, every single step is printed (massive lag) 
// if n = 5, every fifth step is printed a lot less lag 

int stepperspeed = 2000;

int steps = 0; // variable for measuring steps, actual amount of steps = steps*n

float mminsteps = 1.587; // 1 mm = 10 steps (needs to be determined)
int sensordistancemm = 100; // distance between the sensors in mm

int totaldistancemm = 450; // distance in mm from sensor 1 to where middle of board needs to be 

bool motorMoved = false; // Variable to save motorposition so that stepsToMove are moved only once


AccelStepper feedinstepper(1, stepPin, dirPin);  // definition of first stepper motor 


// --------------------------------------------- Setup 

void setup() {
  pinMode(S1, INPUT);
  pinMode(S2, INPUT);
  pinMode(S3, INPUT);
  pinMode(S4, INPUT);
  pinMode(S5, INPUT);
  pinMode(S6, INPUT);
  pinMode(S7, INPUT);
  pinMode(S8, INPUT);
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(relayC1,OUTPUT);
  pinMode(relayC2,OUTPUT);
  pinMode(relayC3,OUTPUT);
  pinMode(relayfan,OUTPUT);
  
  //Serial.begin(9600);
  feedinstepper.setMaxSpeed(1000);  // Sets max speed for stepper motor, needed for the library
  feedinstepper.setAcceleration(0); // if 0 Acceleration is deactivated because it causes inaccuracy but the line is needed for the library
    Serial.begin(115200);  /*Baud rate for Serial Communication*/

  if(!oled.begin(SSD1306_SWITCHCAPVCC, 0x3C)) { // I2C Address 0x3C for 128x64 OLED screen
    Serial.println(F("SSD1306 allocation failed"));
    for(;;);
  }
  delay(2000);  /*delay of 2 sec*/
  oled.clearDisplay();  /*clears the display*/

  oled.setTextSize(2);  /*text size 1*/
  oled.setTextColor(WHITE); /*text color WHITE*/
  oled.setCursor(0, 10);  /*X and Y coordinates on OLED display screen*/
  oled.println("Let's Fucking go");  /*Prints text with size 1*/
  oled.display(); /*display on OLED screen*/
}


// --------------------------------------------- Loop 


void loop() {
  // If first sensor senses board the motor starts rolling 
  while (digitalRead(S1) == LOW && digitalRead(S2) == HIGH ) {
    
    steppermove();
    motorMoved = false;  // is needed for tests so we can reset the motorposition without having to reupload
  }

  oled.clearDisplay();
  oled.setCursor(0,10);
  oled.print("Measuring in progress...");
  oled.display();
  // Both sensors LOW, starts to count steps 
  int steps = 0; // first steps need to be reset 
  while (digitalRead(S1) == LOW && digitalRead(S2) == LOW) {
 
    steppermove();
    steps++;

    //Serial.print("Amount of steps: "); // steps actually don't need to be printed 
    //Serial.println(steps);           // if more accuracy is needed decrease n and dont print every step 

  
  }  


  // this is actually the calculation, it kind of needs to be here   

  int sensordistance = sensordistancemm * mminsteps; // distance between the sensors in steps
  int totaldistance = totaldistancemm * mminsteps; // total distance in steps

  int boardlength = steps * n + sensordistance; // boardlength in steps
  int boardlengthmm = boardlength / mminsteps; // boardlength in mm to print 

  int stepsToMove = totaldistance - boardlength / 2;  // calculated amount of steps that need to be taken
  oled.clearDisplay();
  oled.setCursor(0,10);
  oled.print("Boardlength: " && boardlengthmm && "mm");
  oled.display();


// as soon as sensor 1 looses contact, length measurement is done, steps to move is being calculated and the motor moves the amount of steps 

  if (digitalRead(S1) == HIGH && digitalRead(S2) == LOW && !motorMoved) {  // s1 no board s2 board and motor did not already move 
    feedinstepper.move(-stepsToMove); // needs to be negative for the right direction
    Serial.print("StepsToMove = ");
    Serial.print(stepsToMove);
    steppermovesteps (); 
    
    motorMoved = true; //important so that motor only moves this distance once 
  }

// first cylinder C1 has to move down to position S4
// now the pneumatic cylinder C3 moves from S7 to S8

}

// --------------------------------------------- Functions


void steppermove () { //n number of steps in the loop, we have to check for accuracy 

  for(int x = 0; x < n; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperspeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperspeed);
  }
}

void stepperstop () { //n number of steps in the loop, we have to check for accuracy 

  for(int x = 0; x < 0; x++) {
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(stepperspeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(stepperspeed);
  }

}



void steppermovesteps() {  // stepper moves the calculated amount of steps, saved in "stepsToMove" to align the board

  while (feedinstepper.distanceToGo() != 0) {
    feedinstepper.setSpeed(100);
    feedinstepper.runSpeedToPosition();
  } 


}

void extendC1() {
  if (digitalRead(S3) == HIGH){
  digitalWrite(relayC1, HIGH);
  }

}

void retractC1() {
if (digitalRead(S4) == HIGH){
  digitalWrite(relayC1, LOW);
}}