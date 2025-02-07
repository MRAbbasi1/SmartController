#ifndef RELAY_CONTROL_H
#define RELAY_CONTROL_H

#include <Arduino.h>
#include "alarm.h"
#include "setting.h"
#include "tempSensor.h"

// ================================
// Pin Definitions
// ================================
#define COMPRESSOR_RELAY_PIN 38
#define EVAPORATOR_RELAY_PIN 37
#define CONDENSER_RELAY_PIN 35
#define FAN2_RELAY_PIN 36

// ================================
// Function Prototypes
// ================================
void setupRelays();
void controlRelays();
void reloadCachedData();
void controlEvaporatorRelay();
void controlCompressorAndCondenserRelays();
void controlFan2Relay();

bool getCompressorRelayStatus();
bool getEvaporatorRelayStatus();
bool getCondenserRelayStatus();
bool getFan2RelayStatus();

#endif // RELAY_CONTROL_H
