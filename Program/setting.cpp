// -----------------------------------
// NVS Methods for Setting Management Module
// -----------------------------------

#include "setting.h"
#include <nvs_flash.h>
#include <nvs.h>
#include <string.h>

// -----------------------------------
// Section: Default Values
// -----------------------------------
const int defaultNumericValues[] = {
    20,        // COMPRESSOR_TEMP
    5,         // COMPRESSOR_RANGE
    40,        // HIGH_TEMP_WARNING
    -5,        // ANTI_FREEZE_TEMP
    10,        // ANTI_FREEZE_RANGE
    3,         // COMPRESSOR_REST_TIME
    80,        // FILTER_WARNING
    55,        // FAN2_TEMP
    100001,    // DEVICE_SERIAL_ID
    123456789, // CUSTOMER_ID
    20241104,  // ACTIVATION_DATE
    365,       // SERVICE_INTERVAL
    0          // HOURS_ELAPSED
};

const bool defaultBooleanValues[] = {
    false, // DEVICE_ON
    false, // PRESSURE_WARNING_ON
    false, // TEMP_WARNING_ON
    false, // FILTER_WARNING_ON
    false, // DOOR_WARNING_ON
    false, // FAN2_ON
    false, // ADVANCED_SETTINGS_ON
    false, // CLOUD_ON
    false, // SERIAL_LOG_ON
    false  // CLOUD_LOG_ON
};

const char *defaultStringValues[] = {
    "0",                       // WIFI_SSID
    "0",                       // WIFI_PASSWORD
    "0",                       // AP_NAME
    "0",                       // AP_PASSWORD
    "http://server.com",       // SERVER_URL
    "192.168.1.1",             // SERVER_IP
    "28:2D:11:58:D4:E1:3C:BC", // INLET_SENSOR_ADDRESS
    "28:BD:AD:43:D4:E1:3C:CF", // OUTLET_SENSOR_ADDRESS
    "28:AB:CC:43:D4:E1:3C:FA", // ANTIFREEZE_SENSOR_ADDRESS
    "28:FF:64:1F:70:68:7C:7C"  // FILTER_SENSOR_ADDRESS
};

// -----------------------------------
// Section: Validation Ranges
// -----------------------------------

// Limits for numbers
const int numericMinValues[] = {
    20,       // COMPRESSOR_TEMP_MIN
    0,        // COMPRESSOR_RANGE
    20,       // HIGH_TEMP_WARNING
    -15,      // ANTI_FREEZE_TEMP
    0,        // ANTI_FREEZE_RANGE
    0,        // COMPRESSOR_REST_TIME
    60,       // FILTER_WARNING
    30,       // FAN2_TEMP
    0,        // DEVICE_SERIAL_ID
    0,        // CUSTOMER_ID
    20010903, // ACTIVATION_DATE
    0,        // SERVICE_INTERVAL
    0         // HOURS_ELAPSED
};

const int numericMaxValues[] = {
    50,        // COMPRESSOR_TEMP_MAX
    15,        // COMPRESSOR_RANGE
    50,        // HIGH_TEMP_WARNING
    15,        // ANTI_FREEZE_TEMP
    15,        // ANTI_FREEZE_RANGE
    5,         // COMPRESSOR_REST_TIME
    100,       // FILTER_WARNING
    85,        // FAN2_TEMP
    999999999, // DEVICE_SERIAL_ID
    999999999, // CUSTOMER_ID
    999999999, // ACTIVATION_DATE
    3650,      // SERVICE_INTERVAL
    87600      // HOURS_ELAPSED
};

// Length limit for strings
const int stringMaxLengths[] = {
    32, // WIFI_SSID
    32, // WIFI_PASSWORD
    16, // AP_NAME
    16, // AP_PASSWORD
    64, // SERVER_URL
    15, // SERVER_IP
    32, // INLET_SENSOR_ADDRESS
    32, // OUTLET_SENSOR_ADDRESS
    32, // ANTIFREEZE_SENSOR_ADDRESS
    32  // FILTER_SENSOR_ADDRESS
};

// Temporary Arrays to track change flags
bool numericTempSettingsChanged[NUMERIC_SETTING_COUNT] = {false};
bool booleanTempSettingsChanged[BOOLEAN_SETTING_COUNT] = {false};
bool stringTempSettingsChanged[STRING_SETTING_COUNT] = {false};

// -----------------------------------
// Track last set time for each flag
// -----------------------------------
unsigned long numericLastSetTime[NUMERIC_SETTING_COUNT] = {0};
unsigned long booleanLastSetTime[BOOLEAN_SETTING_COUNT] = {0};
unsigned long stringLastSetTime[STRING_SETTING_COUNT] = {0};

// ----------------------------------------------------------------
// Section: Reset All Changed Flags: Base and Temporary to "false"
// ----------------------------------------------------------------

void resetAllChangedFlagsToFalse()
{
    for (int i = 0; i < NUMERIC_SETTING_COUNT; i++)
    {
        numericTempSettingsChanged[i] = false;
    }
    for (int i = 0; i < BOOLEAN_SETTING_COUNT; i++)
    {
        booleanTempSettingsChanged[i] = false;
    }
    for (int i = 0; i < STRING_SETTING_COUNT; i++)
    {
        stringTempSettingsChanged[i] = false;
    }

    Serial.println("🏁 [Flag] All changed flags reset to --> False ✅");
}

// ---------------------------------------------------------------
// Section: Reset All Changed Flags: Base and Temporary to "true"
// ---------------------------------------------------------------

void resetAllChangedFlagsToTrue()
{
    for (int i = 0; i < NUMERIC_SETTING_COUNT; i++)
    {
        numericTempSettingsChanged[i] = true;
    }
    for (int i = 0; i < BOOLEAN_SETTING_COUNT; i++)
    {
        booleanTempSettingsChanged[i] = true;
    }
    for (int i = 0; i < STRING_SETTING_COUNT; i++)
    {
        stringTempSettingsChanged[i] = true;
    }

    Serial.println("🏁 [Flag] All changed flags reset to --> True ✅");
}

// -----------------------------------
// Section: Initialization
// -----------------------------------
void initializeSettings()
{
    Serial.println("🎛️ [NVS] Initializing settings...🔁");

    if (nvs_flash_init() != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] initialization failed!");

        return;
    }

    nvs_handle_t handle;

    if (nvs_open("storage", NVS_READWRITE, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open!");

        return;
    }

    Serial.println("🎛️ [NVS] opened successfully ✅");

    // Load numeric defaults if missing
    for (int i = 0; i < NUMERIC_SETTING_COUNT; i++)
    {
        char key[16];
        sprintf(key, "num_%d", i);
        int value;
        if (nvs_get_i32(handle, key, &value) != ESP_OK)
        {
            nvs_set_i32(handle, key, defaultNumericValues[i]);

            Serial.printf("🎛️ [NVS] Numeric key '%s' initialized with default value: %d\n", key, defaultNumericValues[i]);
        }
    }

    // Load boolean defaults if missing
    for (int i = 0; i < BOOLEAN_SETTING_COUNT; i++)
    {
        char key[16];
        sprintf(key, "bool_%d", i);
        uint8_t value;
        if (nvs_get_u8(handle, key, &value) != ESP_OK)
        {
            nvs_set_u8(handle, key, defaultBooleanValues[i]);

            Serial.printf("🎛️ [NVS] Boolean key '%s' initialized with default value: %d\n", key, defaultBooleanValues[i]);
        }
    }

    // Load string defaults if missing
    for (int i = 0; i < STRING_SETTING_COUNT; i++)
    {
        char key[16];
        sprintf(key, "str_%d", i);
        size_t length = STRING_MAX_LENGTH;
        char buffer[STRING_MAX_LENGTH];
        if (nvs_get_str(handle, key, buffer, &length) != ESP_OK)
        {
            nvs_set_str(handle, key, defaultStringValues[i]);

            Serial.printf("🎛️ [NVS] String key '%s' initialized with default value: %s\n", key, defaultStringValues[i]);
        }
    }

    nvs_commit(handle);
    nvs_close(handle);
    // resetAllChangedFlags(); // set flags true in start
    resetAllChangedFlagsToTrue(); // set flags true in start

    Serial.println("🎛️ [NVS] Settings initialization completed ✅");
}

// -----------------------------------
// Section: Reset | Erase all NVS and Flag data and reinitialize with default values
// -----------------------------------
void resetSettings()
{

    Serial.println("🎛️ [NVS] Rest Setting to Default Values... 🔁");

    nvs_flash_erase();
    nvs_flash_init();
    initializeSettings();
    // resetAllChangedFlags(); // set flags false in reset
    resetAllChangedFlagsToTrue(); // set flags true in reset

    Serial.println("🎛️ [NVS] Rest Setting Done ✅");
}

// -----------------------------------
// Section: Set in NVS Functions
// -----------------------------------
bool setNumericSetting(NumericIndex index, int value)
{
    if (index < 0 || index >= NUMERIC_SETTING_COUNT)
    {

        Serial.println("🎛️ ❌ [NVS] Invalid numeric index!");

        return false;
    }

    if (value < numericMinValues[index] || value > numericMaxValues[index])
    {

        Serial.printf("🎛️ ❌ [NVS] Value for 'num_%d' is out of bounds. Valid range is: %d to %d\n", index, numericMinValues[index], numericMaxValues[index]);

        return false;
    }

    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READWRITE, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open!");

        return false;
    }

    char key[16];
    sprintf(key, "num_%d", index);
    nvs_set_i32(handle, key, value);
    nvs_commit(handle);
    nvs_close(handle);

    numericTempSettingsChanged[index] = true; // Update the change flag - Temporary
    numericLastSetTime[index] = millis();     // Record the time of the change

    Serial.printf("🔢 [NVS] Numeric key '%s' updated to value: %d\n", key, value);

    return true;
}

bool setBooleanSetting(BooleanIndex index, bool value)
{
    if (index < 0 || index >= BOOLEAN_SETTING_COUNT)
    {

        Serial.println("🎛️ ❌ [NVS] Invalid boolean index!");

        return false;
    }

    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READWRITE, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open NVS!");

        return false;
    }

    char key[16];
    sprintf(key, "bool_%d", index);
    nvs_set_u8(handle, key, value);
    nvs_commit(handle);
    nvs_close(handle);

    booleanTempSettingsChanged[index] = true; // Update the change flag - Temporary
    booleanLastSetTime[index] = millis();     // Record the time of the change

    Serial.printf("🎛️ [NVS] Boolean key '%s' updated to value: %d\n", key, value);

    return true;
}

bool setStringSetting(StringIndex index, const char *value)
{
    // Check if the index is valid
    if (index < 0 || index >= STRING_SETTING_COUNT)
    {

        Serial.println("🎛️ ❌ [NVS] Invalid string index!");

        return false;
    }

    // Check if the value corresponds to the SERVER_IP setting
    if (index == SERVER_IP)
    {
        // Validate the IP address
        Serial.print("Validating IP: ");
        Serial.println(value);

        // IP validation directly inside this function
        int parts[4]; // Array to store the 4 parts of the IP
        int count = 0;

        // Make a copy of the IP string because strtok modifies the original
        char ipCopy[16];
        strncpy(ipCopy, value, sizeof(ipCopy));
        ipCopy[sizeof(ipCopy) - 1] = '\0'; // Ensure null termination

        // Split the string by dots
        char *token = strtok(ipCopy, ".");

        while (token != NULL)
        {
            if (count >= 4)
                return false;             // No more than 4 parts
            parts[count++] = atoi(token); // Convert each part to an integer
            token = strtok(NULL, ".");
        }

        if (count != 4)
            return false; // IP must have exactly 4 parts

        // Check if each part is between 0 and 255
        for (int i = 0; i < 4; i++)
        {
            if (parts[i] < 0 || parts[i] > 255)
                return false;
        }
    }

    // Check if the string exceeds the maximum length allowed
    if (strlen(value) > stringMaxLengths[index])
    {

        Serial.printf("🎛️ ❌ [NVS] String value for 'str_%d' is too long. Max length is: %d\n", index, stringMaxLengths[index]);

        return false;
    }

    // Open NVS for reading and writing
    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READWRITE, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open NVS!");

        return false;
    }

    // Create a key for the setting in NVS
    char key[16];
    sprintf(key, "str_%d", index);

    // Set the string value in NVS
    nvs_set_str(handle, key, value);
    nvs_commit(handle);
    nvs_close(handle);

    stringTempSettingsChanged[index] = true; // Update the change flag - Temporary
    stringLastSetTime[index] = millis();     // Record the time of the change

    Serial.printf("🔠 [NVS] String key '%s' updated to value: %s\n", key, value);

    return true;
}

// -----------------------------------
// Section: Get from NVS Functions
// -----------------------------------
int getNumericSetting(NumericIndex index)
{
    if (index < 0 || index >= NUMERIC_SETTING_COUNT)
    {

        Serial.println("🎛️ ❌ [NVS] Invalid numeric index!");

        return -1;
    }

    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READONLY, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open NVS!");

        return -1;
    }

    char key[16];
    sprintf(key, "num_%d", index);
    int value;
    if (nvs_get_i32(handle, key, &value) != ESP_OK)
    {

        Serial.printf("🔁 [NVS] Key '%s' not found, using default value: %d\n", key, defaultNumericValues[index]);

        value = defaultNumericValues[index];
    }
    nvs_close(handle);

    Serial.printf("🔢 [NVS] Numeric key '%s' retrieved with value: %d\n", key, value);

    return value;
}

bool getBooleanSetting(BooleanIndex index)
{
    if (index < 0 || index >= BOOLEAN_SETTING_COUNT)
    {

        Serial.println("🎛️ ❌ [NVS] Invalid boolean index!");

        return false;
    }

    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READONLY, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open NVS!");

        return false;
    }

    char key[16];
    sprintf(key, "bool_%d", index);
    uint8_t value;
    if (nvs_get_u8(handle, key, &value) != ESP_OK)
    {

        Serial.printf("🔁 [NVS] Key '%s' not found, using default value: %d\n", key, defaultBooleanValues[index]);

        value = defaultBooleanValues[index];
    }
    nvs_close(handle);

    Serial.printf("🎛️ [NVS] Boolean key '%s' retrieved with value: %d\n", key, value);

    return value;
}

bool getStringSetting(StringIndex index, char *buffer, size_t bufferSize)
{
    if (index < 0 || index >= STRING_SETTING_COUNT)
    {

        Serial.println("🎛️ ❌ [NVS] Invalid string index!");

        return false;
    }

    nvs_handle_t handle;
    if (nvs_open("storage", NVS_READONLY, &handle) != ESP_OK)
    {

        Serial.println("🎛️ ❌ [NVS] Failed to open NVS!");

        return false;
    }

    char key[16];
    sprintf(key, "str_%d", index);
    size_t length = bufferSize;
    if (nvs_get_str(handle, key, buffer, &length) != ESP_OK)
    {

        Serial.printf("🔁 [NVS] Key '%s' not found, using default value: %s\n", key, defaultStringValues[index]);

        strncpy(buffer, defaultStringValues[index], bufferSize);
    }
    nvs_close(handle);

    Serial.printf("🔠 [NVS] String key '%s' retrieved with value: %s\n", key, buffer);

    return true;
}

// --------------------------------------------------
// Section: Reset Changed Flag to "false" - Temporary
// --------------------------------------------------

bool resetChangedFlagTemp(const char *type, int index)
{
    unsigned long currentTime = millis();

    if (strcmp(type, "numeric") == 0 && index >= 0 && index < NUMERIC_SETTING_COUNT)
    {
        if (numericTempSettingsChanged[index] && (currentTime - numericLastSetTime[index] >= 60000))
        {
            numericTempSettingsChanged[index] = false;
            Serial.printf("🎛️ ✅ [Flag] Numeric setting flag for index %d reset after 60 seconds.\n", index);
            return true;
        }
    }
    else if (strcmp(type, "boolean") == 0 && index >= 0 && index < BOOLEAN_SETTING_COUNT)
    {
        if (booleanTempSettingsChanged[index] && (currentTime - booleanLastSetTime[index] >= 60000))
        {
            booleanTempSettingsChanged[index] = false;
            Serial.printf("🎛️ ✅ [Flag] Boolean setting flag for index %d reset after 60 seconds.\n", index);
            return true;
        }
    }
    else if (strcmp(type, "string") == 0 && index >= 0 && index < STRING_SETTING_COUNT)
    {
        if (stringTempSettingsChanged[index] && (currentTime - stringLastSetTime[index] >= 60000))
        {
            stringTempSettingsChanged[index] = false;
            Serial.printf("🎛️ ✅ [Flag] String setting flag for index %d reset after 60 seconds.\n", index);
            return true;
        }
    }

    // If no condition met, do nothing
    return false;
}

// ---------------------------------------
// Section: get Changed Flag Temp
// ---------------------------------------
bool getChangedFlagTemp(const char *settingType, int index)
{
    if (index < 0)
    {

        Serial.println("🎛️ ❌ [Flag] Invalid index for reading flag!");

        return false;
    }

    if (strcmp(settingType, "numeric") == 0)
    {
        if (index >= NUMERIC_SETTING_COUNT)
        {

            Serial.println("🎛️ ❌ [Flag] Invalid numeric index for reading!");

            return false;
        }

        Serial.printf("🏁 [Flag] Numeric setting flag for index %d is: %s\n", index, numericTempSettingsChanged[index] ? "true" : "false");

        return numericTempSettingsChanged[index];
    }
    else if (strcmp(settingType, "boolean") == 0)
    {
        if (index >= BOOLEAN_SETTING_COUNT)
        {

            Serial.println("🎛️ ❌ [Flag] Invalid boolean index for reading!");

            return false;
        }

        Serial.printf("🏁 [Flag] Boolean setting flag for index %d is: %s\n", index, booleanTempSettingsChanged[index] ? "true" : "false");

        return booleanTempSettingsChanged[index];
    }
    else if (strcmp(settingType, "string") == 0)
    {
        if (index >= STRING_SETTING_COUNT)
        {

            Serial.println("🎛️ ❌ [Flag] Invalid string index for reading!");

            return false;
        }

        Serial.printf("🏁 [Flag] String setting flag for index %d is: %s\n", index, stringTempSettingsChanged[index] ? "true" : "false");

        return stringTempSettingsChanged[index];
    }
    else
    {

        Serial.println("🎛️ ❌ [Flag] Invalid type for reading flag! Valid types: 'numeric', 'boolean', 'string'");

        return false;
    }
}