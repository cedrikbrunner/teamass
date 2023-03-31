/* 
Test code for "Gravity Digitales 5A Relais Modul"
I used the same code to test the fan.
Relay is connected with the Arduino Pin 1=int Relay; Pin2: 5V; Pin3: GND
Relay is connected with the powersupply and the fan 
COM with GND Fan // NO with GND powersupply (we used NO as normally opend to controll the fan)
NO -> if pin HIGH -> fan turns ON // if pin LOW -> fan turns OFF
plus powersupply directly to plus fan
*/ 

int Relay = 3; //Pin for 

void setup()
{
  pinMode(Relay, OUTPUT);     //Set Pin3 as output
}
void loop()
{
  digitalWrite(Relay, HIGH);   //Turn on relay
  delay(1000);
  digitalWrite(Relay, LOW);    //Turn off relay
  delay(1000);
}