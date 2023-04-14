// ---------- Pins ----------
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


// ---------- Variables ----------
bool board_labeled = false


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
}


// ----------Main Loop ---------- 
void loop() {
  if (board_labeled == false) {
    extend_C2(); //lowers the first swiper
    extend_C1(); //brings the swiper accross the screen
    retract_C2(); //raises first swiper
    extend_C3(); //lowers second swiper
    retract_C1(); //brings swiper back accross the screen
    retract_C3(); //retracts second swiper to release board
    board_labeled = true
  }

  if (digitalRead(S1) == LOW)
    board_labeled = false //for testing purposes to reset the system
}

//----------Functions ----------
void extend_C1() {
  if (digitalRead(S3)) == HIGH) {
    while (digitalRead(S4 == LOW)) {
      digitalWrite(relayC1, HIGH);
    }
  delay(1000); //just in case
  }
}


void extend_C2() {
  if (digitalRead(S5)) == HIGH) {
    while (digitalRead(S6 == LOW)) {
      digitalWrite(relayC2, HIGH);
    }
  delay(1000); //just in case
  }
}


void extend_C3() {
  if (digitalRead(S7)) == HIGH) {
    while (digitalRead(S8 == LOW)) {
      digitalWrite(relayC3, HIGH);
    }
  delay(1000); //just in case
  }
}


void retract_C1() {
  if (digitalRead(S4)) == HIGH) {
    while (digitalRead(S3 == LOW)) {
      digitalWrite(relayC3, LOW); //is this how you do this?
    }
  delay(1000); //just in case
  }
}


void retract_C2() {
  if (digitalRead(S6)) == HIGH) {
    while (digitalRead(S5 == LOW)) {
      digitalWrite(relayC3, LOW); //is this how you do this?
    }
  delay(1000); //just in case
  }
}


void retract_C3() {
  if (digitalRead(S8)) == HIGH) {
    while (digitalRead(S7 == LOW)) {
      digitalWrite(relayC3, LOW); //is this how you do this?
    }
  delay(1000); //just in case
  }
}