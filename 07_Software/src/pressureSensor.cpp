/*
 * Funtions for the Sensors
 */

#include "pressureSensor.h"

pressureSensor::pressureSensor(uint8_t electrodePin, uint8_t topIndicatorPin, uint8_t bottomIndicatorPin)
{
	this->electrodePin = electrodePin;
	this->topIndicator = topIndicatorPin;
	this->bottomIndicator = bottomIndicatorPin;
	pinMode(electrodePin, INPUT);
	pinMode(topIndicatorPin, INPUT);
	pinMode(bottomIndicatorPin, INPUT);
}
bool pressureSensor::stateOf(uint8_t indicator)
{
	return digitalRead(indicator);
}
uint16_t pressureSensor::getPressure()
{
	return analogRead(electrodePin);
}