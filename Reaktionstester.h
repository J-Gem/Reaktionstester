#ifndef Reaktionstester_h
#define Reaktionstester_h
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Reaktionstester
{
private:
    int ledPinB;
    int ledPinG;
    int ledPinR;
    int btnPin1;
    int btnPin2;
    unsigned long zeit;
    unsigned long reactionszeit;
    LiquidCrystal_I2C lcd;

public:
    Reaktionstester(int ledPinB, int ledPinG, int ledPinR, int btn1, int btn2);
    int ReaktionstestVorbereiten();
    int Reaktionstest();
    int warteAufBTN();
    void displayNachricht(const String zeile1, const String zeile2);
    void lowPowerModus();
    bool besterWert(int neuerWert);
    bool speichern(int besterwert);
    int Lesen();
    bool loeschen();
    bool wartezeit(int wartezeit);
    void fehlversuch();
    bool ledSpiel();
};

#endif