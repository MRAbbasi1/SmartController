#include "tempSensor.h"
#include "setting.h"

#define ONE_WIRE_BUS 4

OneWire oneWire(ONE_WIRE_BUS);
DallasTemperature ds18b20(&oneWire);

const uint8_t sensorCount = 4;
DS18B20Sensor sensors[sensorCount];

const unsigned long TEMPERATURE_READ_INTERVAL = 30000;
unsigned long lastTemperatureRequestTime = 0;
bool conversionRequested = false;
const uint16_t CONVERSION_TIME = 750;

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

void handleTemperatureReadings()
{
    unsigned long currentMillis = millis();

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

    if (conversionRequested && (currentMillis - lastTemperatureRequestTime >= CONVERSION_TIME))
    {
        Serial.println(" ");
        Serial.println("---------------Handle Temperature Cache---------------");
        Serial.println("🔄 [Temperature] Reading all sensors:");
        uint8_t validReadings = 0;

        for (uint8_t i = 0; i < sensorCount; i++)
        {
            float temperature = ds18b20.getTempC(sensors[i].address);

            if (temperature != DEVICE_DISCONNECTED_C && temperature > -55 && temperature < 125)
            {
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
                Serial.println(" ");
                Serial.println("---------------Handle Temperature Cache---------------");
                Serial.print("❌ [Temperature] Invalid reading for ");
                Serial.print(sensors[i].name);
                Serial.print(" (");
                Serial.print(temperature);
                Serial.println(" °C)");
            }
        }
        Serial.println(" ");
        Serial.println("---------------Handle Temperature Cache---------------");
        Serial.print("🌡️ [Temperature] Valid readings: ");
        Serial.print(validReadings);
        Serial.print("/");
        Serial.println(sensorCount);
        Serial.println("________________________________________________");

        conversionRequested = false;
    }
}

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