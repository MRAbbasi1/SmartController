// general.cpp
#include "general.h"
#include "setting.h"
#include "relayControl.h"

// ======== System Active Time Counter ========

// Constants
const uint32_t HOUR_INTERVAL = 60 * 60 * 1000; // 1 hour in milliseconds
const uint8_t SAVE_INTERVAL = 1;               // Save every 1 hours
const uint32_t MAX_HOURS = 87600;              // Maximum trackable hours

static uint32_t hoursElapsedCounter = 0;
static uint32_t lastHourUpdateTime = 0;

void initializeHoursElapsedTracking()
{
    uint32_t currentTime = millis();
    Serial.printf("[Time][INFO][%lu] Initializing hours elapsed counter\n", currentTime);
    hoursElapsedCounter = getNumericSetting(HOURS_ELAPSED);
    lastHourUpdateTime = currentTime;
    Serial.printf("[Time][INFO][%lu] Hours elapsed counter initialized: %d hours\n", currentTime, hoursElapsedCounter);
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
                Serial.printf("[Time][INFO][%lu] Hours elapsed counter saved: %d hours\n", currentTime, hoursElapsedCounter);
            }

            Serial.printf("[Time][INFO][%lu] Hours elapsed updated: %d hours\n", currentTime, hoursElapsedCounter);
        }
    }
}