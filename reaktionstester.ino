#include <LowPower.h>

#include <Reaktionstester.h>
#include <Wire.h>



Reaktionstester R1(5,6,9,2,3);


void setup() {
  // put your setup code here, to run once:
 Serial.begin(9600);
  //ablauf();

}

void loop() {
  // put your main code here, to run repeatedly:
  if (R1.ReaktionstestVorbereiten() == 1)
  {
    R1.Reaktionstest();
  }
}