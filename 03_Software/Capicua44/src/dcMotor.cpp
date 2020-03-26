/*
 * funtions for the motor
 */

#include "dcMotor.h"

dcMotor::dcMotor(uint8_t motorPin, uint8_t topSensorPin, uint8_t bottomSensorPin, uint8_t topEndSwitch, uint8_t bottomEndSwitch)
{
    pinMode(motorPin, OUTPUT);
    pinMode(topSensorPin, INPUT);
    pinMode(bottomSensorPin, INPUT);
    pinMode(topEndSwitch, INPUT);
    pinMode(bottomEndSwitch, INPUT);
    this->motorPin = motorPin;
    this->topSensorPin = topSensorPin;
    this->bottomSensorPin = bottomSensorPin;
    this->topEndSwitch = topEndSwitch;
    this->bottomEndSwitch = bottomEndSwitch;
}
bool dcMotor::endSwitch()
{
    uint32_t currentTime = millis();
    static boolean triggerOnce = false;
    static boolean mindLoop = false;

    if (condition && mindLoop)
        return false;
    else
        mindLoop = false;
    if (condition && !triggerOnce)
    {
        triggerOnce = true;
        this->prevMillis = millis();
    }
    if (condition && (currentTime - prevMillis) > DEBOUNCE_TIME)
    {
        mindLoop = true;
        return true;
    }
    else
        return false;
}
bool dcMotor::endSensor()
{
    bool absorbed;

    if (!digitalRead(topSensorPin) && !digitalRead(bottomSensorPin))
        absorbed = true;
    else
        absorbed = false;
    if (absorbed != prevEmitted)
    {
        this->prevEmitted = absorbed;
        return absorbed;
    }
    else
        return false;
}
uint8_t dcMotor::getDirection()
{
    uint8_t direction;

    return direction;
}
uint8_t dcMotor::getSpeed()
{
    uint8_t speed;

    return speed;
}
void dcMotor::setSpeed(uint8_t speed)
{
    this->commandVariable = speed;
}
bool dcMotor::handleMotor()
{

    return false;
}