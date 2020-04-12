/*
 * funtions for the rotar sensor
 */

#include "rotarySensor.h"

rotarySensor *elapsedPointer;

static void interrupt() // global int. function needed for class internal int. funcion
{
    elapsedPointer->calculate();
}

rotarySensor::rotarySensor(uint8_t sensorPin1)
{
    elapsedPointer = this;
    pinMode(sensorPin1, INPUT);
    attachInterrupt(digitalPinToInterrupt(sensorPin1), interrupt, RISING);
}
rotarySensor::rotarySensor(uint8_t sensorPin1, uint8_t sensorPin2)
{
    // TODO: constructor and functions for rotary sensor in both directions
}
rotarySensor::rotarySensor(uint8_t sensorPin1, uint8_t topEndswitch, uint8_t bottomEndSwitch)
{
    // TODO: constructor and functions for rotary sensor and end switch
}
rotarySensor::rotarySensor(uint8_t sensorPin1, uint8_t sensorPin2, uint8_t topEndswitch, uint8_t bottomEndSwitch)
{
    // TODO: constructor and functions for rotary sensor in both directions and end switch
}
void rotarySensor::calculate()
{
    uint32_t elapsedTime;

    incrementCount++;
    elapsedTime = micros() - prevTime;
    rpm = (1.0 / ((double)elapsedTime / (1000000.0) * ENCODER_INCREMENTS)) * 60.0; // calculate the actual rpm
    prevTime = micros();
}
uint32_t rotarySensor::getIncrementCount()
{
    return incrementCount;
}
double rotarySensor::getRpm() // get the current rpm
{
    return rpm;
}
uint8_t rotarySensor::direction()
// detects if something rotates with a minimum fixed rpm
{
    uint8_t direction;
    // TODO: detect if forward or backward
    if (rpm > RPM_MIN)
        direction = UNDEFINED;
    // if(); detect the direction
    else
        direction = NO_ROTATION;
    return direction;
}
rotarySensor::~rotarySensor(){};