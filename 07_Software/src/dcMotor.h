/*
 * header for the motor functions
 */
#include <Arduino.h>
#include <PID_v1.h>

#ifndef DCMOTOR_H
#define DCMOTOR_H

// motor defines
#define ENCODER_INCREMENTS 4.0 // number of increments on the disc
#define RPM_MIN 5              // minimum speed to detect motor failures
#define STARTUP_SPEED 20
#define REVERSED_MOSFET true
// regulator defines
#define KP 1   // amplifier part
#define KI 1   // integrating part
#define KD 0.0 // differentiating part

class dcMotor : public PID
{
    /*
 * private variables and functions (only for use in the class)
 */
private:
    // pin variables
    uint8_t motorPin;
    uint8_t sensorPin;
    // status variables
    bool emitted;
    uint32_t prevTime;
    // regulator variables
    double stateVariable;
    double commandVariable;     // the given rpm's
    double controlledVariable;  // the actual rpm's
    double manipulatedVariable; // the motor PWM
    uint32_t incrementCount;
    // functions
    double calculate();
    /*
 * public variables and functions (for use outside the class)
 */
public:
    dcMotor(uint8_t motorPin, uint8_t sensorPin);
    void regulate();
    void setRpm(double commandVariable);
    uint32_t getIncrementCount();
    double getRpm();
    bool rotate();
    void start();
    void stop();
};
#endif
