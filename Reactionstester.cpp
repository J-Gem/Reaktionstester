#include <Arduino.h>
#include "Reactionstester.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LowPower.h>

Reactionstester::Reactionstester(int ledPin1, int ledPin2, int btnPin1, int btnPin2) // Konstruktor
    : ledPin1(ledPin1),
      ledPin2(ledPin2),
      btnPin1(btnPin1),
      btnPin2(btnPin2),
      lcd(0x27, 16, 2)
{
    pinMode(this->ledPin1, OUTPUT);
    pinMode(this->ledPin2, OUTPUT);
    pinMode(this->btnPin1, INPUT);
    pinMode(this->btnPin2, INPUT);
}

void wakeUp()
{
}

bool Reactionstester::btnAbfrage()
{
    while (digitalRead(this->btnPin1) != LOW)
    {
    }
    return true;
}

void Reactionstester::displayNachricht(const String zeile1, const String zeile2)
{
    this->lcd.clear();
    this->lcd.setCursor(0,0);
    this->lcd.print(zeile1);
    this->lcd.setCursor(0,1);
    this->lcd.print(zeile2);
}

int Reactionstester::ReaktionstestVorbereiten() // Bereitet den Reactionstest vor und wartert anschließend im LowPower Modus auf eine eingabe
{
    this->lcd.init();
    this->lcd.backlight();
    this->displayNachricht("Reactionstest","Starten!");
    digitalWrite(this->ledPin1, LOW);

    //Low Power Mode
    attachInterrupt(digitalPinToInterrupt(2), wakeUp, LOW);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(digitalPinToInterrupt(2));

    if (btnAbfrage())
        return 1;
}

int Reactionstester::Reaktionstest()
{
    // Zeigt auf dem Display die Eingabe an
    this->displayNachricht("                ","                ");

    // Wartet zwischen 1 bis 5 Sekunden bis der Test startet
    delay(random(1000, 5000));

    // startzeit wird ermittelt, lampe und Display werden angesteuert
    digitalWrite(this->ledPin1, HIGH);
    this->zeit = millis();
    this->displayNachricht("Jetzt!!!        ","                ");
    if (btnAbfrage())
    {
        // endzeit wird gemessen und von der startzeit abgezoggen, lampe wird ausgeschaltet und Display wird aktualisiert
        this->reactionszeit = millis();
        digitalWrite(this->ledPin1, LOW);
        this->displayNachricht(String(reactionszeit-zeit) + " ms","                ");
        delay(3000);
        return 1;
    }
}

// Wokwi adruino