void setup() {
  // put your setup code here, to run once:
pinMode(3, INPUT);
pinMode(5, INPUT);
Serial.begin(9600);
Serial.println("Hey we are here!");
}

void loop() {
  // put your main code here, to run repeatedly:
int S1 = digitalRead(5);
int S2 = digitalRead(3);
if (S1 == LOW) {
  Serial.println("Cylinder extended"); 
}
else if (S2 == LOW) {
Serial.println("Cylinder retracted!");
}

else{
  Serial.println("moving");
}

}
