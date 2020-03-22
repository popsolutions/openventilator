/*
 * This is the main Programm of the Ventilator
 */
#include <Arduino.h>
#include "potentiometer.h"
#include <LiquidCrystal_I2C.h> // By Frank The Brabaner

/* defines for the ventilator
   fill in your spezific parameters here
 */
#define DEBUG true
// Ranges for the Parameters
#define RANGE_VOLUME 5
#define RANGE_BREATHS 5
#define RANGE_PROPORTION 5
// LCD defines
#define LCD_ADRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2

// konstruktors for the parts
potentiometer poti1(A0,RANGE_VOLUME); // poti(pin, range);
potentiometer poti2(A1,RANGE_BREATHS);
potentiometer poti3(A2,RANGE_PROPORTION);
LiquidCrystal_I2C display(LCD_ADRESS, LCD_COLUMNS, LCD_ROWS);
// Variables for the Program sequence

// Funtions for programm sequence
void showVolume(uint8_t value);
void showBreaths(uint8_t value);
void showProportions(uint8_t value);

void setup() {
	display.init();
	display.backlight();
	Serial.begin(9600);
}

void loop() {
	// variables for the parameters
	static uint8_t volume;
	static uint8_t breath;
	static int8_t proportions;

	volume = poti1.getValue(volume);
	breath = poti2.getValue(breath);
	proportions = poti3.getValue(proportions);

	if(poti1.isTurned()) {
		showVolume(volume);
	}
	if(poti2.isTurned()) {
		showBreaths(breath);
	}
	if(poti3.isTurned()) {
		showProportions(proportions);
	}
	delay(200);
	Serial.print("Messwert: ");
	Serial.println(analogRead(A3));
}

void showVolume(uint8_t value){
	display.clear();
	display.setCursor(0, 0); // setCourser(column, row);
	display.print("Volume: ");
	display.setCursor(9, 0);
	display.print(value);
}
void showBreaths(uint8_t value){
	display.clear();
	display.setCursor(0, 0);
	display.print("Breaths: ");
	display.setCursor(10, 0);
	display.print(value);
}
void showProportions(uint8_t value){
	display.clear();
	display.setCursor(0, 0);
	display.print("Proportions: ");
	display.setCursor(14, 0);
	display.print(value);
}
