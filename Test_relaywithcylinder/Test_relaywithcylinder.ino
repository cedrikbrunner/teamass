/* 
Test code for "Gravity Digitales 5A Relais Modul"
I used the same code to test the fan.
Relay is connected with the Arduino Pin 1=int Relay; Pin2: 5V; Pin3: GND
Relay is connected with the powersupply and the fan 
COM with GND Fan // NO with GND powersupply (we used NO as normally opend to controll the fan)
NO -> if pin HIGH -> fan turns ON // if pin LOW -> fan turns OFF
plus powersupply directly to plus fan
*/ 

int Relay = 9; //Pin for 

void setup()
{
  pinMode(Relay, OUTPUT);     //Set Pin3 as output
  pinMode(3, INPUT);
  pinMode(5, INPUT);
  Serial.begin(9600);
  Serial.println("Hey we are here!");
}
void loop()
{
 int S1 = digitalRead(5);
int S2 = digitalRead(3);
if (S1 == LOW) {
  Serial.println("Cylinder extended"); 
  digitalWrite(Relay,HIGH);
}
else if (S2 == LOW) {
  Serial.println("Cylinder retracted!");
  digitalWrite(Relay, LOW);
}

else{
  Serial.println("moving");
}

}