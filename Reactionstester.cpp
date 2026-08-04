#include <Arduino.h>
#include "Reactionstester.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LowPower.h>
#include <EEPROM.h>

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

int Reactionstester::warteAufBTN()
{
    while (digitalRead(this->btnPin1) != LOW && digitalRead(this->btnPin2) != LOW)
    {
    }
    if (digitalRead(this->btnPin1) == LOW)
        return 1;
    else if (digitalRead(this->btnPin2) == LOW)
        return 2;
}

void Reactionstester::displayNachricht(const String zeile1, const String zeile2)
{
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print(zeile1);
    this->lcd.setCursor(0, 1);
    this->lcd.print(zeile2);
}

void Reactionstester::lowPowerModus()
{
    attachInterrupt(digitalPinToInterrupt(this->btnPin1), wakeUp, LOW);
    attachInterrupt(digitalPinToInterrupt(this->btnPin2), wakeUp, LOW);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(digitalPinToInterrupt(this->btnPin1));
    detachInterrupt(digitalPinToInterrupt(this->btnPin2));
}

bool Reactionstester::speichern(int besterwert)
{
    EEPROM.put(0, besterwert);
    return true;
}

int Reactionstester::Lesen()
{
    int besterwert = 0;
    EEPROM.get(0, besterwert);
    return besterwert;
}

bool Reactionstester::besterWert(int neuerWert)
{
    int besterwert = 0;
    EEPROM.get(0, besterwert);
    if (besterwert == 0 || besterwert > neuerWert)
    {
        return true;
    }
    else
    {
        return false;
    }
}

bool Reactionstester::loeschen()
{
    this->speichern(0);
}

int Reactionstester::ReaktionstestVorbereiten() // Bereitet den Reactionstest vor und wartert anschließend im LowPower Modus auf eine eingabe
{
    this->lcd.init();
    this->lcd.backlight();
    this->displayNachricht("Reactionstest", "Starten!");
    digitalWrite(this->ledPin1, LOW);
    this->lowPowerModus();
    switch (warteAufBTN())
    {
    case 1:
        return 1;
        break;

    case 2:
        this->loeschen();
        this->displayNachricht("Loesche     ", "Bestenwert");
        delay(500);
        this->ReaktionstestVorbereiten();
        break;
    default:
        this->ReaktionstestVorbereiten();
    }
}

int Reactionstester::Reaktionstest() // Wartet eine zufällige Zeit und startet dann Reaktionstest, nach btn druck wird reaktionszeit gemessen
{
    this->displayNachricht("                ", "                ");
    delay(random(1000, 5000));

    digitalWrite(this->ledPin1, HIGH);
    this->zeit = millis();
    this->displayNachricht("Jetzt!!!        ", "                ");
    if (warteAufBTN() == 1)
    {
        this->reactionszeit = millis();
        digitalWrite(this->ledPin1, LOW);
        this->displayNachricht(String(reactionszeit - zeit) + " ms", "                ");
        if (this->besterWert(reactionszeit - zeit) == true)
        {
            this->speichern(reactionszeit - zeit);
        }
        delay(2000);
        this->displayNachricht("Besterwert    ", String(this->Lesen()) + " ms");
        delay(2000);
        return 1;
    }
}

// Wokwi adruino