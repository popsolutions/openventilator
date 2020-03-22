/*
 * Here you are able to test some parts of Code with PlatformIO
 * command: test -e uno --verbose
 */

#include <Arduino.h>
#include "pressureSensor.h"

uint8_t pin1 = 1;
uint8_t pin2 = 2;
uint8_t pin3 = 3;

pressureSensor sensor(pin1,pin1,pin3);

void setup(){
	Serial.begin(9600);
}

void loop(){
	bool currState; //= sensor.stateOf(9);
	static bool prevState;

	if(currState != prevState) {
		Serial.print("state: ");
		Serial.println(currState);
		prevState = currState;
	}
}
