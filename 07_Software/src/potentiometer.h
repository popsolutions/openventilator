/*
 * Header for the Potentiometer-Functions
 */
#include <Arduino.h>
#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class potentiometer
{
    /*
 * private Variables and Functions (only for use in the class)
 */
private:
    uint8_t gradiations;
    uint8_t potiPin;
    uint8_t prevValue;
    uint8_t value;
    /*
 * public Variables and Functions (for use outside the class)
 */
public:
    potentiometer(uint8_t potiPin, uint8_t gradiations);
    uint8_t getValue();
    bool isTurned();
    bool valueChanged();
    ~potentiometer();
};
#endif
