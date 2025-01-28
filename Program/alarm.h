#ifndef ALARM_H
#define ALARM_H

#include "setting.h"    // Include for NVS reading functions
#include "tempSensor.h" // Include for temperature sensor handling

// Define the pins for door, pressure, and filter monitoring
#define DOOR_PIN 1
#define PRESSURE_PIN 2

// Declare global variables to hold alarm states
extern bool doorClosed;   // For door closed status
extern bool tempAlarm;    // For temperature alarm status
extern bool filterAlarm;  // For filter alarm status
extern bool pressureHigh; // For pressure alarm status

// Function declarations
void alarmSetup();
bool isDoorClosed();
bool isPressureHigh();
bool isFilterWarning();
bool isHighTempAlarm();

#endif
