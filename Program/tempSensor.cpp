#include "tempSensor.h"
#include "setting.h"

#define ONE_WIRE_BUS 4 // sensor GPIO pin

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

const uint8_t sensorCount = 4; // number of sensor
DS18B20Sensor sensors[sensorCount];

const unsigned long TEMPERATURE_READ_INTERVAL = 15000; // reading new data interval
unsigned long lastTemperatureRequestTime = 0;
bool conversionRequested = false;
const uint16_t CONVERSION_TIME = 750; // connection time

// handle Sensor address in NVS
bool convertStringToDeviceAddress(const char *addressStr, DeviceAddress &deviceAddress)
{
    Serial.print("🔍 [Temperature] Converting address string: ");
    Serial.println(addressStr);

    char *ptr = (char *)addressStr;
    for (uint8_t i = 0; i < 8; i++)
    {
        if (*ptr == ':')
            ptr++;
        char hex[3] = {ptr[0], ptr[1], '\0'};
        char *end;
        long value = strtol(hex, &end, 16);

        if (end == hex || value < 0 || value > 255)
        {
            Serial.println("❌ [Temperature] Invalid hex value in address");
            return false;
        }
        deviceAddress[i] = (uint8_t)value;
        ptr += 2;
    }
    Serial.println("✅ [Temperature] Address conversion successful");
    return true;
}

// initialize Sensors in setup
void initializeSensors()
{
    Serial.println("\n🔄 [Temperature] Starting sensor initialization...");

    Serial.println("🔄 [Temperature] Beginning Dallas Temperature library");
    ds18b20.begin();

    Serial.println("🔄 [Temperature] Setting resolution to 12 bits");
    ds18b20.setResolution(12);

    Serial.println("🔄 [Temperature] Enabling async mode");
    ds18b20.setWaitForConversion(false);

    const char *sensorNames[] = {"Inlet", "Outlet", "Antifreeze", "Filter"};
    StringIndex sensorIndexes[] = {INLET_SENSOR_ADDRESS, OUTLET_SENSOR_ADDRESS, ANTIFREEZE_SENSOR_ADDRESS, FILTER_SENSOR_ADDRESS};

    uint8_t successCount = 0;

    for (uint8_t i = 0; i < sensorCount; i++)
    {
        Serial.print("\n🔄 [Temperature] Initializing sensor ");
        Serial.print(i + 1);
        Serial.print(" of ");
        Serial.println(sensorCount);

        char addressStr[STRING_MAX_LENGTH];

        Serial.print("🔄 [Temperature] Reading address for: ");
        Serial.println(sensorNames[i]);

        if (!getStringSetting(sensorIndexes[i], addressStr, STRING_MAX_LENGTH))
        {
            Serial.print("❌ [Temperature] Failed to read address from settings for: ");
            Serial.println(sensorNames[i]);
            continue;
        }

        if (!convertStringToDeviceAddress(addressStr, sensors[i].address))
        {
            Serial.print("❌ [Temperature] Invalid address format for: ");
            Serial.println(sensorNames[i]);
            continue;
        }

        sensors[i].name = sensorNames[i];

        Serial.print("🔄 [Temperature] Checking connection for: ");
        Serial.println(sensorNames[i]);

        if (!ds18b20.isConnected(sensors[i].address))
        {
            Serial.print("❌ [Temperature] Sensor not connected: ");
            Serial.println(sensorNames[i]);
            continue;
        }

        Serial.print("🌡️ [Temperature] ");
        Serial.print(sensors[i].name);
        Serial.print(" initialized | Address: ");
        for (uint8_t j = 0; j < 8; j++)
        {
            Serial.print(sensors[i].address[j], HEX);
            if (j < 7)
                Serial.print(":");
        }
        Serial.println();

        successCount++;
    }

    Serial.print("\n🌡️ [Temperature] Initialization complete: ");
    Serial.print(successCount);
    Serial.print("/");
    Serial.print(sensorCount);
    Serial.println(" sensors initialized");

    Serial.println("________________________________________________");

    // get first temp
    Serial.println("\n🔄 [Temperature] Reading initial values for all sensors...");
    ds18b20.requestTemperatures();
    delay(CONVERSION_TIME);

    for (uint8_t i = 0; i < sensorCount; i++)
    {
        float temperature = ds18b20.getTempC(sensors[i].address);

        if (temperature != DEVICE_DISCONNECTED_C && temperature > -55 && temperature < 125)
        {
            sensors[i].lastValidTemperature = temperature;
            sensors[i].lastReadTime = millis();

            Serial.print("🌡️ [Temperature] ");
            Serial.print(sensors[i].name);
            Serial.print(": ");
            Serial.print(temperature);
            Serial.println(" °C");
        }
        else
        {
            Serial.print("❌ [Temperature] Failed to read initial value for ");
            Serial.println(sensors[i].name);
        }
    }
    Serial.println("________________________________________________");
}

// handle temp requests with cache
void handleTemperatureReadings()
{
    unsigned long currentMillis = millis();

    // Request temperature conversion if it's time to do so
    if (!conversionRequested && (currentMillis - lastTemperatureRequestTime >= TEMPERATURE_READ_INTERVAL))
    {
        Serial.println(" ");
        Serial.println("---------------Handle Temperature Cache---------------");
        Serial.println("\n🌡️ [Temperature] Requesting temperature conversion");
        ds18b20.requestTemperatures();
        conversionRequested = true;
        lastTemperatureRequestTime = currentMillis;
        return;
    }

    // Read the temperature values after conversion is complete
    if (conversionRequested && (currentMillis - lastTemperatureRequestTime >= CONVERSION_TIME))
    {
        Serial.println(" ");
        Serial.println("---------------Handle Temperature Cache---------------");
        Serial.println("🔄 [Temperature] Reading all sensors:");
        uint8_t validReadings = 0;

        // Loop through all sensors
        for (uint8_t i = 0; i < sensorCount; i++)
        {
            // Get the temperature reading from the sensor
            float temperature = ds18b20.getTempC(sensors[i].address);

            // Check if the reading is valid
            if (temperature != DEVICE_DISCONNECTED_C && temperature > -55 && temperature < 125 && !isnan(temperature))
            {
                // Valid reading, print and save the temperature
                Serial.print("🌡️ [Temperature] ");
                Serial.print(sensors[i].name);
                Serial.print(": ");
                Serial.print(temperature);
                Serial.println(" °C");

                sensors[i].lastValidTemperature = temperature;
                sensors[i].lastReadTime = currentMillis;
                validReadings++;
            }
            else
            {
                // Invalid reading, set to NaN
                Serial.println(" ");
                Serial.println("---------------Handle Temperature Cache---------------");
                Serial.print("❌ [Temperature] Invalid reading for ");
                Serial.print(sensors[i].name);
                Serial.print(" (");
                Serial.print(temperature);
                Serial.println(" °C)");

                // Set temperature to NaN if invalid
                temperature = NAN;
                sensors[i].lastValidTemperature = temperature;
                sensors[i].lastReadTime = currentMillis;
            }
        }

        // Log the number of valid readings
        Serial.println(" ");
        Serial.println("---------------Handle Temperature Cache---------------");
        Serial.print("🌡️ [Temperature] Valid readings: ");
        Serial.print(validReadings);
        Serial.print("/");
        Serial.println(sensorCount);
        Serial.println("________________________________________________");

        // Reset the flag to allow the next temperature request
        conversionRequested = false;
    }
}

// function for access every sensor data with name
float readTemperatureByName(String name)
{
    Serial.print("\n🌡️ [Temperature] Reading temperature for: ");
    Serial.println(name);

    unsigned long currentMillis = millis();

    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (String(sensors[i].name) == name)
        {
            if (currentMillis - sensors[i].lastReadTime < TEMPERATURE_READ_INTERVAL * 2)
            {
                Serial.print("🌡️ [Temperature] Using cached value for ");
                Serial.print(name);
                Serial.print(": ");
                Serial.print(sensors[i].lastValidTemperature);
                Serial.println(" °C");
                return sensors[i].lastValidTemperature;
            }

            Serial.println("🌡️ [Temperature] Requesting new reading");
            ds18b20.requestTemperatures();
            delay(CONVERSION_TIME);

            float temperature = ds18b20.getTempC(sensors[i].address);

            if (temperature != DEVICE_DISCONNECTED_C && temperature > -55 && temperature < 125)
            {
                Serial.print("🌡️ [Temperature] ");
                Serial.print(name);
                Serial.print(": ");
                Serial.print(temperature);
                Serial.println(" °C");

                sensors[i].lastValidTemperature = temperature;
                sensors[i].lastReadTime = currentMillis;
                return temperature;
            }

            Serial.print("❌ [Temperature] Invalid reading for ");
            Serial.println(name);
            return NAN;
        }
    }

    Serial.print("❌ [Temperature] Sensor not found: ");
    Serial.println(name);
    return NAN;
}

// Detect and Counting Sensors
int detectDS18B20Sensors(void)
{
    OneWire oneWire(ONE_WIRE_BUS);
    int sensorCount = 0;
    byte addr[8];

    Serial.println("\n🔍 [Temperature] Scanning for DS18B20 sensors...");

    // Reset the OneWire bus first
    oneWire.reset_search();
    delay(250);

    // Search for devices on the bus
    while (oneWire.search(addr))
    {
        // Check if address is valid
        if (OneWire::crc8(addr, 7) != addr[7])
        {
            Serial.println("❌ [Temperature] Invalid CRC!");
            continue;
        }

        // Check if device is a DS18B20
        if (addr[0] != 0x28)
        {
            Serial.println("❌ [Temperature] Device is not a DS18B20!");
            continue;
        }

        // Print device address
        Serial.print("✅ [Temperature] Sensor ");
        Serial.print(sensorCount + 1);
        Serial.print(" found | Address: ");
        for (int i = 0; i < 8; i++)
        {
            if (addr[i] < 0x10)
                Serial.print("0");
            Serial.print(addr[i], HEX);
            if (i < 7)
                Serial.print(":");
        }
        Serial.println();

        sensorCount++;
    }

    Serial.print("\n🔍 [Temperature] Total sensors found: ");
    Serial.println(sensorCount);
    Serial.println("________________________________________________");

    return sensorCount;
}

// setup sensors with ui
int getFirstDS18B20Address(char *addressStr, int maxLen)
{
    if (maxLen < 24)
    { // Need at least 23 chars + null terminator
        return 0;
    }

    OneWire oneWire(ONE_WIRE_BUS);
    byte addr[8];
    char existingAddr[24];
    bool found = false;

    Serial.println("\n🔍 [Temperature] Scanning for DS18B20 sensors...");

    // Reset the OneWire bus first
    oneWire.reset_search();
    delay(250);

    // Get existing addresses from NVS
    bool hasInlet = getStringSetting(INLET_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
    bool hasOutlet = getStringSetting(OUTLET_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
    bool hasAntifreeze = getStringSetting(ANTIFREEZE_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
    bool hasFilter = getStringSetting(FILTER_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));

    // Search for devices
    while (oneWire.search(addr))
    {
        // Check if address is valid
        if (OneWire::crc8(addr, 7) != addr[7])
        {
            continue;
        }

        // Check if device is a DS18B20
        if (addr[0] != 0x28)
        {
            continue;
        }

        // Convert current address to string format
        char currentAddr[24];
        int pos = 0;
        for (int i = 0; i < 8; i++)
        {
            if (i > 0)
            {
                currentAddr[pos++] = ':';
            }
            sprintf(&currentAddr[pos], "%02X", addr[i]);
            pos += 2;
        }
        currentAddr[pos] = '\0';

        // Check if this address is already stored in NVS
        bool isNewAddress = true;
        if (hasInlet)
        {
            getStringSetting(INLET_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
            if (strcmp(currentAddr, existingAddr) == 0)
            {
                isNewAddress = false;
            }
        }
        if (hasOutlet)
        {
            getStringSetting(OUTLET_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
            if (strcmp(currentAddr, existingAddr) == 0)
            {
                isNewAddress = false;
            }
        }
        if (hasAntifreeze)
        {
            getStringSetting(ANTIFREEZE_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
            if (strcmp(currentAddr, existingAddr) == 0)
            {
                isNewAddress = false;
            }
        }
        if (hasFilter)
        {
            getStringSetting(FILTER_SENSOR_ADDRESS, existingAddr, sizeof(existingAddr));
            if (strcmp(currentAddr, existingAddr) == 0)
            {
                isNewAddress = false;
            }
        }

        // If this is a new address, return it
        if (isNewAddress)
        {
            strcpy(addressStr, currentAddr);
            Serial.print("✅ [Temperature] New sensor found | Address: ");
            Serial.println(addressStr);
            Serial.println("________________________________________________");
            return 1;
        }
    }

    Serial.println("❌ [Temperature] No new sensors found!");
    Serial.println("________________________________________________");
    return 0;
}