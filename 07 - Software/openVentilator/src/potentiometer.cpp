/*
 * Funtions for the Potentiometers
 */

#include "potentiometer.h"
#include "EEPROM.h"

potentiometer::potentiometer(uint8_t potiPin, uint8_t gradiations){
	pinMode(potiPin, INPUT);
	this->gradiations = gradiations;
	this->potiPin = potiPin;
}
uint8_t potentiometer::getValue(){
	uint8_t stepValue;
	uint16_t analogValue = analogRead(potiPin);

	stepValue = map(analogValue, 0, 1020, 1, gradiations); // only to 1020 because the risk of bit jitter in the upper range Part
	return stepValue;
}
bool potentiometer::isTurned(){
	static uint16_t prevValue;
	uint16_t currentValue = analogRead(potiPin);

	if(abs(currentValue - prevValue) > 1020 / gradiations) {
		prevValue = currentValue;
		return true;
	}else return false;
}
