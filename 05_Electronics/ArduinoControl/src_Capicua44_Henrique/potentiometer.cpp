/*
 * Funtions for the Potentiometers
 */

#include "potentiometer.h"

potentiometer::potentiometer(uint8_t potiPin, uint8_t gradiations)
{
	// This constructor initialised the potentiometer
	pinMode(potiPin, INPUT);
	this->gradiations = gradiations;
	this->potiPin = potiPin;
}
uint8_t potentiometer::getValue()
{
	// this function gives a value in a fixed range
	// the previous value have to hand over for the "isTurned" function
	this->value = analogRead(potiPin);
	return map(analogRead(potiPin), 0, 1023, 1, gradiations + 1);
	// only from 5 to 1020 because the risk of bit jitter in the upper and lower range Part
}
bool potentiometer::isTurned()
{
	// this function detects a turn of the potentiometer
	this->value = analogRead(potiPin);
	// prevent signal noice
	if (value > prevValue + ((1023 / 2) / gradiations) || value < prevValue - ((1023 / 2) / gradiations))
	{
		this->prevValue = value;
		return true;
	}
	else
		return false;
}
potentiometer::~potentiometer(){};
