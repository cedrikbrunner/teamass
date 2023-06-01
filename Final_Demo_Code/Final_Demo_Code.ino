// ---------- Libraries ----------
//#include <AccelStepper.h>


// ---------- Other Info ----------
// 1 full rotation = 200 Steps


// ---------- Pins ----------
const int stepPin = 2;        // Yellow driver cable
const int dirPin = 3;         // Blue driver cable
const int relayC2 = 6;        // Pin for relay pneumatic cylinder C1
const int relayC1 = 5;        // Pin for relay pneumatic cylinder C2
const int relayC3 = 4;        // Pin for relay pneumatic cylinder C3
const int S1 = 7;             // Pin for IR Sensor 1
const int S2 = 8;             // Pin for IR Sensor 2 printing section
const int S9 = 9;             // Pin for IR Sensor 3 back
const int S10 = 10;           // Pin for IR Sensor 4 front
const int S3 = 32;            // Pin for pneumatic cylinder C1 sensor 3
const int S4 = 36;            // Pin for pneumatic cylinder C1 sensor 4
const int S5 = 40;            // Pin for pneumatic cylinder C2 sensor 5
const int S6 = 44;            // Pin for pneumatic cylinder C2 sensor 6
const int S7 = 48;            // Pin for pneumatic cylinder C3 sensor 7
const int S8 = 52;            // Pin for pneumatic cylinder C3 sensor 8
const int relayfan = 28;      // Pin for relay to start/stop the fan
const int modeswitch = 26 ;    // Switch to change between neutral mode and print mode



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
const int printtimeC1 = 500;
const int printtimeC3 = 5000;
bool readytoprint = false; 
bool printfinished = false;
int stepsToDry = 700;
int dryTime = 15000; 
int stepsToEject = 400;
//AccelStepper feedinstepper(1, stepPin, dirPin);  // definition of first stepper motor 


// ----------Setup ---------- 
void setup() {
  pinMode(S1, INPUT); // IR sensor 1
  pinMode(S2, INPUT); // IR sensor 2
  pinMode(S9, INPUT); // IR sensor 3
  pinMode(S10, INPUT); // IR sensor 4
  pinMode(S3, INPUT); // C1 sensor at retracted position
  pinMode(S4, INPUT); // C1 sensor at expanded position
  pinMode(S5, INPUT); // C2 sensor at retracted position
  pinMode(S6, INPUT); // C2 sensor at expanded position
  pinMode(S7, INPUT); // C3 sensor at retracted position
  pinMode(S8, INPUT); // C3 sensor at expanded position
  pinMode(modeswitch, INPUT_PULLUP);
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
int var = digitalRead(modeswitch);
Serial.print("Switch is:"); Serial.println(var);
if (digitalRead(modeswitch) == HIGH){
  while (digitalRead(S10) == LOW) { // If first sensor senses board the motor starts rolling
    steppermove();
  }
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
      digitalWrite(relayC2,HIGH);
      delay(1300);
      digitalWrite(relayC2, LOW);
      delay(1200);
    }
    if (digitalRead(S8)==LOW && digitalRead(S3)==LOW) { //C3 expanded and Swiper retracted
      digitalWrite(relayC2,HIGH); //Alu expand
      delay(printtimeC1);
    }
    if (digitalRead(S6)==LOW){ //if alu expanded
      digitalWrite(relayC3,LOW); //C3 retracted -> color distribution
      delay(1400);
      digitalWrite(relayC2,LOW);
      delay(1200);
    }
    if (digitalRead(S7)==LOW){ //C3 retracted
      digitalWrite(relayC2,LOW); //Alu retracting
      digitalWrite(relayC1,HIGH); //Swiper expanding
      delay(printtimeC1);
    }
    if(digitalRead(S4)==LOW && digitalRead(S5) == LOW){ //if alu retracted and swiper expanded
      digitalWrite(relayC3,HIGH); //C3 expanding => screenprinting
      delay(2500);
      digitalWrite(relayC1,LOW);
      delay(1500);
    }
    if (digitalRead(S8)==LOW){ //C3 expanded
      digitalWrite(relayC1,LOW); //swiper retracting
      delay(printtimeC1);
    }
    if (digitalRead(S3) == LOW && digitalRead(S5) == LOW){ //alu and swiper retracted
      digitalWrite(relayC3, LOW); //C3 retracting
      delay(2500);
    }
    
  } // ----- finish printing
  printfinished = true;
  while (digitalRead(S3) == LOW && digitalRead(S5) == LOW && digitalRead(S7) == LOW && digitalRead(S9) == HIGH){
   for(int x = 0; x < stepsToDry; x++) {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(stepperspeed);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(stepperspeed);
            }
    digitalWrite(relayfan, HIGH);
    delay(dryTime);
    digitalWrite(relayfan, LOW);
    while(digitalRead(S9) == HIGH){
      steppermove();
      }
    while(digitalRead(S9)==LOW){
    for(int x = 0; x < stepsToEject; x++) {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(stepperspeed);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(stepperspeed);
            }
            break;
    }
    break;
  }

}
}
else{
  if(digitalRead(S10)==LOW){
    while (digitalRead(S9) == HIGH){
      steppermove();
      
    }

    while(digitalRead(S9)==LOW){
    for(int x = 0; x < stepsToEject; x++) {
              digitalWrite(stepPin, HIGH);
              delayMicroseconds(stepperspeed);
              digitalWrite(stepPin, LOW);
              delayMicroseconds(stepperspeed);
            }      
     break;
    }
   
   
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

  }
}