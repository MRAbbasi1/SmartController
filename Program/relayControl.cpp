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

static float inletTemp = 0.0;
static float antiFreezeTemp = 0.0;
static float fan2SensorTemp = 0.0;

// ================================
// Timing Variables
// ================================
unsigned long lastCheckTime = 0;
const unsigned long CHECK_INTERVAL = 5000; // Time for settings refresh
unsigned long lastRelayUpdateTime = 0;
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

    digitalWrite(COMPRESSOR_RELAY_PIN, HIGH); // OFF With HIGH | ON with LOW
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
        controlCompressorAndCondenserRelays();
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
    Serial.println("");
    Serial.println("---------------Relay Control: Check Settings---------------");

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

    Serial.println("");
    Serial.println("---------------Relay Control: Check Temperatures---------------");
    // Read sensor temperatures
    inletTemp = readTemperatureByName("Inlet");
    antiFreezeTemp = readTemperatureByName("Antifreeze");
    fan2SensorTemp = readTemperatureByName("Filter");

    Serial.println("");
    Serial.println("---------------Relay Control: Check Alarms---------------");
    // Update alarm states
    cachedDoorClosed = isDoorClosed();
    cachedFilterWarning = isFilterWarning();
    Serial.println("");
}

// ================================
// Relay Control Functions
// ================================
void controlEvaporatorRelay()
{
    bool newStatus = (deviceOn && cachedDoorClosed && !cachedFilterWarning && !isnan(inletTemp));

    if (newStatus != evaporatorRelayStatus)
    {
        evaporatorRelayStatus = newStatus;
        digitalWrite(EVAPORATOR_RELAY_PIN, newStatus ? LOW : HIGH);

        Serial.println("");
        Serial.print("[RELAY] 🟢 Evaporator: ");
        Serial.print(newStatus ? "ON" : "OFF");
        Serial.print(" | Reason: ");

        if (!deviceOn)
            Serial.println("🔴 Device is OFF");
        else if (!cachedDoorClosed)
            Serial.println("🚨 Door is OPEN");
        else if (cachedFilterWarning)
            Serial.println("🚨 Filter warning active");
        else if (isnan(inletTemp))
            Serial.println("⛔️ Invalid inlet temperature (NAN)");
        else
            Serial.println("✅ All conditions met");

        Serial.println("");
    }
}

void controlCompressorAndCondenserRelays()
{
    static unsigned long condenserOnTime = 0;
    bool newCondenserStatus = false;
    bool newCompressorStatus = false;

    if (deviceOn && cachedDoorClosed && !cachedFilterWarning && !isnan(antiFreezeTemp) && !isnan(inletTemp))
    {
        if (antiFreezeTemp > antiFreezeBase + antiFreezeRange)
        {
            if (inletTemp >= compressorTemp + compressorRange || inletTemp >= compressorTemp)
            {
                newCondenserStatus = true;

                if (newCondenserStatus != condenserRelayStatus)
                {
                    condenserOnTime = millis();
                }

                newCompressorStatus = (millis() - condenserOnTime >= 5000);
            }
        }
    }
    else
    {
        newCondenserStatus = false;
        newCompressorStatus = false;
    }

    if (newCondenserStatus != condenserRelayStatus)
    {
        condenserRelayStatus = newCondenserStatus;
        digitalWrite(CONDENSER_RELAY_PIN, newCondenserStatus ? LOW : HIGH);

        Serial.println("");
        Serial.print("[RELAY] 🟡 Condenser: ");
        Serial.print(newCondenserStatus ? "ON" : "OFF");
        Serial.print(" | Reason: ");

        if (!deviceOn)
            Serial.println("🔴 Device is OFF");
        else if (!cachedDoorClosed)
            Serial.println("🚨 Door is OPEN");
        else if (cachedFilterWarning)
            Serial.println("🚨 Filter warning active");
        else if (isnan(antiFreezeTemp))
            Serial.println("⛔️ Invalid antifreeze temperature (NAN)");
        else if (isnan(inletTemp))
            Serial.println("⛔️ Invalid inlet temperature (NAN)");
        else if (antiFreezeTemp <= antiFreezeBase + antiFreezeRange)
            Serial.println("🥶 Antifreeze temperature too low");
        else if (inletTemp < compressorTemp + compressorRange)
            Serial.println("🆗 Inlet temperature below range");
        else
            Serial.println("✅ All conditions met");
        Serial.println("");
    }

    if (newCompressorStatus != compressorRelayStatus)
    {
        compressorRelayStatus = newCompressorStatus;
        digitalWrite(COMPRESSOR_RELAY_PIN, newCompressorStatus ? LOW : HIGH);

        Serial.println("");
        Serial.print("[RELAY] 🔵 Compressor: ");
        Serial.print(newCompressorStatus ? "ON" : "OFF");
        Serial.print(" | Reason: ");

        if (!newCondenserStatus)
            Serial.println("🔴 Condenser is OFF");
        else if ((millis() - condenserOnTime) < 5000)
            Serial.println("⌛️ Waiting for delay");
        else
            Serial.println("✅ All conditions met");
        Serial.println("");
    }
}

void controlFan2Relay()
{
    bool newStatus = (deviceOn && cachedDoorClosed && !cachedFilterWarning &&
                      !isnan(fan2SensorTemp) && fan2SensorTemp > fan2Temp);

    if (newStatus != fan2RelayStatus)
    {
        fan2RelayStatus = newStatus;
        digitalWrite(FAN2_RELAY_PIN, newStatus ? LOW : HIGH);

        Serial.println("");
        Serial.print("[RELAY] 🟠 Fan2: ");
        Serial.print(newStatus ? "ON" : "OFF");
        Serial.print(" | Reason: ");

        if (!deviceOn)
            Serial.println("🔴 Device is OFF");
        else if (!cachedDoorClosed)
            Serial.println("🚨 Door is OPEN");
        else if (cachedFilterWarning)
            Serial.println("🚨 Filter warning active");
        else if (isnan(fan2SensorTemp))
            Serial.println("⛔️ Invalid sensor temperature (NAN)");
        else if (fan2SensorTemp <= fan2Temp)
            Serial.println("🆗 Sensor temperature below threshold");
        else
            Serial.println("✅ All conditions met");
        Serial.println("");
    }
}

// ================================
// Get status functions
// ================================
bool getCompressorRelayStatus()
{
    Serial.printf("🔵 Compressor Relay Working: %s\n", compressorRelayStatus ? "True" : "False");
    return compressorRelayStatus;
}

bool getEvaporatorRelayStatus()
{
    Serial.printf("🟢 Evaporator Relay Working:  %s\n", evaporatorRelayStatus ? "True" : "False");
    return evaporatorRelayStatus;
}

bool getCondenserRelayStatus()
{
    Serial.printf("🟡 Condensor Relay Working:  %s\n", condenserRelayStatus ? "True" : "False");
    return condenserRelayStatus;
}

bool getFan2RelayStatus()
{
    Serial.printf("🟠 Fan-2 Relay Working:  %s\n", fan2RelayStatus ? "True" : "False");
    return fan2RelayStatus;
}