#define extend = HIGH; // need to test that if that works
#define retract = LOW;
int button1 = 3; //C1
int button2 = 4;
int button3 = 5; //C2
int button4 = 6;
int button5 = 7; //C3
int button6 = 8;
// relay 1 for c1
int relay1 = 9;
int relay2 = 10;
int relay3 = 11;

void setup() {
  // put your setup code here, to run once:
pinMode(button1, INPUT);
pinMode(button2, INPUT);
pinMode(button3, INPUT);
pinMode(button4, INPUT);
pinMode(button5, INPUT);
pinMode(button6, INPUT);
pinMode(relay1, OUTPUT);
pinMode(relay2, OUTPUT);
pinMode(relay3, OUTPUT);

Serial.begin(9600);
Serial.println("Hey we are here!");
}

void loop() {
  // put your main code here, to run repeatedly:
int B1 = digitalRead(button1);
int B2 = digitalRead(button2);
int B3 = digitalRead(button3);
int B4 = digitalRead(button4);
int B5 = digitalRead(button5);
int B6 = digitalRead(button6);

if (B1 == HIGH) {
  Serial.println("Cylinder 1 extended"); 
  digitalWrite(relay1, extend); // or LOW - need to test that
}

else if (B1 == HIGH) {
Serial.println("Cylinder 1 retracted!");
 digitalWrite(relay1, retract); // or HIGH - need to test that
}

else if (B3 == HIGH) {
Serial.println("Cylinder 2 extended!");
 digitalWrite(relay2, extend); // or LOW - need to test that
}

else if (B4 == HIGH) {
Serial.println("Cylinder 2 retracted!");
digitalWrite(relay2, retract);
}

else if (B5 == HIGH) {
Serial.println("Cylinder 3 extended!");
 digitalWrite(relay3, extend); // or LOW - need to test that
}

else if (B6 == HIGH) {
Serial.println("Cylinder 4 retracted!");
digitalWrite(relay3, retract);
}

else{
  Serial.println("nothing - Press a button!");
}

}
