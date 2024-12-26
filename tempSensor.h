#ifndef TEMP_SENSOR_H
#define TEMP_SENSOR_H

#include <Arduino.h>
#include <OneWire.h>
#include <DallasTemperature.h>

// Define DS18B20 sensor structure
struct DS18B20Sensor
{
    String name;              // Name of the sensor (e.g., Inlet, Outlet)
    DeviceAddress address;    // Unique address of the sensor
};

// Declare global variables for sensors
extern DS18B20Sensor sensors[];  // Array of DS18B20 sensors
extern const uint8_t sensorCount; // Number of sensors

// Function prototypes
void initializeSensors();                 // Function to initialize all sensors
float readTemperatureByName(String name); // Function to read temperature from a sensor by name

#endif
