#ifndef Reactionstester_h
#define Reactionstester_h
#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

class Reactionstester
{
private:
    int ledPin1;
    int ledPin2;
    int btnPin1;
    int btnPin2;
    unsigned long zeit;
    unsigned long reactionszeit;
    LiquidCrystal_I2C lcd;

public:
    Reactionstester(int, int, int, int); // Konstruktor
    int ReaktionstestVorbereiten();
    int Reaktionstest();
    int warteAufBTN();
    void displayNachricht(const String zeile1, const String zeile2);
};

#endif