// ---------- Libraries ----------
//#include <AccelStepper.h>


// ---------- Other Info ----------
// 1 full rotation = 200 Steps


// ---------- Pins ----------
const int stepPin = 2;        // Yellow driver cable
const int dirPin = 3;         // Blue driver cable
const int relayC1 = 6;        // Pin for relay pneumatic cylinder C1
const int relayC2 = 5;        // Pin for relay pneumatic cylinder C2
const int relayC3 = 4;        // Pin for relay pneumatic cylinder C3
const int S1 = 7;             // Pin for IR Sensor 1
const int S2 = 8;             // Pin for IR Sensor 2
const int S9 = 9;             // Pin for IR Sensor 3
const int S3 = 32;            // Pin for pneumatic cylinder C1 sensor 3
const int S4 = 36;            // Pin for pneumatic cylinder C1 sensor 4
const int S5 = 40;            // Pin for pneumatic cylinder C2 sensor 5
const int S6 = 44;            // Pin for pneumatic cylinder C2 sensor 6
const int S7 = 48;            // Pin for pneumatic cylinder C3 sensor 7
const int S8 = 52;            // Pin for pneumatic cylinder C3 sensor 8
const int relayfan = 13;      // Pin for relay to start/stop the fan



// ---------- Variables ----------
int n = 5; // n is the stepsize for one step. If n = 1, every single step is printed (massive lag). If n = 5, every fifth step is printed a lot less lag
int stepperspeed = 2000;
int steps = 0; // variable for measuring steps, actual amount of steps = steps*n
float mminsteps = 2; // 1 mm = 10 steps (needs to be determined) 1.587
int sensordistancemm = 225; // distance between the sensors in mm
int totaldistancemm = 364; // distance in mm from sensor 1 to where middle of board needs to be
int correction = 62; 
bool motorMoved = false; // Variable to save motorposition so that stepsToMove are moved only once
int state_tracker = 0; //This variable will track the current state to avoid errors (NOT USED CURRENTLY)
int stepsToMove;
const int numberofprints = 1;
const int printwaittime = 3000;
bool readytoprint = false; 
bool printfinished = false;
//AccelStepper feedinstepper(1, stepPin, dirPin);  // definition of first stepper motor 


// ----------Setup ---------- 
void setup() {
  pinMode(S1, INPUT); // IR sensor 1
  pinMode(S2, INPUT); // IR sensor 2
  pinMode(S3, INPUT); // C1 sensor at retracted position
  pinMode(S4, INPUT); // C1 sensor at expanded position
  pinMode(S5, INPUT); // C2 sensor at retracted position
  pinMode(S6, INPUT); // C2 sensor at expanded position
  pinMode(S7, INPUT); // C3 sensor at retracted position
  pinMode(S8, INPUT); // C3 sensor at expanded position
  pinMode(stepPin,OUTPUT);
  pinMode(dirPin,OUTPUT);
  pinMode(relayC1,OUTPUT);
  pinMode(relayC2,OUTPUT);
  pinMode(relayC3,OUTPUT);
  pinMode(relayfan,OUTPUT);
  
  Serial.begin(9600);
  //feedinstepper.setMaxSpeed(1000);  // Sets max speed for stepper motor, needed for the library
  //feedinstepper.setAcceleration(0); // if 0 Acceleration is deactivated because it causes inaccuracy but the line is needed for the library
}


// ----------Main Loop ---------- 
void loop() {
  
  while (digitalRead(S1) == LOW && digitalRead(S2) == HIGH) { // If first sensor senses board the motor starts rolling
    steppermove();
      motorMoved = false; // s needed for tests so we can reset the motorposition without having to reupload
    readytoprint = false;
    printfinished = false;
  }

  int steps = 0; // first steps need to be reset 
  while (digitalRead(S1) == LOW && digitalRead(S2) == LOW) { // Both sensors LOW, starts to count steps
    steppermove();
    steps++;
    //Serial.print("Amount of steps: "); // steps actually don't need to be printed 
    //Serial.println(steps);           // if more accuracy is needed decrease n and dont print every step 
  }  

  // this is actually the calculation, it kind of needs to be here   
  int sensordistance = sensordistancemm * mminsteps; // distance between the sensors in steps
  int totaldistance = (totaldistancemm-correction) * mminsteps; // total distance in steps
  int boardlength = steps * n + sensordistance; // boardlength in steps
  int boardlengthmm = boardlength / mminsteps; // boardlength in mm to print 
  int stepsToMove = totaldistance - (boardlength /2) ;  // calculated amount of steps that need to be taken

  // as soon as sensor 1 looses contact, length measurement is done, steps to move is being calculated and the motor moves the amount of steps 
  if (digitalRead(S1) == HIGH && digitalRead(S2) == LOW && !motorMoved && motorMoved == false) {  // s1 no board s2 board and motor did not already move 
   
    Serial.print("StepsToMove = "); Serial.println(stepsToMove);
    Serial.print("Boardlength= "); Serial.println(boardlengthmm);
    if (stepsToMove > 0) {
      digitalWrite(dirPin, LOW);
        for(int x = 0; x < stepsToMove; x++) {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(stepperspeed);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(stepperspeed);
            }
    
    }

    else {
      digitalWrite(dirPin, HIGH); // set direction to backward
      for(int x = 0; x < -stepsToMove; x++) {
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepperspeed);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepperspeed);
      }
    }
    readytoprint = true;
    motorMoved = true; //important so that motor only moves this distance once 
    Serial.println(readytoprint);
  }
//print section
if (readytoprint == true && digitalRead(S2) == LOW){
  for(int x = 0; x < numberofprints; x++) {
    Serial.println("ready to print");
    
    if (digitalRead(S3) == LOW && digitalRead(S5) == LOW){ //alu and swiper retracted
      Serial.println("expand C3");
      digitalWrite(relayC3,HIGH); //C3 expanding
      delay(printwaittime);
    }
    if (digitalRead(S8)==LOW && digitalRead(S3)==LOW) { //C3 expanded and Swiper retracted
      digitalWrite(relayC2,HIGH); //Alu expand
      delay(printwaittime);
    }
    if (digitalRead(S6)==LOW){ //if alu expanded
      digitalWrite(relayC3,LOW); //C3 retracted -> color distribution
      delay(printwaittime);
    }
    if (digitalRead(S7)==LOW){ //C3 retracted
      digitalWrite(relayC2,LOW); //Alu retracting
      digitalWrite(relayC1,HIGH); //Swiper expanding
      delay(printwaittime);
    }
    if(digitalRead(S4)==LOW && digitalRead(S5) == LOW){ //if alu retracted and swiper expanded
      digitalWrite(relayC3,HIGH); //C3 expanding => screenprinting
      delay(printwaittime);
    }
    if (digitalRead(S8)==LOW){ //C3 expanded
      digitalWrite(relayC1,LOW); //swiper retracting
      delay(printwaittime);
    }
    if (digitalRead(S3) == LOW && digitalRead(S5) == LOW){ //alu and swiper retracted
      digitalWrite(relayC3, LOW); //C3 retracting
      delay(printwaittime);
    }
  } // ----- finish printing
  
  printfinished = true;
  while (digitalRead(S3) == LOW && digitalRead(S5) == LOW && digitalRead(S7) == LOW && digitalRead(S8) == HIGH){
   steppermove();
  }
 
}
 
}


//----------Functions ----------
void steppermove () { //n number of steps in the loop, we have to check for accuracy
  for(int x = 0; x < n; x++) {
    digitalWrite(dirPin, LOW);
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
  digitalWrite(dirPin, HIGH);
  for(int x = 0; x < stepsToMove; x++) {
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepperspeed);
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepperspeed);
      }
}

void steppermovestepsback() {  // stepper moves the calculated amount of steps, saved in "stepsToMove" to align the board
  digitalWrite(dirPin, HIGH); // set direction to backward
      for(int x = 0; x < -stepsToMove; x++) {
        digitalWrite(stepPin, LOW);
        delayMicroseconds(stepperspeed);
        digitalWrite(stepPin, HIGH);
        delayMicroseconds(stepperspeed);
      }
}


int extend_cylinder() {
  if (digitalRead(S3) == HIGH){
  digitalWrite(relayC1, HIGH);
  }
}


int retract_cylinder() {
  if (digitalRead(S4) == HIGH){
  digitalWrite(relayC1, LOW);
  
}}
