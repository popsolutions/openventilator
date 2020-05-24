/*
 * header for the rotary sensor functions
 */
#include <Arduino.h>
#include <PID_v1.h>

#ifndef ROTARYSENSOR_H
#define ROTARYSENSOR_H

// sensor defines
#define ENCODER_INCREMENTS 32.0f // number of increments on the disc
#define RPM_MIN 5                // minimum speed to detect motor failures
// sensor enums
enum DIRECTION
{
    NO_ROTATION,
    UNDEFINED,
    FORWARD,
    BACKWARD
};

class rotarySensor
{
    /*
 * private variables and functions (only for use in the class)
 */
private:
    uint32_t incrementCount;
    uint32_t prevTime;
    double rpm;

public:
    rotarySensor(uint8_t sensorPin1);
    rotarySensor(uint8_t sensorPin1, uint8_t sensorPin2);
    rotarySensor(uint8_t sensorPin1, uint8_t topEndswitch, uint8_t bottomEndSwitch);
    rotarySensor(uint8_t sensorPin1, uint8_t sensorPin2, uint8_t topEndswitch, uint8_t bottomEndSwitch);
    void calculate();
    uint32_t getIncrementCount();
    double getRpm();
    uint8_t direction();
    ~rotarySensor();
};
#endif
