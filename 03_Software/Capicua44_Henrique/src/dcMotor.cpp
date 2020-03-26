/*
 * funtions for the motor
 */

#include "dcMotor.h"

dcMotor *elapsedPointer;

static void elapsedInterrupt() // global int. function needed for class internal int. funcion
{
    elapsedPointer->detectElapsedTime();
}

dcMotor::dcMotor(uint8_t motorPin, uint8_t sensorPin)
{
    pinMode(motorPin, OUTPUT);
    pinMode(sensorPin, INPUT_PULLUP);
    this->motorPin = motorPin;
    this->sensorPin = sensorPin;
    elapsedPointer = this;
    attachInterrupt(digitalPinToInterrupt(sensorPin), elapsedInterrupt, RISING);
}
void dcMotor::detectElapsedTime() // class internal int. function
{
    this->prevTime = micros();
}
uint32_t dcMotor::getElapsedTime()
{
    uint32_t currentTime = micros();
    return (currentTime - this->prevTime);
}
bool dcMotor::rotate() // detects if the motor rotates with a minimum rpm
{
    float minRpm = getRpm();
    if (minRpm > RPM_MIN)
        return true;
    else
        return false;
}
double dcMotor::getRpm() // get the current rpm
{
    return 1 / ((double)getElapsedTime() / ((double)1000000 * (double)ENCODER_HOLES) * (double)60);
}
void dcMotor::setRpm(uint16_t speed)
{
    this->commandVariable = speed;
}
bool dcMotor::handleMotor()
{
    return false;
}