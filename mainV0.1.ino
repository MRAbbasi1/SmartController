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
    pixels.setPixelColor(0, pixels.Color(0, 255, 0)); // Green
    pixels.show();

    Serial.println("________________ Starting Setup ________________");

    // 0- Setup Display
    displaySetup();

    Serial.println("________________________________________________");

    // 1- Initialize Settings (NVS)
    initializeSettings();

    Serial.println("________________________________________________");

    // 2- Setup Sensors (DHT11, DS18B20, DHT22)
    initializeSensors();

    Serial.println("________________________________________________");

    // 3- Setup Wi-Fi
    wifiSetup();

    Serial.println("________________________________________________");

    // 4- Initialize the Alarm system
    alarmSetup();

    Serial.println("________________________________________________");

    // 5- Setup Relays
    setupRelays();

    Serial.println("________________________________________________");

    // 6- Update the Time
    setupAndUpdateTime();

    Serial.println("________________________________________________");

    // Indicate end of setup with RGB Red
    pixels.setBrightness(64);
    pixels.setPixelColor(0, pixels.Color(64, 0, 0));
    pixels.show();

    Serial.println("_________________ End of setup _________________");
}

void loop()
{
    displayLoop(); // Continuously update display (if applicable)

    wifiLoop(); // Handle Wi-Fi connection checks and reconnection

    controlRelays(); // Handle Relay Control
}
