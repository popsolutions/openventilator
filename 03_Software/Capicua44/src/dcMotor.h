/*
 * header for the motor functions
 */
#include <Arduino.h>
#ifndef DCMOTOR_H
#define DCMOTOR_H

#define LEFT 0
#define RIGHT 1
#define DEBOUNCE_TIME 20

#define condition (digitalRead(topEndSwitch) || digitalRead(bottomEndSwitch))

class dcMotor
{
    /*
 * private variables and functions (only for use in the class)
 */
private:
    // pin variables
    uint8_t motorPin;
    uint8_t topSensorPin;
    uint8_t bottomSensorPin;
    uint8_t topEndSwitch;
    uint8_t bottomEndSwitch;
    // status variables
    bool prevEmitted;
    bool prevPressed;
    uint32_t prevMillis;
    // regulator variables
    uint8_t commandVariable;
    // functions

    /*
 * public variables and functions (for use outside the class)
 */
public:
    bool endSwitch();
    bool endSensor();
    uint8_t getDirection();
    uint8_t getSpeed();
    dcMotor(uint8_t motorPin, uint8_t topSensorPin, uint8_t bottomSensorPin, uint8_t topEndSwitch, uint8_t bottomEndSwitch);
    void setSpeed(uint8_t speed);
    bool handleMotor(); // alarm if false
};
#endif
