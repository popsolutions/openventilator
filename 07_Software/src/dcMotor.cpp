/*
 * funtions for the motor
 */

#include "dcMotor.h"

dcMotor::dcMotor(uint8_t motorPin)
{
    pinMode(motorPin, OUTPUT);
}
dcMotor::dcMotor(uint8_t motorPin, uint16_t pwmFrequency)
{
    pinMode(motorPin, OUTPUT);
    // TODO: PWM frequency
}
void dcMotor::setRpm(double rpm)
{
    analogWrite(motorPin, rpm);
}
dcMotor::~dcMotor() {}