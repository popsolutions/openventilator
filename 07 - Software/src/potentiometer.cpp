/*
 * Funtions for the Potentiometers
 */

#include "potentiometer.h"

potentiometer::potentiometer(uint8_t potiPin, uint8_t gradiations){
	pinMode(potiPin, INPUT);
}
