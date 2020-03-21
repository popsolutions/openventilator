/*
 * This is the main Programm of the Ventilator
 */
#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include "potentiometer.h"
#include "lcd.h"

/*
 * defines for the ventilator
 */
#define DEBUG true
#define LCDADRESS 0x27
#define COLUMNS 16
#define _ROWS 2
// Ranges for the Parameters
#define RANGE_VOLUME 10
#define RANGE_BREATHS 10
#define RANGE_PROPORTION 10

/*
 * konstruktors for the parts
 */
potentiometer poti1(A0,RANGE_VOLUME);
potentiometer poti2(A1,RANGE_BREATHS);
potentiometer poti3(A2,RANGE_PROPORTION);

void setup() {
	Serial.begin(9600);
}

void loop() {
	if(poti1.isTurned()) {
		Serial.print("Poti1: ");
		Serial.println(poti1.getValue());
	}
}
