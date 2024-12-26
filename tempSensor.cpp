#include "tempSensor.h"

// Define the GPIO pin for all DS18B20 sensors
#define ONE_WIRE_BUS 4 // Change to the GPIO pin you're using

// OneWire instance for communication
OneWire oneWire(ONE_WIRE_BUS);

// DallasTemperature instance
DallasTemperature ds18b20(&oneWire);

// Define the sensors array (adjust size as needed)
const uint8_t sensorCount = 4; // Number of sensors
DS18B20Sensor sensors[sensorCount];

// --------------------------------------
// Function to initialize all sensors
// --------------------------------------
void initializeSensors()
{
    Serial.println("🔁 [Temperature] Initializing DS18B20 sensors...");
    delay(500); // Small delay for better readability in serial monitor

    // Start the DallasTemperature library
    ds18b20.begin();
    delay(500);

    // Get the number of devices on the OneWire bus
    int deviceCount = ds18b20.getDeviceCount();
    if (deviceCount == 0)
    {
        Serial.println("❌ [Temperature] No DS18B20 devices detected!");
        return;
    }

    // Check if the number of detected sensors matches the expected count
    if (deviceCount != sensorCount)
    {
        Serial.print("⛔️ [Temperature] Warning: Expected ");
        Serial.print(sensorCount);
        Serial.print(" sensors but found ");
        Serial.println(deviceCount);
        delay(500);
    }

    // Assign names and addresses to sensors
    for (uint8_t i = 0; i < deviceCount; i++) // Loop through detected devices
    {
        // Get the address of the sensor
        if (ds18b20.getAddress(sensors[i].address, i))
        {
            // Assign names based on index
            switch (i)
            {
            case 0:
                sensors[i].name = "Inlet";
                break;
            case 1:
                sensors[i].name = "Outlet";
                break;
            case 2:
                sensors[i].name = "Antifreeze";
                break;
            case 3:
                sensors[i].name = "Filter";
                break;
            }

            // Print sensor information
            Serial.print("✅ [Temperature] ");
            Serial.print(sensors[i].name);
            Serial.print(" sensor initialized | Address: ");
            for (uint8_t j = 0; j < 8; j++)
            {
                Serial.print(sensors[i].address[j], HEX);
                if (j < 7)
                    Serial.print(":");
            }
            Serial.println();

            // Delay to ensure readability
            delay(500);
        }
        else
        {
            Serial.print("❌ [Temperature] Failed to get address for sensor ");
            Serial.println(i + 1);
            delay(500);
        }
    }

    // Print completion message
    Serial.println("🌡️ [Temperature] Sensors initialization completed ✅");
    Serial.println("________________________________________________");
    delay(500);
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
            // Request temperature from all sensors
            ds18b20.requestTemperatures();
            float temperature = ds18b20.getTempC(sensors[i].address);

            // Check for valid temperature
            if (temperature != DEVICE_DISCONNECTED_C)
            {
                // Print the temperature and address for the specific sensor
                Serial.print("🌡️ [Temperature] ");
                Serial.print(name);
                Serial.print(": ");
                Serial.print(temperature);
                Serial.print(" °C | Address: ");

                // Print the address in hexadecimal format
                for (uint8_t j = 0; j < 8; j++)
                {
                    Serial.print(sensors[i].address[j], HEX);
                    if (j < 7)
                        Serial.print(":"); // Add colon between bytes
                }
                Serial.println();

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
