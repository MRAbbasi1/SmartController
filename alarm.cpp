// ===============================================
// Includes and Global Variables
// ===============================================

#include "alarm.h"
#include "setting.h"    // For NVS functions
#include "tempSensor.h" // For temperature readings from sensors

// Initialize global variables
bool doorClosed = false;   // Door closed status
bool tempAlarm = false;    // Temperature alarm status
bool filterAlarm = false;  // Filter alarm status
bool pressureHigh = false; // Pressure alarm status

// ===============================================
// Initialization Functions
// ===============================================

/**
 * @brief Initialize the alarm system for door, pressure, and filter monitoring.
 */
void alarmSetup()
{
    // Set pin modes for door and pressure monitoring
    pinMode(DOOR_PIN, INPUT_PULLDOWN);
    pinMode(PRESSURE_PIN, INPUT_PULLDOWN);

    // Initialize settings before accessing NVS
    initializeSettings();

    Serial.println("🚨 [Alarm] Alarm system initialized ✅");
}

// ===============================================
// Door Monitoring Function
// ===============================================

/**
 * @brief Check if the door is closed based on NVS setting.
 *
 * @return true if the door is closed, false otherwise.
 */
bool isDoorClosed()
{
    // -----------------------------------------------
    // Step 1: Check Temporary Flags
    // -----------------------------------------------

    // Temporary flag: Indicates if the setting has been read or not.
    bool doorSettingTemp = getChangedFlagTemp("boolean", DOOR_WARNING_ON);

    // -----------------------------------------------
    // Step 2: Define and Retain Last State
    // -----------------------------------------------
    // Retain the last known state of the door warning.
    static bool doorWarning = false;

    // -----------------------------------------------
    // Step 3: Read NVS If Necessary
    // -----------------------------------------------
    // If Temporary flags are true, read from NVS.
    if (doorSettingTemp)
    {
        doorWarning = getBooleanSetting(DOOR_WARNING_ON);

        // Reset the temporary flag after reading NVS.
        resetChangedFlagTemp("boolean", DOOR_WARNING_ON);
    }

    // -----------------------------------------------
    // Step 4: Process the Door State
    // -----------------------------------------------
    if (!doorWarning)
    {
        // If doorWarning is false, force the door to be considered closed.
        doorClosed = true;

        Serial.println("🚨 [Alarm] Door is Closed ✅");
    }
    else
    {
        // Read the current door state from the hardware pin.
        int doorState = digitalRead(DOOR_PIN);

        // Determine if the door is closed.
        doorClosed = (doorState == HIGH);

        // Log the door state.
        if (doorClosed)
        {

            Serial.println("🚨 [Alarm] Door is Closed ✅");
        }
        else
        {

            Serial.println("🚨 [Alarm] Door is Open ⛔️");
        }
    }

    // -----------------------------------------------
    // Step 5: Return the Door State
    // -----------------------------------------------
    return doorClosed;
}

// ===============================================
// Pressure Monitoring Function
// ===============================================

/**
 * @brief Check if the pressure is high based on NVS setting.
 *
 * @return true if the pressure is high, false otherwise.
 */
bool isPressureHigh()
{
    // -----------------------------------------------
    // Step 1: Check Temporary Flags
    // -----------------------------------------------

    // Temporary flag: Indicates if the setting has been read or not.
    bool pressureSettingTemp = getChangedFlagTemp("boolean", PRESSURE_WARNING_ON);

    // -----------------------------------------------
    // Step 2: Define and Retain Last State
    // -----------------------------------------------
    // Retain the last known state of the pressure warning.
    static bool pressureWarning = false;

    // -----------------------------------------------
    // Step 3: Read NVS If Necessary
    // -----------------------------------------------
    // If Temporary flags are true, read from NVS.
    if (pressureSettingTemp)
    {
        pressureWarning = getBooleanSetting(PRESSURE_WARNING_ON);

        // Reset the temporary flag after reading NVS.
        resetChangedFlagTemp("boolean", PRESSURE_WARNING_ON);
    }

    // -----------------------------------------------
    // Step 4: Process the Pressure State
    // -----------------------------------------------
    if (!pressureWarning)
    {
        // If pressureWarning is false, pressure is normal.
        pressureHigh = false;

        Serial.println("🚨 [Alarm] Pressure is Normal ✅");
    }
    else
    {
        // Read the current pressure state from the hardware pin.
        int pressureState = digitalRead(PRESSURE_PIN);

        // Determine if the pressure is high.
        pressureHigh = (pressureState == HIGH);

        // Log the pressure state.
        if (pressureHigh)
        {

            Serial.println("🚨 [Alarm] Pressure is High ⛔️");
        }
        else
        {

            Serial.println("🚨 [Alarm] Pressure is Normal ✅");
        }
    }

    // -----------------------------------------------
    // Step 5: Return the Pressure State
    // -----------------------------------------------
    return pressureHigh;
}

// ===============================================
// Filter Monitoring Functions
// ===============================================

/**
 * @brief Check if the filter temperature exceeds the warning threshold, based on NVS setting.
 *
 * @return true if the temperature exceeds the warning threshold, false otherwise.
 */
bool isFilterWarning()
{
    // -----------------------------------------------
    // Step 1: Define and Retain Last State
    // -----------------------------------------------
    static bool filterWarning = false;     // Retain the last known warning state.
    static bool filterAlarm = false;       // Retain the last alarm state.
    static int filterWarningThreshold = 0; // Retain the warning threshold.

    // -----------------------------------------------
    // Step 2: Check Temporary Flags
    // -----------------------------------------------
    bool filterSettingTemp = getChangedFlagTemp("boolean", FILTER_WARNING_ON);

    // -----------------------------------------------
    // Step 3: Read NVS Only If Necessary
    // -----------------------------------------------
    if (filterSettingTemp)
    {
        // Read the filter warning change flag and threshold from NVS.
        filterWarning = getBooleanSetting(FILTER_WARNING_ON);
        filterWarningThreshold = getNumericSetting(FILTER_WARNING);

        // Reset the temporary flag after reading NVS.
        resetChangedFlagTemp("boolean", FILTER_WARNING_ON);
    }

    // -----------------------------------------------
    // Step 4: Process the Filter Alarm State
    // -----------------------------------------------
    if (!filterWarning)
    {
        if (filterAlarm) // Only log if the alarm state changes.
        {
            filterAlarm = false;

            Serial.println("🚨 [Alarm] Filter is Normal ✅");
        }
    }
    else
    {
        // Read the temperature from Sensor 3.
        float temperature = readTemperatureByName("Filter");

        // Compare the temperature with the threshold.
        if (temperature > filterWarningThreshold)
        {
            if (!filterAlarm) // Only log if the alarm state changes.
            {
                filterAlarm = true;

                Serial.printf("🚨 [Alarm] Filter temperature: %0.2f exceeds the threshold of %d\n", temperature, filterWarningThreshold);
            }
        }
        else
        {
            if (filterAlarm) // Only log if the alarm state changes.
            {
                filterAlarm = false;

                Serial.printf("🚨 [Alarm] Filter temperature: %0.2f is below the threshold of %d\n", temperature, filterWarningThreshold);
            }
        }
    }

    // -----------------------------------------------
    // Step 5: Return the Filter Alarm State
    // -----------------------------------------------
    return filterAlarm;
}

// ===============================================
// Temperature Monitoring Functions
// ===============================================

/**
 * @brief Check if the temperature exceeds the high temperature warning threshold, based on NVS setting.
 *
 * @return true if the temperature exceeds the threshold, false otherwise.
 */
bool isHighTempAlarm()
{
    // -----------------------------------------------
    // Step 1: Define and Retain Last State
    // -----------------------------------------------
    static bool tempWarning = false;         // Retain the last known warning state.
    static bool tempAlarm = false;           // Retain the last alarm state.
    static int highTempWarningThreshold = 0; // Retain the high temperature threshold.

    // -----------------------------------------------
    // Step 2: Check Temporary Flags
    // -----------------------------------------------
    bool tempSettingTemp = getChangedFlagTemp("boolean", TEMP_WARNING_ON);

    // -----------------------------------------------
    // Step 3: Read NVS Only If Necessary
    // -----------------------------------------------
    if (tempSettingTemp)
    {
        // Read the high temperature warning change flag and threshold from NVS.
        tempWarning = getBooleanSetting(TEMP_WARNING_ON);
        highTempWarningThreshold = getNumericSetting(HIGH_TEMP_WARNING);

        // Reset the temporary flag after reading NVS.
        resetChangedFlagTemp("boolean", TEMP_WARNING_ON);
    }

    // -----------------------------------------------
    // Step 4: Process the Temperature Alarm State
    // -----------------------------------------------
    if (!tempWarning)
    {
        if (tempAlarm) // Only log if the alarm state changes.
        {
            tempAlarm = false;

            Serial.println("🚨 [Alarm] Temperature is Normal ✅");
        }
    }
    else
    {
        // Read the temperature from Sensor 0.
        float temperature = readTemperatureByName("Inlet");

        // Compare the temperature with the threshold.
        if (temperature > highTempWarningThreshold)
        {
            if (!tempAlarm) // Only log if the alarm state changes.
            {
                tempAlarm = true;

                Serial.printf("🚨 [Alarm] Temperature: %0.2f exceeds the high threshold of %d\n", temperature, highTempWarningThreshold);
            }
        }
        else
        {
            if (tempAlarm) // Only log if the alarm state changes.
            {
                tempAlarm = false;

                Serial.printf("🚨 [Alarm] Temperature: %0.2f is below the high threshold of %d\n", temperature, highTempWarningThreshold);
            }
        }
    }

    // -----------------------------------------------
    // Step 5: Return the Temperature Alarm State
    // -----------------------------------------------
    return tempAlarm;
}
