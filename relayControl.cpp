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
// static bool cachedHighPressure = false;

static float inletTemp = 0.0;
static float antiFreezeTemp = 0.0;
static float fan2SensorTemp = 0.0;

// ================================
// Timing Variables
// ================================
unsigned long lastCheckTime = 0;
const unsigned long CHECK_INTERVAL = 30000; // Time for settings refresh
unsigned long lastRelayUpdateTime = 0;
unsigned long lastCompressorCheck = 0;
unsigned long compressorLastOffTime = 0;
const unsigned long RELAY_UPDATE_INTERVAL = 1000; // 1 second

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

    // Update relays in RELAY_UPDATE_INTERVAL intervals
    if (currentTime - lastRelayUpdateTime >= RELAY_UPDATE_INTERVAL)
    {
        controlEvaporatorRelay();
        controlCompressorAndCondenserRelays(); // Unified logic for compressor and condenser
        controlFan2Relay();
        lastRelayUpdateTime = currentTime;
    }
}

// ================================
// Reload Cached Data
// Refreshes settings, alarms, and sensor data from NVS
// ================================
void reloadCachedData()
{
    Serial.println(" ");
    Serial.println("---------------Relay Control: Check Flags---------------");

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

    Serial.println(" ");
    Serial.println("---------------Relay Control: Read Temperature---------------");

    // Read sensor temperatures
    inletTemp = readTemperatureByName("Inlet");
    antiFreezeTemp = readTemperatureByName("Antifreeze");
    fan2SensorTemp = readTemperatureByName("Filter");

    Serial.println(" ");
    Serial.println("---------------Relay Control: Check Alarm Status---------------");

    // Update alarm states
    cachedDoorClosed = isDoorClosed();
    cachedFilterWarning = isFilterWarning();
    // cachedHighPressure = isPressureHigh();
}

// ================================
// Relay Control Functions
// ================================
void controlEvaporatorRelay()
{
    bool newStatus = (deviceOn && cachedDoorClosed && !cachedFilterWarning);

    if (newStatus != evaporatorRelayStatus)
    {
        evaporatorRelayStatus = newStatus;
        digitalWrite(EVAPORATOR_RELAY_PIN, newStatus ? LOW : HIGH);
        Serial.println("");
        Serial.println("- - - - - - - - Relay Control - - - - - - - -");
        Serial.println(newStatus ? "🟢 Evaporator: ON" : "🔴 Evaporator: OFF");
        Serial.println("");
    }
}

void controlCompressorAndCondenserRelays()
{
    static unsigned long condenserOnTime = 0;
    bool newCondenserStatus = false;
    bool newCompressorStatus = false;

    if (deviceOn && cachedDoorClosed && !cachedFilterWarning &&
        antiFreezeTemp > antiFreezeBase + antiFreezeRange &&
        inletTemp >= compressorTemp + compressorRange)
    {
        newCondenserStatus = true;

        if (newCondenserStatus != condenserRelayStatus)
        {
            condenserOnTime = millis();
        }

        newCompressorStatus = (millis() - condenserOnTime >= 5000);
    }
    else
    {
        newCondenserStatus = false;
        newCompressorStatus = false;
        compressorLastOffTime = millis();
    }

    if (newCondenserStatus != condenserRelayStatus)
    {
        condenserRelayStatus = newCondenserStatus;
        digitalWrite(CONDENSER_RELAY_PIN, newCondenserStatus ? LOW : HIGH);
        Serial.println("");
        Serial.println("- - - - - - - - Relay Control - - - - - - - -");
        Serial.println(newCondenserStatus ? "🟡 Condenser: ON" : "🔴 Condenser: OFF");
        Serial.println("");
    }

    if (newCompressorStatus != compressorRelayStatus)
    {
        compressorRelayStatus = newCompressorStatus;
        digitalWrite(COMPRESSOR_RELAY_PIN, newCompressorStatus ? LOW : HIGH);
        Serial.println("");
        Serial.println("- - - - - - - - Relay Control - - - - - - - -");
        Serial.println(newCompressorStatus ? "🟠 Compressor: ON" : "🔴 Compressor: OFF");
        Serial.println("");
    }
}

void controlFan2Relay()
{
    bool newStatus = (deviceOn && cachedDoorClosed && !cachedFilterWarning &&
                      fan2SensorTemp > fan2Temp);

    if (newStatus != fan2RelayStatus)
    {
        fan2RelayStatus = newStatus;
        digitalWrite(FAN2_RELAY_PIN, newStatus ? LOW : HIGH);
        Serial.println("");
        Serial.println("- - - - - - - - Relay Control - - - - - - - -");
        Serial.println(newStatus ? "🟣 Fan2: ON" : "🔴 Fan2: OFF");
        Serial.println("");
    }
}