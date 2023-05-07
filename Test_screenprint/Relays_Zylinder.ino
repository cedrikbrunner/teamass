#include <Arduino.h>

// Zylinderpins
const int zylinder1 = 8;
const int zylinder2 = 9;
const int zylinder3 = 10;

// Umschaltknöpfe Pins
const int umschalter1 = 2;
const int umschalter2 = 3;
const int umschalter3 = 4;

void setup() {
  pinMode(zylinder1, OUTPUT);
  pinMode(zylinder2, OUTPUT);
  pinMode(zylinder3, OUTPUT);

  pinMode(umschalter1, INPUT_PULLUP);
  pinMode(umschalter2, INPUT_PULLUP);
  pinMode(umschalter3, INPUT_PULLUP);
}

void loop() {
  // Zylinder 1 steuern
  if (digitalRead(umschalter1) == LOW) {
    digitalWrite(zylinder1, HIGH); // Zylinder 1 ausfahren
  } else {
    digitalWrite(zylinder1, LOW); // Zylinder 1 einfahren
  }

  // Zylinder 2 steuern
  if (digitalRead(umschalter2) == LOW) {
    digitalWrite(zylinder2, HIGH); // Zylinder 2 ausfahren
  } else {
    digitalWrite(zylinder2, LOW); // Zylinder 2 einfahren
  }

  // Zylinder 3 steuern
  if (digitalRead(umschalter3) == LOW) {
    digitalWrite(zylinder3, HIGH); // Zylinder 3 ausfahren
  } else {
    digitalWrite(zylinder3, LOW); // Zylinder 3 einfahren
  }

  delay(50); // Entprellzeit für die Umschaltknöpfe
}
