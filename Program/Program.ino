#include <Arduino.h>
#include "setting.h"
#include "display.h"
#include "wifi.h"
#include "relayControl.h"
#include "tempSensor.h"
#include "alarm.h"
#include "general.h"
#include <Adafruit_NeoPixel.h>
#include <esp_heap_caps.h>

// Built in RGB
#define PIN_NEOPIXEL 48
#define NUM_PIXELS 1

Adafruit_NeoPixel pixels(NUM_PIXELS, PIN_NEOPIXEL, NEO_GRB + NEO_KHZ800);

// ============================ System Info =====================

unsigned long lastshowMemoryUsageUpdateTime = 0;
const long showMemoryUsageupdateInterval = 5000;

void showMemoryUsage()
{
  Serial.println("...........................");
  size_t freeHeap = ESP.getFreeHeap();
  size_t totalHeap = ESP.getHeapSize();

  Serial.println("🟥 [System] Memory Usage 🟥");
  Serial.print("Free Heap: ");
  Serial.print(freeHeap / 1024.0, 2);
  Serial.println(" KB");

  Serial.print("Total Heap: ");
  Serial.print(totalHeap / 1024.0, 2);
  Serial.println(" KB");

  Serial.print("Used Heap: ");
  Serial.print((totalHeap - freeHeap) / 1024.0, 2);
  Serial.println(" KB");

  Serial.println("");

  size_t freePSRAM = heap_caps_get_free_size(MALLOC_CAP_SPIRAM);
  size_t totalPSRAM = heap_caps_get_total_size(MALLOC_CAP_SPIRAM);

  Serial.print("Free PSRAM: ");
  Serial.print(freePSRAM / 1024.0, 2);
  Serial.println(" KB");

  Serial.print("Total PSRAM: ");
  Serial.print(totalPSRAM / 1024.0, 2);
  Serial.println(" KB");

  Serial.print("Used PSRAM: ");
  Serial.print((totalPSRAM - freePSRAM) / 1024.0, 2);
  Serial.println(" KB");
  Serial.println("...........................");
}

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

  showMemoryUsage();

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

  unsigned long currentMillis = millis(); // Resource usage Monitor

  if (currentMillis - lastshowMemoryUsageUpdateTime >= showMemoryUsageupdateInterval)
  {
    lastshowMemoryUsageUpdateTime = currentMillis;
    showMemoryUsage();
  }
}
