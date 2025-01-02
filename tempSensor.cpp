#include "tempSensor.h"
#include "setting.h"

// Define the GPIO pin for all DS18B20 sensors
#define ONE_WIRE_BUS 4 // Change to the GPIO pin you're using

// OneWire instance for communication
OneWire oneWire(ONE_WIRE_BUS);

// DallasTemperature instance
DallasTemperature ds18b20(&oneWire);

// Define the sensors array (adjust size as needed)
const uint8_t sensorCount = 4; // Number of sensors
DS18B20Sensor sensors[sensorCount];

unsigned long lastTemperatureRequestTime = 0; // Timestamp for last request
const unsigned long requestInterval = 1000;   // Interval between requests in milliseconds

// To keep track of the current sensor being read
uint8_t currentSensorIndex = 0;

bool convertStringToDeviceAddress(const char *addressStr, DeviceAddress &deviceAddress)
{
    char *ptr = (char *)addressStr;
    for (uint8_t i = 0; i < 8; i++)
    {
        // Skip colons
        if (*ptr == ':')
            ptr++;

        // Read two hex digits
        char hex[3] = {ptr[0], ptr[1], '\0'};

        // Convert hex to byte
        char *end;
        long value = strtol(hex, &end, 16);

        // Validate conversion
        if (end == hex || value < 0 || value > 255)
        {
            return false;
        }

        deviceAddress[i] = (uint8_t)value;
        ptr += 2;
    }
    return true;
}

// --------------------------------------
// Function to initialize all sensors
// --------------------------------------
void initializeSensors()
{
    Serial.println("🔁 [Temperature] Initializing DS18B20 sensors...");

    // Start the DallasTemperature library
    ds18b20.begin();

    // Names of the sensors and their corresponding NVS keys (use enum values)
    const char *sensorNames[] = {"Inlet", "Outlet", "Antifreeze", "Filter"};
    StringIndex sensorIndexes[] = {INLET_SENSOR_ADDRESS, OUTLET_SENSOR_ADDRESS, ANTIFREEZE_SENSOR_ADDRESS, FILTER_SENSOR_ADDRESS};

    // Initialize each sensor
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        char addressStr[STRING_MAX_LENGTH];

        // Attempt to get the address for the current sensor from NVS
        if (!getStringSetting(sensorIndexes[i], addressStr, STRING_MAX_LENGTH))
        {
            Serial.print("❌ [Temperature] Failed to read sensor address for: ");
            Serial.println(sensorNames[i]);
            continue; // Skip this sensor initialization
        }

        // Convert the address string to device address
        if (!convertStringToDeviceAddress(addressStr, sensors[i].address))
        {
            Serial.print("❌ [Temperature] Invalid address format for: ");
            Serial.println(sensorNames[i]);
            continue; // Skip this sensor if address is invalid
        }

        // Assign the name of the sensor
        sensors[i].name = sensorNames[i];

        // Print sensor information
        Serial.print("✅ [Temperature] ");
        Serial.print(sensors[i].name);
        Serial.print(" sensor initialized | Address: ");
        for (uint8_t j = 0; j < 8; j++)
        {
            Serial.print(sensors[i].address[j], HEX);
            if (j < 7)
                Serial.print(":"); // Add colon between bytes
        }
        Serial.println();
    }

    Serial.println("🌡️ [Temperature] Sensors initialization completed ✅");
    Serial.println("________________________________________________");
}

// --------------------------------------
// Function to handle reading temperatures in a non-blocking manner
// --------------------------------------
void handleTemperatureReadings()
{
    // Check if it's time to request temperatures
    if (millis() - lastTemperatureRequestTime >= requestInterval)
    {
        // Request temperature for the current sensor
        ds18b20.requestTemperatures();
        float temperature = ds18b20.getTempC(sensors[currentSensorIndex].address);

        // Check for valid temperature
        if (temperature != DEVICE_DISCONNECTED_C)
        {
            Serial.print("🌡️ [Temperature] ");
            Serial.print(sensors[currentSensorIndex].name);
            Serial.print(": ");
            Serial.print(temperature);
            Serial.println(" °C");
        }
        else
        {
            Serial.print("❌ [Temperature] Failed to read temperature for sensor: ");
            Serial.println(sensors[currentSensorIndex].name);
        }

        // Move to the next sensor
        currentSensorIndex = (currentSensorIndex + 1) % sensorCount;

        // Update the timestamp
        lastTemperatureRequestTime = millis();
    }
}

// --------------------------------------
// Function to read temperature from a specific sensor by name
// --------------------------------------
float readTemperatureByName(String name)
{
    // Loop through the sensors to find the one with the given name
    for (uint8_t i = 0; i < sensorCount; i++)
    {
        if (sensors[i].name == name)
        {
            // Request temperature for the specific sensor
            float temperature = ds18b20.getTempC(sensors[i].address);

            // Check for valid temperature
            if (temperature != DEVICE_DISCONNECTED_C)
            {
                Serial.print("🌡️ [Temperature] ");
                Serial.print(name);
                Serial.print(": ");
                Serial.print(temperature);
                Serial.println(" °C");

                return temperature;
            }
            else
            {
                Serial.print("❌ [Temperature] Failed to read temperature for sensor: ");
                Serial.println(name);
                return NAN;
            }
        }
    }

    // If no sensor with the given name is found
    Serial.print("❌ [Temperature] Sensor not found with name: ");
    Serial.println(name);
    return NAN;
}