#include "relayControl.h"

// ================================
// Relay Status Variables
// ================================
bool compressorRelayStatus = false;
bool evaporatorRelayStatus = false;
bool condenserRelayStatus = false;
bool fan2RelayStatus = false;

// ================================
// Cached Settings and Sensor Data
// ================================
static bool deviceOn = false;
static bool fan2On = false;
static int compressorTemp = 0, compressorRange = 0, compressorRestTime = 0;
static int antiFreezeBase = 0, antiFreezeRange = 0, fan2Temp = 0;

static bool cachedDoorClosed = true;
static bool cachedFilterWarning = false;
static bool cachedHighPressure = false;

static float inletTemp = 0.0;
static float antiFreezeTemp = 0.0;
static float fan2SensorTemp = 0.0;

// ================================
// Timing Variables
// ================================
unsigned long lastCheckTime = 0;
const unsigned long CHECK_INTERVAL = 10000; // Time for settings refresh
unsigned long compressorLastOffTime = 0;

// ================================
// Setup Function
// Initializes relay pins and loads cached data
// ================================
void setupRelays()
{
    pinMode(COMPRESSOR_RELAY_PIN, OUTPUT);
    pinMode(EVAPORATOR_RELAY_PIN, OUTPUT);
    pinMode(CONDENSER_RELAY_PIN, OUTPUT);
    pinMode(FAN2_RELAY_PIN, OUTPUT);

    digitalWrite(COMPRESSOR_RELAY_PIN, HIGH); // OFF for Active LOW
    digitalWrite(EVAPORATOR_RELAY_PIN, HIGH);
    digitalWrite(CONDENSER_RELAY_PIN, HIGH);
    digitalWrite(FAN2_RELAY_PIN, HIGH);

    reloadCachedData();
}

// ================================
// Control Loop
// Periodically refreshes settings and controls all relays
// ================================
void controlRelays()
{
    unsigned long currentTime = millis();

    // Reload settings and sensor data every CHECK_INTERVAL
    if (currentTime - lastCheckTime >= CHECK_INTERVAL)
    {
        reloadCachedData();
        lastCheckTime = currentTime;
    }

    // Relay control functions
    controlEvaporatorRelay();
    controlCompressorAndCondenserRelays(); // Unified logic for compressor and condenser
    controlFan2Relay();
}

// ================================
// Reload Cached Data
// Refreshes settings, alarms, and sensor data from NVS
// ================================
void reloadCachedData()
{
    // ================================
    // Check and reload settings -> Flag -> NVS
    // ================================

    if (getChangedFlagTemp("boolean", DEVICE_ON))
    {
        deviceOn = getBooleanSetting(DEVICE_ON);
        resetChangedFlagTemp("boolean", DEVICE_ON);
    }

    if (getChangedFlagTemp("boolean", FAN2_ON))
    {
        fan2On = getBooleanSetting(FAN2_ON);
        resetChangedFlagTemp("boolean", FAN2_ON);
    }

    if (getChangedFlagTemp("numeric", COMPRESSOR_TEMP))
    {
        compressorTemp = getNumericSetting(COMPRESSOR_TEMP);
        resetChangedFlagTemp("numeric", COMPRESSOR_TEMP);
    }

    if (getChangedFlagTemp("numeric", COMPRESSOR_RANGE))
    {
        compressorRange = getNumericSetting(COMPRESSOR_RANGE);
        resetChangedFlagTemp("numeric", COMPRESSOR_RANGE);
    }

    if (getChangedFlagTemp("numeric", COMPRESSOR_REST_TIME))
    {
        compressorRestTime = getNumericSetting(COMPRESSOR_REST_TIME);
        resetChangedFlagTemp("numeric", COMPRESSOR_REST_TIME);
    }

    if (getChangedFlagTemp("numeric", ANTI_FREEZE_TEMP))
    {
        antiFreezeBase = getNumericSetting(ANTI_FREEZE_TEMP);
        resetChangedFlagTemp("numeric", ANTI_FREEZE_TEMP);
    }

    if (getChangedFlagTemp("numeric", ANTI_FREEZE_RANGE))
    {
        antiFreezeRange = getNumericSetting(ANTI_FREEZE_RANGE);
        resetChangedFlagTemp("numeric", ANTI_FREEZE_RANGE);
    }

    if (getChangedFlagTemp("numeric", FAN2_TEMP))
    {
        fan2Temp = getNumericSetting(FAN2_TEMP);
        resetChangedFlagTemp("numeric", FAN2_TEMP);
    }

    // Read sensor temperatures
    inletTemp = readTemperatureByName("Inlet");
    antiFreezeTemp = readTemperatureByName("Antifreeze");
    fan2SensorTemp = readTemperatureByName("Filter");

    // Update alarm states
    cachedDoorClosed = isDoorClosed();
    cachedFilterWarning = isFilterWarning();
    cachedHighPressure = isPressureHigh();
}

// ================================
// Control Evaporator Relay
// Handles the evaporator relay based on basic conditions
// ================================
void controlEvaporatorRelay()
{
    bool newStatus = (deviceOn && cachedDoorClosed && !cachedFilterWarning);

    if (newStatus != evaporatorRelayStatus)
    {
        evaporatorRelayStatus = newStatus;
        if (newStatus)
        {
            digitalWrite(EVAPORATOR_RELAY_PIN, LOW); // ON for Active LOW
            Serial.println("--------------------------------");
            Serial.println("️🔵 [Relay] Evaporator Relay: ON");
            Serial.println("--------------------------------");
        }
        else
        {
            digitalWrite(EVAPORATOR_RELAY_PIN, HIGH); // OFF for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🔴 [Relay] Evaporator Relay: OFF");
            Serial.println("--------------------------------");
        }
    }
}

// ================================
// Control Compressor and Condenser Relays
// Unified logic to ensure proper timing and activation
// ================================
void controlCompressorAndCondenserRelays()
{
    static unsigned long condenserOnTime = 0;
    bool newCondenserStatus = false;
    bool newCompressorStatus = false;

    // Conditions for condenser and compressor
    if (deviceOn && cachedDoorClosed && !cachedFilterWarning &&
        antiFreezeTemp > antiFreezeBase + antiFreezeRange &&
        inletTemp >= compressorTemp + compressorRange)
    {
        newCondenserStatus = true;

        // If condenser is turning on, record the time
        if (newCondenserStatus != condenserRelayStatus)
        {
            condenserOnTime = millis();
        }

        // Compressor turns on 5 seconds after condenser
        newCompressorStatus = (millis() - condenserOnTime >= 5000);
    }
    else if (antiFreezeTemp <= antiFreezeBase || inletTemp <= compressorTemp)
    {
        newCondenserStatus = false;
        newCompressorStatus = false;
        compressorLastOffTime = millis(); // Update last off time for compressor
    }

    // Update condenser relay
    if (newCondenserStatus != condenserRelayStatus)
    {
        condenserRelayStatus = newCondenserStatus;
        if (newCondenserStatus)
        {
            digitalWrite(CONDENSER_RELAY_PIN, LOW); // ON for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🟡 [Relay] Condenser Relay: ON");
            Serial.println("--------------------------------");
        }
        else
        {
            digitalWrite(CONDENSER_RELAY_PIN, HIGH); // OFF for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🔴 [Relay] Condenser Relay: OFF");
            Serial.println("--------------------------------");
        }
    }

    // Update compressor relay
    if (newCompressorStatus != compressorRelayStatus)
    {
        compressorRelayStatus = newCompressorStatus;
        if (newCompressorStatus)
        {
            digitalWrite(COMPRESSOR_RELAY_PIN, LOW); // ON for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🟠 [Relay] Compressor Relay: ON");
            Serial.println("--------------------------------");
        }
        else
        {
            digitalWrite(COMPRESSOR_RELAY_PIN, HIGH); // OFF for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🔴 [Relay] Compressor Relay: OFF");
            Serial.println("--------------------------------");
        }
    }
}

// ================================
// Control Fan2 Relay
// Handles the fan2 relay based on temperature and alarms
// ================================
void controlFan2Relay()
{
    bool newStatus = (deviceOn && cachedDoorClosed && !cachedFilterWarning &&
                      fan2SensorTemp > fan2Temp);

    if (newStatus != fan2RelayStatus)
    {
        fan2RelayStatus = newStatus;
        if (newStatus)
        {
            digitalWrite(FAN2_RELAY_PIN, LOW); // ON for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🟣 [Relay] Fan2 Relay: ON");
            Serial.println("--------------------------------");
        }
        else
        {
            digitalWrite(FAN2_RELAY_PIN, HIGH); // OFF for Active LOW
            Serial.println("--------------------------------");
            Serial.println("🔴 [Relay] Fan2 Relay: OFF");
            Serial.println("--------------------------------");
        }
    }
}
