#include <AccelStepper.h>

const int stepsPerRevolution = 200;  // Schritte pro Umdrehung
const int motorPin1 = 2;             // Step-Pin
const int motorPin2 = 3;             // Dir-Pin

AccelStepper feedinstepper(1, motorPin1, motorPin2);  // Initialisiere Stepper-Objekt

void setup() {
  feedinstepper.setMaxSpeed(1000);  // Setze maximale Geschwindigkeit in Schritten pro Sekunde
  feedinstepper.setAcceleration(0); // Deaktiviere Beschleunigung
}

void loop() {
  int stepsToMove = 300*2;

  // Zurücksetzen der Position auf Null
  feedinstepper.setCurrentPosition(0);

  feedinstepper.moveTo(stepsToMove);
  while (feedinstepper.distanceToGo() != 0) {
    feedinstepper.setSpeed(250);
    feedinstepper.runSpeedToPosition();
  }
  delay(1000);
}
