#include <Arduino.h>
#include "setting.h"
#include "display.h"
#include "wifi.h"
#include "relayControl.h"
#include "tempSensor.h"
#include "alarm.h"
#include "general.h"
#include <Adafruit_NeoPixel.h>

// Built in RGB
#define PIN_NEOPIXEL 48
#define NUM_PIXELS 1

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

void setup()
{
  Serial.begin(115200);

  // Initialize RGB LED
  pixels.begin();
  pixels.clear();
  pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // RGB Green
  pixels.show();

  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  Serial.println("----------------- Start Setup -----------------");
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

  // Setup Relays
  setupRelays();

  Serial.println("-------------------------Setup Relays--------------------------");

  // Setup Display
  displaySetup();

  Serial.println("-------------------------Setup Display--------------------------");

  // Initialize Settings (NVS)
  initializeSettings();

  Serial.println("--------------------------Initialize Settings-------------------------");

  // Setup Sensors
  initializeSensors();

  Serial.println("-------------------------Setup Sensors--------------------------");

  // Setup Wi-Fi
  wifiSetup();

  Serial.println("-------------------------Setup Wi-Fi--------------------------");

  // Initialize the Alarm system
  alarmSetup();

  Serial.println("--------------------------Alarm System-------------------------");

  // Update the Time
  setupAndUpdateTime();

  Serial.println("------------------------Updating Time---------------------------");

  // Initialize the maintenance counter
  initializeHoursElapsedTracking();

  Serial.println("------------------------Maintenance Counter---------------------------");

  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");
  Serial.println("----------------- End of Setup -----------------");
  Serial.println("!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!!");

  // Indicate end of setup with RGB Red
  pixels.setBrightness(64);
  pixels.setPixelColor(0, pixels.Color(64, 0, 0));
  pixels.show();
}

void loop()
{
  displayLoop(); // Continuously update display (if applicable)

  wifiLoop(); // Handle Wi-Fi connection checks and reconnection

  handleTemperatureReadings(); // Handle temperature sensor readings

  controlRelays(); // Handle Relay Control

  updateHoursElapsedCounter(); // Update the maintenance counter periodically
}
