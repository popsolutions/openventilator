/*
 * This is the main Programm of the Ventilator.
 */
#include <Arduino.h>
#include <EEPROM.h>
#include <LiquidCrystal_I2C.h> // By Frank The Brabaner
#include <PID_v1.h>
#include "potentiometer.h"
#include "pressureSensor.h"
#include "rotarySensor.h"
#include "dcMotor.h"

/* 
 * Defines for the ventilator, fill in your specific parameters here.
 */
#define DEBUG true
// input ranges for the parameters
#define RANGE_VOLUME 10
#define RANGE_BREATHS 10
#define RANGE_PROPORTION 10
// LCD defines
#define LCD_ADRESS 0x27
#define LCD_COLUMNS 16
#define LCD_ROWS 2
// potentiometer pin defines
#define POTI_1 A0
#define POTI_2 A1
#define POTI_3 A2
// pressure sensor pin defines
#define PRESSURE_ELECTRODE A3
#define PRESSURE_TOPSENSOR 5
#define PRESSURE_BOTTOMENSOR 6
// motor pin
#define MOTOR_PIN 12
// sensor pin
#define SENSOR_PIN 2
// regulator defines
#define KP 3.0f // amplifier part
#define KI 2.0f // integrating part
#define KD 0.1f // differentiating part
/*
 * Enums for the ventilator
 */
// EEPROM adress
enum EEPROM_ADRESS
{
	ADRESS_VOLUME,
	ADRESS_BREATHS,
	ADRESS_PROPORTIONS
};

/*
 * variables for the Program sequence
 */
// input parameters
uint8_t volume = EEPROM.read(ADRESS_VOLUME);
uint8_t breath = EEPROM.read(ADRESS_BREATHS);
uint8_t proportions = EEPROM.read(ADRESS_PROPORTIONS);
// regulator parameters
double regulatedVariable;
double manipulatedVariable;
double targetVariable;

// constructors for the parts (can also stand directly in the respective methods for better data encapsulation)
potentiometer poti1(A0, RANGE_VOLUME); // poti(pin, range);
potentiometer poti2(A1, RANGE_BREATHS);
potentiometer poti3(A2, RANGE_PROPORTION);
LiquidCrystal_I2C display(LCD_ADRESS, LCD_COLUMNS, LCD_ROWS);								  // display(adress, columns, rows);
rotarySensor encoder(SENSOR_PIN);															  // encoder(sensorPin);
PID regulator(&regulatedVariable, &manipulatedVariable, &targetVariable, KP, KI, KD, DIRECT); // regulator((&Input, &Output, &Setpoint, Kp, Ki, Kd, Direction);
dcMotor motor(MOTOR_PIN);																	  // motor(motorPin);
pressureSensor pressure(PRESSURE_ELECTRODE, PRESSURE_TOPSENSOR, PRESSURE_BOTTOMENSOR);		  // sensor(electodePin, topPin, bottomPin);

// funtions for programm sequence
void show(String topic, uint8_t value);
void handlePotentiometers(uint8_t &volume, uint8_t &breath, uint8_t &proportions);
void handleMotor(uint8_t &volume, uint8_t &breath, uint8_t &proportions); // in progress

void setup()
{
	// initialise the display
	display.init();
	display.backlight();
	// initialise the serial port
	Serial.begin(9600);
	// initialise the PID regulator
	regulator.SetMode(AUTOMATIC);
}
//#############################//
// main loop of the ventilator //
//#############################//
void loop()
{
	handlePotentiometers(volume, breath, proportions);
	handleMotor(volume, breath, proportions);
}

// core functions
void show(String topic, uint8_t value)
{
	display.clear();
	display.setCursor(0, 0); // setCourser(column, row);
	display.print(topic);
	if (value < 10)
		display.setCursor(15, 0);
	else
		display.setCursor(14, 0);
	display.print(value);
}
// head functions
void handlePotentiometers(uint8_t &volume, uint8_t &breath, uint8_t &proportions)
{
	if (poti1.isTurned())
	{
		volume = poti1.getValue();
		show("Volume: ", volume);
	}
	if (poti2.isTurned())
	{
		breath = poti2.getValue();
		show("Breath: ", breath);
	}
	if (poti3.isTurned())
	{
		proportions = poti3.getValue();
		show("Proportions: ", proportions);
	}
}
void handleMotor(uint8_t &volume, uint8_t &breath, uint8_t &proportions) // in progress
{
	uint32_t currentMillis = millis();
	static uint32_t prevMillis;

	regulatedVariable = encoder.getRpm();
	targetVariable = breath * 250;
	regulator.Compute();
	analogWrite(MOTOR_PIN, manipulatedVariable);

#ifdef DEBUG
	if (currentMillis - prevMillis > 500)
	{
		Serial.print("[DEBUG]: Encoder_Inkrements = ");
		Serial.println(encoder.getIncrementCount());
		Serial.print("[DEBUG]: Encoder_Rpm = ");
		Serial.println(regulatedVariable);
		Serial.print("[DEBUG]: Motor_PWM = ");
		Serial.println(manipulatedVariable);
		prevMillis = currentMillis;
	}
#endif
}