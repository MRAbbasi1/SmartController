#include "general.h"
#include "wifi.h"
#include <ArduinoJson.h> // Include the ArduinoJson library for JSON parsing
#include "setting.h"
#include "relayControl.h"

// ======== Cache Variables ==========
static time_t cachedEpoch = 0;             // Cached epoch time from last update
static unsigned long lastUpdateMillis = 0; // Time when the epoch was last updated
static bool timeIsValid = false;           // Flag to check if time is valid

// ======== Helper Functions ========

// Helper: Convert epoch to formatted string (YYYY/MM/DD-HH:MM)
String formatTime(time_t epoch)
{
    struct tm *timeinfo = localtime(&epoch);
    char buffer[20];
    snprintf(buffer, sizeof(buffer), "%04d/%02d/%02d-%02d:%02d",
             timeinfo->tm_year + 1900, timeinfo->tm_mon + 1, timeinfo->tm_mday,
             timeinfo->tm_hour, timeinfo->tm_min);
    return String(buffer);
}

// Helper: Update the cached time (epoch) with the current time
void updateCachedTime(time_t epoch)
{
    cachedEpoch = epoch;
    lastUpdateMillis = millis();
    timeIsValid = true;
    Serial.println("⏱️ [Time] Time successfully updated and cached: " + formatTime(epoch));
}

// ======== Time Retrieval Functions ========

// Function: Get time from NTP servers
time_t getLocalNTPTime()
{
    configTime(3 * 3600 + 1800, 0, "pool.ntp.org", "time.nist.gov"); // Set up the NTP servers with Iran timezone (UTC+3:30)
    struct tm timeinfo;
    if (!getLocalTime(&timeinfo)) // Get local time using NTP
    {
        Serial.println("❌ [Time] Failed to obtain NTP time.");
        return 0; // Return 0 if NTP fails
    }
    return mktime(&timeinfo); // Convert struct tm to epoch time
}

// Function: Get time from API (World Time API)
time_t getAPITime(const char *apiURL = "http://worldtimeapi.org/api/timezone/Asia/Tehran")
{
    HTTPClient http;
    http.begin(apiURL);        // Initiate HTTP request to the API
    int httpCode = http.GET(); // Send GET request

    if (httpCode != 200) // Check if the request was successful
    {
        Serial.printf("❌ [Time] HTTP request failed with code: %d\n", httpCode);
        return 0; // Return 0 if request fails
    }

    String payload = http.getString(); // Read the response as a string
    http.end();                        // Close the HTTP connection

    // Create a JSON document for parsing
    JsonDocument doc;

    // Deserialize the JSON response
    DeserializationError error = deserializeJson(doc, payload);
    if (error) // Check for deserialization errors
    {
        Serial.println("❌ [Time] Failed to parse JSON.");
        return 0; // Return 0 if JSON parsing fails
    }

    const char *datetime = doc["datetime"].as<const char *>(); // Extract the datetime string from the response
    if (datetime == nullptr)                                   // Check if the datetime is empty
    {
        Serial.println("❌ [Time] Failed to parse API time.");
        return 0; // Return 0 if the datetime is invalid
    }

    // Extract epoch time from the datetime string
    struct tm timeinfo = {0};
    sscanf(datetime, "%4d-%2d-%2dT%2d:%2d:%2d",
           &timeinfo.tm_year, &timeinfo.tm_mon, &timeinfo.tm_mday,
           &timeinfo.tm_hour, &timeinfo.tm_min, &timeinfo.tm_sec);
    timeinfo.tm_year -= 1900; // Adjust year to tm_year format
    timeinfo.tm_mon -= 1;     // Adjust month to tm_mon format

    return mktime(&timeinfo); // Convert struct tm to epoch time
}

// ======== Setup and Time Update ========

// Function: Setup and update time based on NTP or API
void setupAndUpdateTime()
{
    char ssid[32];
    if (!wifiIsConnected(ssid, sizeof(ssid))) // Check if Wi-Fi is connected
    {
        Serial.println("❌ [Time] Wi-Fi not connected, cannot update time.");
        return; // Exit function if not connected to Wi-Fi
    }

    // Try to get time from NTP
    time_t localTime = getLocalNTPTime();
    if (localTime > 0)
    {
        updateCachedTime(localTime); // Update time if NTP request was successful
        return;
    }

    // If NTP fails, try to get time from API
    time_t apiTime = getAPITime();
    if (apiTime > 0)
    {
        updateCachedTime(apiTime); // Update time if API request was successful
        return;
    }

    Serial.println("❌ [Time] All time sources failed. Cached time remains unchanged.");
}

// ======== Get Current Time ========

// Function: Get the current time based on cached epoch time
String getTime()
{
    if (!timeIsValid) // Check if time is valid
    {
        return "Time Unavailable"; // Return a default message if time is invalid
    }

    // Calculate the current epoch time based on cached time and elapsed milliseconds
    unsigned long elapsedMillis = millis() - lastUpdateMillis;
    time_t currentEpoch = cachedEpoch + (elapsedMillis / 1000);

    return formatTime(currentEpoch); // Format and return the current time as a string
}

// ======== System Active Time Counter ========

// Constants
const uint32_t HOUR_INTERVAL = 60 * 60 * 1000; // 1 hour in milliseconds
const uint8_t SAVE_INTERVAL = 1;               // Save every 1 hours
const uint32_t MAX_HOURS = 87600;              // Maximum trackable hours

static uint32_t hoursElapsedCounter = 0;
static uint32_t lastHourUpdateTime = 0;

void initializeHoursElapsedTracking()
{
    Serial.println("------------Setup Hours Elapsed Counter------------");
    hoursElapsedCounter = getNumericSetting(HOURS_ELAPSED);
    lastHourUpdateTime = millis();
    Serial.println("🔧 Hours Elapsed Counter Initialized");
}

void updateHoursElapsedCounter()
{
    uint32_t currentTime = millis();

    // Check if 1 hour has passed and Evaporator Fan is running
    if ((currentTime - lastHourUpdateTime >= HOUR_INTERVAL))
    {
        // Reset the hour timer
        lastHourUpdateTime = currentTime;

        if (getEvaporatorRelayStatus())
        {
            // Increment the counter
            hoursElapsedCounter++;

            // Ensure we don't exceed max limit
            hoursElapsedCounter = min(hoursElapsedCounter, MAX_HOURS);

            // Save only every 1 hours
            if (hoursElapsedCounter % SAVE_INTERVAL == 0)
            {
                setNumericSetting(HOURS_ELAPSED, hoursElapsedCounter);
            }

            Serial.printf("🔧 Hours Elapsed Updated: %d\n", hoursElapsedCounter);
        }
    }
}