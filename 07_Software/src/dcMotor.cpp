/*
 * funtions for the motor
 */

#include "dcMotor.h"

dcMotor *elapsedPointer;

static void regulateInterrupt() // global int. function needed for class internal int. funcion
{
    elapsedPointer->regulate();
}

dcMotor::dcMotor(uint8_t motorPin, uint8_t sensorPin) : PID(&controlledVariable, &manipulatedVariable, &commandVariable, KP, KI, KD, DIRECT)
{
    pinMode(motorPin, OUTPUT);
    pinMode(sensorPin, INPUT_PULLUP);
    this->motorPin = motorPin;
    this->sensorPin = sensorPin;
    elapsedPointer = this;
    attachInterrupt(digitalPinToInterrupt(sensorPin), regulateInterrupt, RISING);
    SetMode(AUTOMATIC);
    if (REVERSED_MOSFET)
        SetControllerDirection(REVERSE);
}
double dcMotor::calculate()
{
    uint32_t elapsedTime;
    elapsedTime = micros() - prevTime;
    return (1.0 / ((double)elapsedTime / (1000000.0))) * 60.0; // calculate the actual rpm
}
void dcMotor::regulate() // class internal int. function
{
    incrementCount++;
    //Serial.println(controlledVariable);
    controlledVariable = calculate();
    Compute();                                  // calculate the manipulated variable
    analogWrite(motorPin, manipulatedVariable); // regulate the motor
    prevTime = micros();
}
void dcMotor::setRpm(double commandVariable)
{
    this->commandVariable = commandVariable;
}
uint32_t dcMotor::getIncrementCount()
{
    return incrementCount;
}
double dcMotor::getRpm() // get the current rpm
{
    return controlledVariable;
}
bool dcMotor::rotate()
// detects if the motor rotates with a minimum fixed rpm
{
    if (calculate() > RPM_MIN)
        return true;
    else
        return false;
}
void dcMotor::start()
{
    commandVariable = stateVariable;
    analogWrite(motorPin, 255);
}
void dcMotor::stop()
{
    stateVariable = commandVariable;
    commandVariable = 0;
    analogWrite(motorPin, 0);
}