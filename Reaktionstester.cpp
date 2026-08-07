#include <Arduino.h>
#include "Reaktionstester.h"
#include <Wire.h>
#include <LiquidCrystal_I2C.h>
#include <LowPower.h>
#include <EEPROM.h>

Reaktionstester::Reaktionstester(int ledPinB, int ledPinG, int ledPinR, int btnPin1, int btnPin2) // Konstruktor
    : ledPinB(ledPinB),
      ledPinG(ledPinG),
      ledPinR(ledPinR),
      btnPin1(btnPin1),
      btnPin2(btnPin2),
      lcd(0x27, 16, 2)
{
    pinMode(this->ledPinB, OUTPUT);
    pinMode(this->ledPinG, OUTPUT);
    pinMode(this->ledPinR, OUTPUT);
    pinMode(this->btnPin1, INPUT);
    pinMode(this->btnPin2, INPUT);
}

void wakeUp()
{
}

int Reaktionstester::warteAufBTN()
{
    while (digitalRead(this->btnPin1) != LOW && digitalRead(this->btnPin2) != LOW)
    {
    }
    if (digitalRead(this->btnPin1) == LOW)
        return 1;
    else if (digitalRead(this->btnPin2) == LOW)
        return 2;
}

void Reaktionstester::displayNachricht(const String zeile1, const String zeile2)
{
    this->lcd.clear();
    this->lcd.setCursor(0, 0);
    this->lcd.print(zeile1);
    this->lcd.setCursor(0, 1);
    this->lcd.print(zeile2);
}

void Reaktionstester::lowPowerModus()
{
    attachInterrupt(digitalPinToInterrupt(this->btnPin1), wakeUp, LOW);
    attachInterrupt(digitalPinToInterrupt(this->btnPin2), wakeUp, LOW);
    LowPower.powerDown(SLEEP_FOREVER, ADC_OFF, BOD_OFF);
    detachInterrupt(digitalPinToInterrupt(this->btnPin1));
    detachInterrupt(digitalPinToInterrupt(this->btnPin2));
}

bool Reaktionstester::speichern(int besterwert)
{
    EEPROM.put(0, besterwert);
    return true;
}

int Reaktionstester::Lesen()
{
    int besterwert = 0;
    EEPROM.get(0, besterwert);
    return besterwert;
}

bool Reaktionstester::besterWert(int neuerWert)
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

bool Reaktionstester::loeschen()
{
    this->speichern(0);
}

bool Reaktionstester::wartezeit(int wartezeit)
{
    delay(500);
    unsigned long startZeit = millis();
    while (millis() - startZeit < wartezeit)
    {
        if (digitalRead(this->btnPin1) == LOW)
        {
            return true;
        }
    }
    return false;
}

void Reaktionstester::fehlversuch()
{
    this->displayNachricht("Fehlversuch   ", "                ");
    delay(1500);
    // this->ReaktionstestVorbereiten();
}

bool Reaktionstester::ledSpiel()
{
    long warteZeit = millis();
    digitalWrite(this->ledPinR, HIGH);
    while (digitalRead(this->btnPin1) != LOW)
    {
        if ((millis() - warteZeit) > 600 && (millis() - warteZeit) < 1200)
        {
            digitalWrite(this->ledPinR, LOW);
            digitalWrite(this->ledPinB, HIGH);
        }
        else if ((millis() - warteZeit) > 1200)
        {
            digitalWrite(this->ledPinB, LOW);
            digitalWrite(this->ledPinG, HIGH);
            return false;
        }
    }
    digitalWrite(this->ledPinB, LOW);
    digitalWrite(this->ledPinG, LOW);
    digitalWrite(this->ledPinR, LOW);
    return true;
}

int Reaktionstester::ReaktionstestVorbereiten() // Bereitet den Reactionstest vor und wartert anschließend im LowPower Modus auf eine eingabe
{
    this->lcd.init();
    this->lcd.backlight();
    this->displayNachricht("Reactionstest", "Starten!");
    digitalWrite(this->ledPinG, LOW);
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

int Reaktionstester::Reaktionstest() // Wartet eine zufällige Zeit und startet dann Reaktionstest, nach btn druck wird reaktionszeit gemessen
{
    this->displayNachricht("                ", "                ");
    if (this->wartezeit(random(800, 3800)))
    {
        this->fehlversuch();
        return 0;
    }

    if (this->ledSpiel())
    {
        this->fehlversuch();
        return 0;
    }
    this->zeit = millis();
    this->displayNachricht("Jetzt!!!        ", "                ");
    if (warteAufBTN() == 1)
    {
        this->reactionszeit = millis();
        digitalWrite(this->ledPinG, LOW);
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