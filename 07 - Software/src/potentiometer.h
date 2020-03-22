/*
 * Header for the Potentiometer-Functions
 */
#include <Arduino.h>
#ifndef POTENTIOMETER_H
#define POTENTIOMETER_H

class potentiometer {
/*
 * private Variables and Functions (only for use in the class)
 */
private:
uint8_t gradiations;
uint8_t potiPin;
uint8_t value;
uint8_t prevValue;
/*
 * public Variables and Functions (for use outside the class)
 */
public:
potentiometer(uint8_t potiPin, uint8_t gradiations);
void check(uint8_t prevValue);
uint8_t getValue(uint8_t prevValue);
bool isTurned();
bool valueChanged();
};
#endif
