#ifndef SETTING_H
#define SETTING_H

#include <nvs_flash.h>
#include <nvs.h>
#include <Arduino.h>

#ifdef __cplusplus
extern "C"
{
#endif

// -----------------------------------
// Section: Constants and Definitions
// -----------------------------------
#define NUMERIC_SETTING_COUNT 13
#define BOOLEAN_SETTING_COUNT 10
#define STRING_SETTING_COUNT 10
#define STRING_MAX_LENGTH 64

    // -----------------------------------
    // Section: Enumerations
    // -----------------------------------
    typedef enum
    {
        COMPRESSOR_TEMP,
        COMPRESSOR_RANGE,
        HIGH_TEMP_WARNING,
        ANTI_FREEZE_TEMP,
        ANTI_FREEZE_RANGE,
        COMPRESSOR_REST_TIME,
        FILTER_WARNING,
        FAN2_TEMP,
        DEVICE_SERIAL_ID,
        CUSTOMER_ID,
        ACTIVATION_DATE,
        SERVICE_INTERVAL,
        HOURS_ELAPSED
    } NumericIndex;

    typedef enum
    {
        DEVICE_ON,
        PRESSURE_WARNING_ON,
        TEMP_WARNING_ON,
        FILTER_WARNING_ON,
        DOOR_WARNING_ON,
        FAN2_ON,
        ADVANCED_SETTINGS_ON,
        CLOUD_ON,
        SERIAL_LOG_ON,
        CLOUD_LOG_ON
    } BooleanIndex;

    typedef enum
    {
        WIFI_SSID,
        WIFI_PASSWORD,
        AP_NAME,
        AP_PASSWORD,
        SERVER_URL,
        SERVER_IP,
        INLET_SENSOR_ADDRESS,
        OUTLET_SENSOR_ADDRESS,
        ANTIFREEZE_SENSOR_ADDRESS,
        FILTER_SENSOR_ADDRESS
    } StringIndex;

    // -----------------------------------
    // Section: Default Values
    // -----------------------------------
    extern const int defaultNumericValues[NUMERIC_SETTING_COUNT];
    extern const bool defaultBooleanValues[BOOLEAN_SETTING_COUNT];
    extern const char *defaultStringValues[STRING_SETTING_COUNT];

    // -----------------------------------
    // Section: Temporary - Change Flags
    // -----------------------------------
    extern bool numericTempSettingsChanged[NUMERIC_SETTING_COUNT];
    extern bool booleanTempSettingsChanged[BOOLEAN_SETTING_COUNT];
    extern bool stringTempSettingsChanged[STRING_SETTING_COUNT];

    // -----------------------------------
    // Track last set time for each flag
    // -----------------------------------
    extern unsigned long numericLastSetTime[NUMERIC_SETTING_COUNT];
    extern unsigned long booleanLastSetTime[BOOLEAN_SETTING_COUNT];
    extern unsigned long stringLastSetTime[STRING_SETTING_COUNT];

    // -----------------------------------
    // Section: Functions
    // -----------------------------------
    void initializeSettings();
    void resetSettings();

    bool setNumericSetting(NumericIndex index, int value);       // Set Num
    bool setBooleanSetting(BooleanIndex index, bool value);      // Set Bool
    bool setStringSetting(StringIndex index, const char *value); // Set String

    int getNumericSetting(NumericIndex index);                                 // Get Num
    bool getBooleanSetting(BooleanIndex index);                                // Get Bool
    bool getStringSetting(StringIndex index, char *buffer, size_t bufferSize); // Get String

    void resetAllChangedFlagsToFalse(); // Reset all Flags to "false" - Temporary - Base
    void resetAllChangedFlagsToTrue();  // Reset all Flags to "true" - Temporary - Base

    bool resetChangedFlagTemp(const char *type, int index);      // Reset an Index Flag to "false" - Temporary
    bool getChangedFlagTemp(const char *settingType, int index); // Check Index Changed Flag Temporary

#ifdef __cplusplus
}
#endif

#endif // SETTING_H
