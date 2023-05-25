
// Input Pins
const int S0Pin = 10;    // PIN S0
const int S1Pin = 7;     // PIN S1
const int S2Pin = 9;     // PIN S2
const int S3Pin = 32;    // PIN S3
const int S4Pin = 36;    // PIN S4
const int S5Pin = 40;    // PIN S5
const int S6Pin = 44;    // PIN S6
const int S7Pin = 48;    // PIN S7
const int S8Pin = 52;    // PIN S8
const int S9Pin = 9;     // PIN S9
const int S10Pin = 1;    // PIN S10

// Output Pins
const int stepPin = 2;   // Yellow driver cable
const int dirPin = 3;    // Blue driver cable
const int C1Pin = 6;     // Pin for relay pneumatic cylinder C1
const int C2Pin = 5;     // Pin for relay pneumatic cylinder C2
const int C3Pin = 4;     // Pin for relay pneumatic cylinder C3
const int FanPin = 13;   // Pin for relay to start/stop the fan


// ---------- Global Variables ----------
bool INIT = true;
int ProcessPhase = 0;
int timecounter = 0;
int deltaTime = 10;      // Time increment per cycle [ms]

// Process Variables
int INITSpeed = 2000;
int StepSpeed = 2000;     // Microseconds
int LengthBoard = 0;      // Length of the board
float StepinMM = 0.2;     // Factor of Speed to Length
int deltaLength = 0;      // Delta length per cycle
int deltaPosition = 0;    // Delta position per cycle
int distanceS1toS2 = 100; // Distance S1 to S2
int distanceS2toC = 100;  // Distance S2 to Center
int nPrint = 1;           // Number of prints required
int kPrint = 0;           // Number of prints done

// Sensor variables
int S0 = 0;              // Sensor Infeed Upstream Detection
int S1 = 0;              // Sensor Infeed Measure Unit
int S2 = 0;              // Sensor Print Measure Unit
int S3 = 0;              // C1 sensor at retracted position
int S4 = 0;              // C1 sensor at expanded position
int S5 = 0;              // C2 sensor at retracted position
int S6 = 0;              // C2 sensor at expanded position
int S7 = 0;              // C3 sensor at retracted position
int S8 = 0;              // C3 sensor at expanded position
int S9 = 0;              // Sensor Outfeed Downstream Detection
int S10 = 0;             // Bypass mode

// Actor variables
int StepState = 0;       // 0: Stopped; 1: Forward; 2: Backward
int dirState = 0;        // 0: Forward, 1: Backward
int C1 = 0;              // Cylinder 1 output
int C2 = 0;              // Cylinder 2 output
int C3 = 0;              // Cylinder 3 output
int FanState = 0;        // Fan Output

//


void setup() {
  // put your setup code here, to run once:
  INIT = true;
  pinMode(S0Pin, INPUT);
  pinMode(S1Pin, INPUT);
  pinMode(S2Pin, INPUT);
  pinMode(S3Pin, INPUT);
  pinMode(S4Pin, INPUT);
  pinMode(S5Pin, INPUT);
  pinMode(S6Pin, INPUT);
  pinMode(S7Pin, INPUT);
  pinMode(S8Pin, INPUT);
  pinMode(S9Pin, INPUT);
  pinMode(S10Pin, INPUT);
  pinMode(stepPin, OUTPUT);
  pinMode(dirPin, OUTPUT);
  pinMode(C1Pin, OUTPUT);
  pinMode(C2Pin, OUTPUT);
  pinMode(C3Pin, OUTPUT);
  pinMode(FanPin, OUTPUT);
  Serial.begin(9600);
}

void loop() {
  // put your main code here, to run repeatedly:
  if (INIT == true) {
    initialisation();
  }

  // Read INPUTS
  updateSensors();            // Read all sensors

  // Program logic
  timecounter += deltaTime;   // Zeitmessung
  
  switch (ProcessPhase) {
    case 0:                   // Ready to receive board
      if (S0 == 1 and S1 == 1) {
        ProcessPhase = 1;
        LengthBoard = distanceS1toS2;
        StepState = 0;
      }
      //if (S10 = 1){           // Bypass
        //ProcessPhase = 9;
      //}
      break;
    case 1:                   // Insert board
      if (S1 == 1) {
        StepState = 1;
        if (S1 == 1 && S2 == 1) {
          ProcessPhase = 2;
        }
      } else {
        StepState = 0;
        ProcessPhase = 0;
      }
      break;
    case 2:                   // Measuring Process
        measureBoard();
        break;
     case 3:                  // Positioning of the board
        calculatePosition();
        if (S7 == 0){
          C3 = 1;
        }
        break;  
     case 4:                  // Expand C3
        if (S3 == 1 && S5 == 1 && S8 == 1) {
          C2 = 1;
        } else if (S3 == 1 && S5 == 1) {
          C3 = 1;
        } else {
          C1 = 0;
          C2 = 0;
        }
        if (S6 == 1 && S8 == 1 && S4 == 0) {
          ProcessPhase = 5;
        }
        break;
     case 5:                 // Retract C1
        C3 = 0;
        if (S7 == 1 && S6 == 1) {
          ProcessPhase = 6;
        }
        break;
     case 6:                 // Retract C3 and expand C2
        C2 = 0;
        C1 = 1;
        if (S4 == 1 && S5 == 1 && S7 == 1) {
          ProcessPhase = 7;
        }
        break;
     case 7:                 // Expand C1 and retract C2
        C3 = 1;
        if (S8 == 1 && S4 == 1) {
          C1 = 0;
          kPrint += 1;
          if (kPrint <= nPrint) {   //Repeat process if needed)
            ProcessPhase = 4;
          } else {
            kPrint = 0;
            ProcessPhase = 8;           
          }
        }
        break;
     case 8:
        if (S9 == 0){
          StepState = 1;
          FanState = 1;
          StepSpeed = 4000;
        } else if (S9 == 1){
          StepState = 0;
          FanState = 0;
          StepSpeed = INITSpeed;
        }
     
        break;
     
     case 9:
      if (S10 == 1){
        StepState = 1;
      } else{
        StepState = 0;
        ProcessPhase = 0;
      }
        break;
     default:
    ;
  }

  // Write Outputs
  updateStepper();
  updateCylinders();
  updateFan();

}

//----------Functions ----------

// Initialising
void initialisation() {
  ProcessPhase = 0;
  StepState = 0;
  timecounter = 0;
  C1 = 0;
  C2 = 0;
  C3 = 0;
  kPrint = 0;
  
  // Setup calculations
  LengthBoard = distanceS1toS2;
  deltaLength = StepinMM;

  // Complete INIT phase
  INIT = false;
}

// Inputs
void updateSensors() {
  S0 != digitalRead(S0Pin);
  S1 != digitalRead(S1Pin);
  S2 != digitalRead(S2Pin);
  S3 = digitalRead(S3Pin);
  S4 = digitalRead(S4Pin);
  S5 = digitalRead(S5Pin);
  S6 = digitalRead(S6Pin);
  S7 = digitalRead(S7Pin);
  S8 = digitalRead(S8Pin);
  S9 != digitalRead(S9Pin);
  S10 = digitalRead(S10Pin);

  static int p = 1;
  p += 1;
  if (p == 1000) {
  Serial.println("Sensor update:");
  Serial.println(S0);
  Serial.println(S1);
  Serial.println(S2);
  Serial.println(S3);
  Serial.println(S4);
  Serial.println(S5);
  Serial.println(S6);
  Serial.println(S7);
  Serial.println(S8);
  Serial.println(S9);
  Serial.println(S10);
  Serial.println(ProcessPhase);
  Serial.println(StepState);
  p = 1;
  }
}


// Logic
void measureBoard() {
  if (S1 == 1 && S2 == 1) {
    LengthBoard += deltaLength;
  } else if (S2 == 1) {
    deltaPosition = distanceS1toS2 + distanceS2toC - (1/2 * LengthBoard);
    ProcessPhase = 3;
  } else {
    StepState = 0;
    ProcessPhase = 0;
  }
}

void calculatePosition() {
  static int timeout = 0;
  timeout += deltaTime;
  
  if (deltaPosition >= -1 && deltaPosition <= 1){
    StepState = 0;
    ProcessPhase = 4;
    timeout = 0;
  } else if (deltaPosition > 1 && timeout <= 30000) {
    StepState = 1;
    deltaPosition -= deltaLength;
  } else if (deltaPosition < -1 && timeout <= 30000) {
    StepState = 2;
    deltaPosition += deltaLength;
  } else {
    StepState = 0;
    ProcessPhase = 0;
    timeout = 0;
  }
}

// Outputs
void updateStepper() {
  if (StepState == 0) {                // Stop
    //digitalWrite(stepPin, LOW);
  } 
  if (StepState == 1) {  // Forward
    if (dirState == HIGH) {
      digitalWrite(dirPin, LOW);
      dirState = LOW;
    }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(StepSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(StepSpeed);
}
if (StepState == 2) {  // Backward
  if (dirState == LOW) {
    digitalWrite(dirPin, HIGH);
    dirState = HIGH;
  }
    digitalWrite(stepPin, HIGH);
    delayMicroseconds(StepSpeed);
    digitalWrite(stepPin, LOW);
    delayMicroseconds(StepSpeed); 
  }
  
}

void updateCylinders() {
  if (C1 == 0) {
    digitalWrite(C1Pin, LOW);
  } else if (C1 == 1) {
    digitalWrite(C1Pin, HIGH);
  }
  if (C2 == 0) {
    digitalWrite(C2Pin, LOW);
  } else if (C2 == 1) {
    digitalWrite(C2Pin, HIGH);
  }
  if (C3 == 0) {
    digitalWrite(C3Pin, LOW);
  } else if (C3 == 1) {
    digitalWrite(C3Pin, HIGH);
  }
}

void updateFan() {
  if (FanState == 0) {
    digitalWrite(FanPin, LOW);
  } else if (FanState == 1) {
    digitalWrite(FanPin, HIGH);    
  }
}

