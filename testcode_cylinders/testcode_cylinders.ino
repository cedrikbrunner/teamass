// ---------- Pins ----------
const int relayC1 = 2;        // Pin for relay pneumatic cylinder C1
const int relayC2 = 3;        // Pin for relay pneumatic cylinder C2
const int relayC3 = 4;        // Pin for relay pneumatic cylinder C3
const int S3 = 5;             // Pin for pneumatic cylinder C1 sensor 3
const int S4 = 6;            // Pin for pneumatic cylinder C1 sensor 4
const int S5 = 7;            // Pin for pneumatic cylinder C2 sensor 5
const int S6 = 8;            // Pin for pneumatic cylinder C2 sensor 6
const int S7 = 9;            // Pin for pneumatic cylinder C3 sensor 7
const int S8 = 10;            // Pin for pneumatic cylinder C3 sensor 8


// ---------- Variables ----------
int sensor_retracted = 0;
int sensor_extended = 0;
int sensor_ready = 0;


// ----------Setup ---------- 
void setup() {
  pinMode(S3, INPUT); // C1 sensor at retracted position
  pinMode(S4, INPUT); // C1 sensor at expanded position
  pinMode(S5, INPUT); // C2 sensor at retracted position
  pinMode(S6, INPUT); // C2 sensor at expanded position
  pinMode(S7, INPUT); // C3 sensor at retracted position
  pinMode(S8, INPUT); // C3 sensor at expanded position
  pinMode(relayC1,OUTPUT);
  pinMode(relayC2,OUTPUT);
  pinMode(relayC3,OUTPUT);

  Serial.begin(9600);
}


// ----------Main Loop ---------- 
void loop() {
  ready = LOW; //use to start the cycle
  Serial.println(digitalRead(S3));
  Serial.println(digitalRead(S4));
  Serial.println(digitalRead(S5));
  Serial.println(digitalRead(S6));
  Serial.println(digitalRead(S7));
  Serial.println(digitalRead(S8));
  Serial.println("--------------------:");
  delay(100);
  if (ready == HIGH) {
    extend_C2(); //lowers the first swiper
    extend_C1(); //brings the swiper accross the screen
    retract_C2(); //raises first swiper
    extend_C3(); //lowers second swiper
    retract_C1(); //brings swiper back accross the screen
    retract_C3(); //retracts second swiper to release board
    Serial.println("Board Printed!!!");
  }
}


//----------Functions ----------
void extend_C1() {
  if ((sensor_retracted = digitalRead(S3)) == LOW) {
    while ((sensor_extended = digitalRead(S4)) == HIGH) {
      digitalWrite(relayC1, HIGH);
      Serial.println("extending C1");
    }
  delay(1000); //just in case
  Serial.println("C1 extended");
  }
}


void extend_C2() {
  if ((sensor_retracted = digitalRead(S5)) == LOW) {
    while ((sensor_extended = digitalRead(S6)) == HIGH) {
      digitalWrite(relayC2, HIGH);
      Serial.println("extending C2");
    }
  delay(1000); //just in case
  Serial.println("C2 extended");
  }
}


void extend_C3() {
  if ((sensor_retracted = digitalRead(S7)) == LOW) {
    while ((sensor_extended = digitalRead(S8)) == HIGH) {
      digitalWrite(relayC3, HIGH);
      Serial.println("extending C3");
    }
  delay(1000); //just in case
  Serial.println("C3 extended");
  }
}


void retract_C1() {
  if ((sensor_extended = digitalRead(S4)) == LOW) {
    while ((sensor_retracted = digitalRead(S3)) == HIGH) {
      digitalWrite(relayC3, LOW);
      Serial.println("retracting C1");
    }
  delay(1000); //just in case
  Serial.println("C1 retracted");
  }
}

void retract_C2() {
  if ((sensor_extended = digitalRead(S6)) == LOW) {
    while ((sensor_retracted = digitalRead(S5)) == HIGH) {
      digitalWrite(relayC3, LOW);
      Serial.println("retracting C2");
    }
  delay(1000); //just in case
  Serial.println("C2 retracted");
  }
}


void retract_C3() {
  if ((sensor_extended = digitalRead(S8)) == LOW) {
    while ((sensor_retracted = digitalRead(S7)) == HIGH) {
      digitalWrite(relayC3, LOW);
      Serial.println("retracting C3");
    }
  delay(1000); //just in case
  Serial.println("C3 retracted");
  }
}