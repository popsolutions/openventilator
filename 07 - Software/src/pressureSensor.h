/*
 * Header for the Sensor-Functions
 */
#include <Arduino.h>
#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

class pressureSensor {
/*
 * private Variables and Functions (only for use in the class)
 */
private:
/*
 * public Variables and Functions (for use outside the class)
 */
public:
pressureSensor(uint8_t anodePin, uint8_t cathodePin, uint8_t topIndicator, uint8_t bottomIndicator);
};
#endif
