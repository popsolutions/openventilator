/*
 * Header for the Sensor-Functions
 */
#include <Arduino.h>
#ifndef PRESSURESENSOR_H
#define PRESSURESENSOR_H

class pressureSensor
{
    /*
 * private Variables and Functions (only for use in the class)
 */
private:
    uint8_t electrodePin;
    uint8_t topIndicator;
    uint8_t bottomIndicator;
    bool stateOf(uint8_t indicator);
    /*
 * public Variables and Functions (for use outside the class)
 */
public:
    pressureSensor(uint8_t electrodePin, uint8_t topIndicatorPin, uint8_t bottomIndicatorPin);
    bool handleSensor(); // alarm if false
    uint16_t getPressure();
};
#endif
