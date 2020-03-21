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
void getPosition();
/*
 * public Variables and Functions (for use outside the class)
 */
public:
potentiometer(uint8_t potiPin, uint8_t gradiations);
void getValue();
};
#endif
