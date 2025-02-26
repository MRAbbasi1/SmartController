// alarm.cpp
// ===============================================
// Includes and Global Variables
// ===============================================

#include "alarm.h"
#include "setting.h"    // For NVS functions
#include "tempSensor.h" // For temperature readings

bool doorClosed = false;
bool tempAlarm = false;
bool filterAlarm = false;
bool pressureHigh = false;

// ===============================================
// Initialization Functions
// ===============================================

void alarmSetup()
{
    pinMode(DOOR_PIN, INPUT_PULLDOWN);
    pinMode(PRESSURE_PIN, INPUT_PULLDOWN);
    initializeSettings();
    Serial.println("🚨 [Alarm] Alarm system initialized ✅");
}

// ===============================================
// Door Monitoring
// ===============================================

bool isDoorClosed()
{
    bool doorSettingTemp = getChangedFlagTemp("boolean", DOOR_WARNING_ON);
    static bool doorWarning = false;

    if (doorSettingTemp)
    {
        doorWarning = getBooleanSetting(DOOR_WARNING_ON);
        resetChangedFlagTemp("boolean", DOOR_WARNING_ON);
    }

    if (!doorWarning)
    {
        doorClosed = true;
        Serial.println("🚨 [Alarm] Door is Closed ✅ - Default Disabled in Setting");
    }
    else
    {
        doorClosed = (digitalRead(DOOR_PIN) == HIGH);
        Serial.println(doorClosed ? "🚨 [Alarm] Door is Closed ✅" : "🚨 [Alarm] Door is Open ⛔️");
    }

    return doorClosed;
}

// ===============================================
// Pressure Monitoring
// ===============================================

bool isPressureHigh()
{
    bool pressureSettingTemp = getChangedFlagTemp("boolean", PRESSURE_WARNING_ON);
    static bool pressureWarning = false;

    if (pressureSettingTemp)
    {
        pressureWarning = getBooleanSetting(PRESSURE_WARNING_ON);
        resetChangedFlagTemp("boolean", PRESSURE_WARNING_ON);
    }

    if (!pressureWarning)
    {
        pressureHigh = false;
        Serial.println("🚨 [Alarm] Pressure is Normal ✅ - Default Disabled in Setting");
    }
    else
    {
        pressureHigh = (digitalRead(PRESSURE_PIN) == HIGH);
        Serial.println(pressureHigh ? "🚨 [Alarm] Pressure is High ⛔️" : "🚨 [Alarm] Pressure is Normal ✅");
    }

    return pressureHigh;
}

// ===============================================
// Filter Monitoring
// ===============================================

bool isFilterWarning()
{
    static bool filterWarning = false;
    static bool filterAlarm = false;
    static int filterWarningThreshold = 0;
    bool filterSettingTemp = getChangedFlagTemp("boolean", FILTER_WARNING_ON);

    if (filterSettingTemp)
    {
        filterWarning = getBooleanSetting(FILTER_WARNING_ON);
        filterWarningThreshold = getNumericSetting(FILTER_WARNING);
        resetChangedFlagTemp("boolean", FILTER_WARNING_ON);
    }

    if (!filterWarning)
    {
        if (filterAlarm)
        {
            filterAlarm = false;
            Serial.println("🚨 [Alarm] Filter is Normal ✅ - Default Disabled in Setting");
        }
    }
    else
    {
        float temperature = readTemperatureByName("Filter");

        // Trigger an alert if the temperature value is NaN or above the threshold.
        filterAlarm = (isnan(temperature) || temperature > filterWarningThreshold);

        // Warning messages for invalid values ​​and comparison with thresholds
        if (isnan(temperature))
        {
            Serial.println("🚨 [Alarm] Invalid Filter Temperature Reading (NAN) detected!");
        }
        else
        {
            Serial.printf("🚨 [Alarm] Filter temperature: %0.2f %s threshold of %d\n",
                          temperature, filterAlarm ? "exceeds" : "is below", filterWarningThreshold);
        }
    }

    return filterAlarm;
}

// ===============================================
// Temperature Monitoring
// ===============================================

bool isHighTempAlarm()
{
    static bool tempWarning = false;
    static bool tempAlarm = false;
    static int highTempWarningThreshold = 0;
    bool tempSettingTemp = getChangedFlagTemp("boolean", TEMP_WARNING_ON);

    if (tempSettingTemp)
    {
        tempWarning = getBooleanSetting(TEMP_WARNING_ON);
        highTempWarningThreshold = getNumericSetting(HIGH_TEMP_WARNING);
        resetChangedFlagTemp("boolean", TEMP_WARNING_ON);
    }

    if (!tempWarning)
    {
        if (tempAlarm)
        {
            tempAlarm = false;
            Serial.println("🚨 [Alarm] Temperature is Normal ✅ - Default Disabled in Setting");
        }
    }
    else
    {
        float temperature = readTemperatureByName("Inlet");

        // Trigger an alert if the temperature value is NaN or above the threshold.
        tempAlarm = (isnan(temperature) || temperature > highTempWarningThreshold);

        // Warning messages for invalid values ​​and comparison with thresholds
        if (isnan(temperature))
        {
            Serial.println("🚨 [Alarm] Invalid Temperature Reading (NAN) detected!");
        }
        else
        {
            Serial.printf("🚨 [Alarm] Temperature: %0.2f %s high threshold of %d\n",
                          temperature, tempAlarm ? "exceeds" : "is below", highTempWarningThreshold);
        }
    }

    return tempAlarm;
}
