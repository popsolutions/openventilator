/*
 * header for the motor functions
 */
#include <Arduino.h>
#ifndef DCMOTOR_H
#define DCMOTOR_H

#define LEFT 0
#define RIGHT 1
#define ENCODER_HOLES 32
#define RPM_MIN 10

#define condition (digitalRead(topEndSwitch) || digitalRead(bottomEndSwitch))

class dcMotor
{
    /*
 * private variables and functions (only for use in the class)
 */
private:
    // pin variables
    uint8_t motorPin;
    uint8_t sensorPin;
    // status variables
    bool emitted;
    uint32_t prevTime;
    // regulator variables
    uint16_t commandVariable;
    // functions

    /*
 * public variables and functions (for use outside the class)
 */
public:
    uint32_t getElapsedTime();
    dcMotor(uint8_t motorPin, uint8_t sensorPin);
    void detectElapsedTime();
    bool rotate();
    double getRpm();
    void setRpm(uint16_t rpm);
    bool handleMotor(); // alarm if false
};
#endif
