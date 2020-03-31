/*
 * header for the motor functions
 */
#include <Arduino.h>

#ifndef DCMOTOR_H
#define DCMOTOR_H

class dcMotor
{
    /*
 * private variables and functions (only for use in the class)
 */
private:
    uint8_t motorPin;
    double rpm;
    /*
 * public variables and functions (for use outside the class)
 */
public:
    dcMotor(uint8_t motorPin);
    dcMotor(uint8_t motorPin, uint16_t pwmFrequency);
    void setRpm(double rpm);
    ~dcMotor();
};
#endif
