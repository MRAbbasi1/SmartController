// -----------------------------------
// Display, Touch and UI Module
// -----------------------------------

#include "display.h"
#include "ui.h"

// ============================ Display Setup ============================

// Declare a buffer for drawing
static lv_disp_draw_buf_t draw_buf;
static lv_color_t *buf1 = NULL;
static lv_color_t *buf2 = NULL;

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); // Initialize TFT display with width and height

// ============================ Update Functions for Main Screen ============================

// Time interval for updating the arc value
const uint32_t updateArcMainScreenInterval = 5000; // in milliseconds
uint32_t lastUpdateMainScreenArc = 0;              // Stores the last update time

// main screen Arc graph
void updateArcMainScreen()
{
    // Check if the user is on the main screen
    if (lv_scr_act() == ui_mainScreen)
    {
        // Get the current time
        uint32_t currentTime = millis();

        // Check if the update interval has passed
        if (currentTime - lastUpdateMainScreenArc >= updateArcMainScreenInterval)
        {
            // Update the last update time
            lastUpdateMainScreenArc = currentTime;

            Serial.println("----------------UI-MainScreen----------------");

            // Read the temperature from the sensor (float values)
            float inletTempMainScreen = readTemperatureByName("Inlet");
            float outletTempMainScreen = readTemperatureByName("Outlet");

            // Variables to hold label text
            char inletLabelText[32];
            char outletLabelText[32];

            // Check if the inlet temperature is valid
            if (!isnan(inletTempMainScreen) &&
                inletTempMainScreen != DEVICE_DISCONNECTED_C &&
                inletTempMainScreen > -55 && inletTempMainScreen < 125)
            {
                // Update the arc value and label with valid temperature
                lv_arc_set_value(ui_inlet_Arc, static_cast<int>(inletTempMainScreen));
                snprintf(inletLabelText, sizeof(inletLabelText), "%.1f C°\n\nINLET", inletTempMainScreen);
                lv_obj_add_flag(ui_arc_danger_inlet, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                // Set error value for invalid temperature
                lv_arc_set_value(ui_inlet_Arc, 0); // Default value for invalid data
                snprintf(inletLabelText, sizeof(inletLabelText), "Error\n\nINLET");
                lv_obj_clear_flag(ui_arc_danger_inlet, LV_OBJ_FLAG_HIDDEN);
            }

            // Check if the outlet temperature is valid
            if (!isnan(outletTempMainScreen) &&
                outletTempMainScreen != DEVICE_DISCONNECTED_C &&
                outletTempMainScreen > -55 && outletTempMainScreen < 125)
            {
                // Update the arc value and label with valid temperature
                lv_arc_set_value(ui_outlet_Arc, static_cast<int>(outletTempMainScreen));
                snprintf(outletLabelText, sizeof(outletLabelText), "%.1f C°\n\nOUTLET", outletTempMainScreen);
                lv_obj_add_flag(ui_arc_danger_outlet_, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                // Set error value for invalid temperature
                lv_arc_set_value(ui_outlet_Arc, 0); // Default value for invalid data
                snprintf(outletLabelText, sizeof(outletLabelText), "Error\n\nOUTLET");
                lv_obj_clear_flag(ui_arc_danger_outlet_, LV_OBJ_FLAG_HIDDEN);
            }

            // Update the labels
            lv_label_set_text(ui_inletMainsScreen, inletLabelText);
            lv_label_set_text(ui_outletMainsScreen, outletLabelText);

            Serial.println("");

            // Print the updated values or error for debugging
            if (!isnan(inletTempMainScreen))
                Serial.printf("📺 [UI-MainScreen] Inlet Temp: %.1f°C\n", inletTempMainScreen);
            else
                Serial.println("📺 [UI-MainScreen] Inlet Temp: ❌ Error (Invalid data)");

            if (!isnan(outletTempMainScreen))
                Serial.printf("📺 [UI-MainScreen] Outlet Temp: %.1f°C\n", outletTempMainScreen);
            else
                Serial.println("📺 [UI-MainScreen] Outlet Temp: ❌ Error (Invalid data)");

            Serial.println("________________________________________________");
        }
    }
}

// Device status (true = ON, false = OFF)
bool isDeviceOn = false;                // Device working status (true = ON, false = OFF)
bool isHighTempAlarmActive = false;     // High temperature alarm status (true = active, false = inactive)
bool isHighPressureAlarmActive = false; // High pressure alarm status (true = active, false = inactive)
bool isDoorClosedStatus = false;        // Door status (true = closed, false = open)
bool isCompressorRelayActive = false;   // Door status (true = closed, false = open)

// Timing variables for checking door status
uint32_t lastDoorCheckTime = 0;            // Last time door status was checked
const uint32_t DOOR_CHECK_INTERVAL = 5000; // Interval for checking door status

// Timing variables for periodic checks in the main screen
uint32_t lastDeviceCheckTime = 0;            // Last check time for device working status
const uint32_t DEVICE_CHECK_INTERVAL = 5000; // Interval for checking device status

uint32_t lastHighTempCheckTime = 0;             // Last check time for high temperature alarm status
const uint32_t HIGH_TEMP_CHECK_INTERVAL = 5000; // Interval for checking high temperature alarm status

uint32_t lastPressureCheckTime = 0;            // Last check time for high pressure alarm status
const uint32_t PRESSURE_CHECK_INTERVAL = 5000; // Interval for checking high pressure alarm status

uint32_t lastRelayCheckTime = 0;            // Last time the relay status was checked
const uint32_t RELAY_CHECK_INTERVAL = 5000; // Interval to check relay status

// Function to update compressor relay icons
void updateCompressorIcons()
{
    if (isCompressorRelayActive)
    {
        // If the compressor relay is ON
        lv_obj_clear_flag(ui_Compressor_ON_Icon, LV_OBJ_FLAG_HIDDEN); // Show the "ON" icon
        lv_obj_add_flag(ui_Compressor_OFF_Icon, LV_OBJ_FLAG_HIDDEN);  // Hide the "OFF" icon
    }
    else
    {
        // If the compressor relay is OFF
        lv_obj_clear_flag(ui_Compressor_OFF_Icon, LV_OBJ_FLAG_HIDDEN); // Show the "OFF" icon
        lv_obj_add_flag(ui_Compressor_ON_Icon, LV_OBJ_FLAG_HIDDEN);    // Hide the "ON" icon
    }
}

// Function to update the device icons based on the device status
void updateDeviceIcons()
{
    if (isDeviceOn)
    {
        // If the device is ON
        lv_obj_clear_flag(ui_Cooler_ON_Icon, LV_OBJ_FLAG_HIDDEN); // Show the "ON" icon
        lv_obj_add_flag(ui_Cooler_OFF_Icon, LV_OBJ_FLAG_HIDDEN);  // Hide the "OFF" icon
    }
    else
    {
        // If the device is OFF
        lv_obj_clear_flag(ui_Cooler_OFF_Icon, LV_OBJ_FLAG_HIDDEN); // Show the "OFF" icon
        lv_obj_add_flag(ui_Cooler_ON_Icon, LV_OBJ_FLAG_HIDDEN);    // Hide the "ON" icon
    }
}

// Function to update the temperature alarm icons based on the high temperature alarm status
void updateTempAlarmIcons()
{
    if (isHighTempAlarmActive)
    {
        // If the high temperature alarm is active
        lv_obj_clear_flag(ui_High_Temp_Icon, LV_OBJ_FLAG_HIDDEN); // Show the high temperature icon
        lv_obj_add_flag(ui_Low_Temp_Icon, LV_OBJ_FLAG_HIDDEN);    // Hide the low temperature icon
    }
    else
    {
        // If the high temperature alarm is not active
        lv_obj_clear_flag(ui_Low_Temp_Icon, LV_OBJ_FLAG_HIDDEN); // Show the low temperature icon
        lv_obj_add_flag(ui_High_Temp_Icon, LV_OBJ_FLAG_HIDDEN);  // Hide the high temperature icon
    }
}

// Function to update the pressure alarm icons based on the high pressure alarm status
void updatePressureAlarmIcons()
{
    if (isHighPressureAlarmActive)
    {
        // If the high pressure alarm is active
        lv_obj_clear_flag(ui_High_Pressure_Icon, LV_OBJ_FLAG_HIDDEN); // Show the high pressure icon
        lv_obj_add_flag(ui_Low_Pressure_Icon, LV_OBJ_FLAG_HIDDEN);    // Hide the low pressure icon
    }
    else
    {
        // If the high pressure alarm is not active
        lv_obj_clear_flag(ui_Low_Pressure_Icon, LV_OBJ_FLAG_HIDDEN); // Show the low pressure icon
        lv_obj_add_flag(ui_High_Pressure_Icon, LV_OBJ_FLAG_HIDDEN);  // Hide the high pressure icon
    }
}

// Function to update door status icons
void updateDoorIcons()
{
    if (isDoorClosedStatus)
    {
        // If the door is closed
        lv_obj_clear_flag(ui_Door_Close_Icon, LV_OBJ_FLAG_HIDDEN); // Show the "Closed" icon
        lv_obj_add_flag(ui_Door_Open_Icon, LV_OBJ_FLAG_HIDDEN);    // Hide the "Open" icon
    }
    else
    {
        // If the door is open
        lv_obj_clear_flag(ui_Door_Open_Icon, LV_OBJ_FLAG_HIDDEN); // Show the "Open" icon
        lv_obj_add_flag(ui_Door_Close_Icon, LV_OBJ_FLAG_HIDDEN);  // Hide the "Closed" icon
    }
}

// Function to periodically check the device status
void checkDeviceStatus()
{
    uint32_t currentMillis = millis(); // Get the current system time

    // Check if the interval for the device power status has passed
    if (currentMillis - lastDeviceCheckTime >= DEVICE_CHECK_INTERVAL)
    {
        lastDeviceCheckTime = currentMillis; // Update the last check time

        Serial.println("----------------UI-MainScreen----------------");

        if (getEvaporatorRelayStatus())
        {
            isDeviceOn = true;
        }
        else
        {
            isDeviceOn = false;
        }

        // Update the icons based on the new device status
        updateDeviceIcons();

        Serial.println("________________________________________________");
    }
}

// Function to periodically check the high temperature alarm status
void checkHighTempAlarmStatus()
{
    uint32_t currentMillis = millis(); // Get the current system time

    // Check if the interval for the high temperature alarm status has passed
    if (currentMillis - lastHighTempCheckTime >= HIGH_TEMP_CHECK_INTERVAL)
    {
        lastHighTempCheckTime = currentMillis; // Update the last check time

        Serial.println("----------------UI-MainScreen----------------");

        // Check if the high temperature alarm status has changed
        if (isHighTempAlarm()) // This function needs to be defined to check the high temperature condition
        {
            // If the high temperature alarm is active
            isHighTempAlarmActive = true;
        }
        else
        {
            // If the high temperature alarm is not active
            isHighTempAlarmActive = false;
        }

        // Update the alarm icons based on the current status
        updateTempAlarmIcons();

        Serial.println("________________________________________________");
    }
}

// Function to periodically check the high pressure alarm status
void checkPressureAlarmStatus()
{
    uint32_t currentMillis = millis(); // Get the current system time

    // Check if the interval for the high pressure alarm status has passed
    if (currentMillis - lastPressureCheckTime >= PRESSURE_CHECK_INTERVAL)
    {
        lastPressureCheckTime = currentMillis; // Update the last check time

        Serial.println("----------------UI-MainScreen----------------");

        // Check if the high pressure alarm status has changed
        if (isPressureHigh()) // This function needs to be defined to check the high pressure condition
        {
            // If the high pressure alarm is active
            isHighPressureAlarmActive = true;
        }
        else
        {
            // If the high pressure alarm is not active
            isHighPressureAlarmActive = false;
        }

        // Update the pressure icons based on the current status
        updatePressureAlarmIcons();

        Serial.println("________________________________________________");
    }
}

// Function to check door status periodically
void checkDoorStatus()
{
    uint32_t currentMillis = millis(); // Get the current time

    // Check if the interval for checking door status has passed
    if (currentMillis - lastDoorCheckTime >= DOOR_CHECK_INTERVAL)
    {
        lastDoorCheckTime = currentMillis; // Update the last check time

        Serial.println("----------------UI-MainScreen----------------");

        // Check the door status
        if (isDoorClosed()) // This function should return true if the door is closed
        {
            // If the door is closed
            isDoorClosedStatus = true;
        }
        else
        {
            // If the door is open
            isDoorClosedStatus = false;
        }

        // Update the door icons based on the current status
        updateDoorIcons();

        Serial.println("________________________________________________");
    }
}

// Function to check Relay status periodically
void checkRelayStatus()
{
    uint32_t currentMillis = millis(); // Get the current system time

    // Check if the interval for relay status update has passed
    if (currentMillis - lastRelayCheckTime >= RELAY_CHECK_INTERVAL)
    {
        lastRelayCheckTime = currentMillis; // Update the last check time

        Serial.println("----------------UI-MainScreen----------------");

        if (getCompressorRelayStatus())
        {
            isCompressorRelayActive = true;
        }
        else
        {
            isCompressorRelayActive = false;
        }

        // Update the compressor icons based on relay status
        updateCompressorIcons();

        Serial.println("________________________________________________");
    }
}

// Function to check both device and temperature status periodically
void checkStatusIcon()
{
    if (lv_scr_act() == ui_mainScreen) //// Check if the active screen is ui_mainScreen
    {
        checkDeviceStatus();        // Check device working status
        checkHighTempAlarmStatus(); // Check high temperature alarm status
        checkPressureAlarmStatus(); // Check high pressure alarm status
        checkDoorStatus();          // Check door status
        checkRelayStatus();         // Check Relay status
    }
}

// ============================ Update Functions for Status-Info Screen =========================

// update Temp panel labels
static uint32_t lastUpdateTemperatureLabels = 0;
const uint32_t temperatureLabelsUpdateInterval = 10000; // Update every 10 seconds

void updateTemperatureLabels()
{
    if (lv_scr_act() == ui_statusScreen && !lv_obj_has_flag(ui_tempStatusPanel, LV_OBJ_FLAG_HIDDEN))
    {
        uint32_t currentTime = millis();

        if (currentTime - lastUpdateTemperatureLabels >= temperatureLabelsUpdateInterval)
        {
            // Non-blocking timing
            lastUpdateTemperatureLabels = currentTime;

            float inletTempstatusScreen = readTemperatureByName("Inlet");
            float outletTempstatusScreen = readTemperatureByName("Outlet");
            float antifreezeTempstatusScreen = readTemperatureByName("Antifreeze");
            float filterTempstatusScreen = readTemperatureByName("Filter");

            char buffer[32];

            // Inlet Temperature
            if (!isnan(inletTempstatusScreen) && inletTempstatusScreen != DEVICE_DISCONNECTED_C &&
                inletTempstatusScreen > -55 && inletTempstatusScreen < 125)
            {
                snprintf(buffer, sizeof(buffer), "INLET: %.1f °C", inletTempstatusScreen);
                lv_obj_add_flag(ui_inlet_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "INLET: NaN - Sensor Error!");
                lv_obj_clear_flag(ui_inlet_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_Inlet_Temp_status, buffer);

            // Outlet Temperature
            if (!isnan(outletTempstatusScreen) && outletTempstatusScreen != DEVICE_DISCONNECTED_C &&
                outletTempstatusScreen > -55 && outletTempstatusScreen < 125)
            {
                snprintf(buffer, sizeof(buffer), "OUTLET: %.1f °C", outletTempstatusScreen);
                lv_obj_add_flag(ui_outlet_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "OUTLET: NaN - Sensor Error!");
                lv_obj_clear_flag(ui_outlet_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_Outlet_Temp_status, buffer);

            // Antifreeze Temperature
            if (!isnan(antifreezeTempstatusScreen) && antifreezeTempstatusScreen != DEVICE_DISCONNECTED_C &&
                antifreezeTempstatusScreen > -55 && antifreezeTempstatusScreen < 125)
            {
                snprintf(buffer, sizeof(buffer), "ANTIFREEZE: %.1f °C", antifreezeTempstatusScreen);
                lv_obj_add_flag(ui_antifreeze_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "ANTIFREEZE: NaN - Sensor Error!");
                lv_obj_clear_flag(ui_antifreeze_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_antifreeze_temp_status, buffer);

            // Filter Temperature
            if (!isnan(filterTempstatusScreen) && filterTempstatusScreen != DEVICE_DISCONNECTED_C &&
                filterTempstatusScreen > -55 && filterTempstatusScreen < 125)
            {
                snprintf(buffer, sizeof(buffer), "FILTER: %.1f °C", filterTempstatusScreen);
                lv_obj_add_flag(ui_filter_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                snprintf(buffer, sizeof(buffer), "FILTER: NaN - Sensor Error!");
                lv_obj_clear_flag(ui_filter_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_fan_2_filter_alarm_status, buffer);

            // Debug Logs
            Serial.println("");
            Serial.println("📺 [UI-StatusScreen] Temperature Updates:");
            Serial.printf(" - Inlet Temp: %s\n", (!isnan(inletTempstatusScreen)) ? String(inletTempstatusScreen, 1).c_str() : "❌ Error (Invalid data)");
            Serial.printf(" - Outlet Temp: %s\n", (!isnan(outletTempstatusScreen)) ? String(outletTempstatusScreen, 1).c_str() : "❌ Error (Invalid data)");
            Serial.printf(" - Antifreeze Temp: %s\n", (!isnan(antifreezeTempstatusScreen)) ? String(antifreezeTempstatusScreen, 1).c_str() : "❌ Error (Invalid data)");
            Serial.printf(" - Filter Temp: %s\n", (!isnan(filterTempstatusScreen)) ? String(filterTempstatusScreen, 1).c_str() : "❌ Error (Invalid data)");
            Serial.println("");
        }
    }
}

// update Alarm panel labels
static uint32_t lastUpdateAlarmLabels = 0;
const uint32_t alarmLabelsUpdateInterval = 10000; // Update every 10 seconds

void updateAlarmLabels()
{
    if (lv_scr_act() == ui_statusScreen && !lv_obj_has_flag(ui_alarmStatusPanel, LV_OBJ_FLAG_HIDDEN))
    {
        uint32_t currentTime = millis();

        if (currentTime - lastUpdateAlarmLabels >= alarmLabelsUpdateInterval)
        {
            lastUpdateAlarmLabels = currentTime;

            char buffer_temp[64];
            char buffer_filter[64];
            char buffer_door[64];
            char buffer_pressure[64];

            // high temp alarm label status
            if (isHighTempAlarm())
            {
                float highTempAlarmInlet = readTemperatureByName("Inlet");

                if (!isnan(highTempAlarmInlet) && highTempAlarmInlet != DEVICE_DISCONNECTED_C &&
                    highTempAlarmInlet > -55 && highTempAlarmInlet < 125)
                {
                    snprintf(buffer_temp, sizeof(buffer_temp), "TEMP: %.1f °C - High Temp!", highTempAlarmInlet);
                    lv_obj_clear_flag(ui_high_temp_danger, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    snprintf(buffer_temp, sizeof(buffer_temp), "TEMP: NaN - Sensor Error!");
                    lv_obj_clear_flag(ui_high_temp_danger, LV_OBJ_FLAG_HIDDEN);
                }

                lv_obj_set_style_text_color(ui_high_Temp_Alarm, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red color for warning/error
            }
            else
            {
                snprintf(buffer_temp, sizeof(buffer_temp), "TEMP: Within Safe Limits - OK!");
                lv_obj_set_style_text_color(ui_high_Temp_Alarm, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green color for OK
                lv_obj_add_flag(ui_high_temp_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_high_Temp_Alarm, buffer_temp);

            // filter alarm label status
            if (isFilterWarning())
            {
                float highTempAlarmFilter = readTemperatureByName("Filter");

                if (!isnan(highTempAlarmFilter) && highTempAlarmFilter != DEVICE_DISCONNECTED_C &&
                    highTempAlarmFilter > -55 && highTempAlarmFilter < 125)
                {
                    snprintf(buffer_filter, sizeof(buffer_filter), "FILTER: %.1f °C - Filter Warning!", highTempAlarmFilter);
                    lv_obj_clear_flag(ui_filter_danger, LV_OBJ_FLAG_HIDDEN);
                }
                else
                {
                    snprintf(buffer_filter, sizeof(buffer_filter), "FILTER: NaN - Sensor Error!");
                    lv_obj_clear_flag(ui_filter_danger, LV_OBJ_FLAG_HIDDEN);
                }

                lv_obj_set_style_text_color(ui_Filter_Alarm, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red color for warning/error
            }
            else
            {
                snprintf(buffer_filter, sizeof(buffer_filter), "FILTER: Operating Normally - Stable!");
                lv_obj_set_style_text_color(ui_Filter_Alarm, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green color for OK
                lv_obj_add_flag(ui_filter_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_Filter_Alarm, buffer_filter);

            // door alarm label status
            if (!isDoorClosed())
            {
                snprintf(buffer_door, sizeof(buffer_door), "DOOR: Critical Warning - Open!");
                lv_obj_set_style_text_color(ui_Door_Alarm, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red for warning
                lv_obj_clear_flag(ui_door_danger, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                snprintf(buffer_door, sizeof(buffer_door), "DOOR: Closed - OK!");
                lv_obj_set_style_text_color(ui_Door_Alarm, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green for normal state
                lv_obj_add_flag(ui_door_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_Door_Alarm, buffer_door);

            // pressure alarm label status
            if (isPressureHigh())
            {
                snprintf(buffer_pressure, sizeof(buffer_pressure), "PRESSURE: High - Warning!");
                lv_obj_set_style_text_color(ui_high_Pressure_Alarm, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red for warning
                lv_obj_clear_flag(ui_high_pressure_danger, LV_OBJ_FLAG_HIDDEN);
            }
            else
            {
                snprintf(buffer_pressure, sizeof(buffer_pressure), "PRESSURE: Normal - Stable!");
                lv_obj_set_style_text_color(ui_high_Pressure_Alarm, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green for normal state
                lv_obj_add_flag(ui_high_pressure_danger, LV_OBJ_FLAG_HIDDEN);
            }
            lv_label_set_text(ui_high_Pressure_Alarm, buffer_pressure);

            // Debug Logs
            Serial.println("");
            Serial.println("📺 [UI-StatusScreen] Alarm Status Updates:");

            Serial.printf(" - Inlet Temp Alarm: %s\n", isHighTempAlarm() ? (!isnan(readTemperatureByName("Inlet")) ? String(readTemperatureByName("Inlet"), 1).c_str() : "❌ Error") : "✅ Within Safe Limits");

            Serial.printf(" - Filter Temp Alarm: %s\n", isFilterWarning() ? (!isnan(readTemperatureByName("Filter")) ? String(readTemperatureByName("Filter"), 1).c_str() : "❌ Error") : "✅ Operating Normally");

            Serial.println(!isDoorClosed() ? " - Door Alarm: 🚪 Open - Critical Warning !" : " - Door Alarm: ✅ Closed - good");

            Serial.println(isPressureHigh() ? " - Pressure Alarm: 🚨 High Pressure - System Alert !" : " - Pressure Alarm: ✅ Normal - Stable");

            Serial.println("");
        }
    }
}

// update Relay status panel labels
static uint32_t lastUpdateRelayLabels = 0;
const uint32_t relayLabelsUpdateInterval = 10000; // Update every 10 seconds

void updateRelayLabels()
{
    if (lv_scr_act() == ui_statusScreen && !lv_obj_has_flag(ui_relayStatusPanel, LV_OBJ_FLAG_HIDDEN))
    {
        uint32_t currentTime = millis();

        if (currentTime - lastUpdateRelayLabels >= relayLabelsUpdateInterval)
        {
            lastUpdateRelayLabels = currentTime;

            char buffer_evaprator[32];
            char buffer_compressor[32];
            char buffer_condensor[32];
            char buffer_fan2[32];

            // Evaporator status
            if (getEvaporatorRelayStatus())
            {
                snprintf(buffer_evaprator, sizeof(buffer_evaprator), "EVAPRATOR: ON - Working !");
                lv_obj_set_style_text_color(ui_evaprartor_Fan_Status, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green for normal state
            }
            else
            {
                snprintf(buffer_evaprator, sizeof(buffer_evaprator), "EVAPRATOR: OFF - Not Working !");
                lv_obj_set_style_text_color(ui_evaprartor_Fan_Status, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red for warning
            }
            lv_label_set_text(ui_evaprartor_Fan_Status, buffer_evaprator);

            // Compressor status
            if (getCompressorRelayStatus())
            {
                snprintf(buffer_compressor, sizeof(buffer_compressor), "COMPRESSOR: ON - Working !");
                lv_obj_set_style_text_color(ui_Compressor_Status, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green for normal state
            }
            else
            {
                snprintf(buffer_compressor, sizeof(buffer_compressor), "COMPRESSOR: OFF - Not Working !");
                lv_obj_set_style_text_color(ui_Compressor_Status, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red for warning
            }
            lv_label_set_text(ui_Compressor_Status, buffer_compressor);

            // Condenser status
            if (getCondenserRelayStatus())
            {
                snprintf(buffer_condensor, sizeof(buffer_condensor), "CONDENSER: ON - Working !");
                lv_obj_set_style_text_color(ui_condensor_Fan_1_Status, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green for normal state
            }
            else
            {
                snprintf(buffer_condensor, sizeof(buffer_condensor), "CONDENSER: OFF - Not Working !");
                lv_obj_set_style_text_color(ui_condensor_Fan_1_Status, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red for warning
            }
            lv_label_set_text(ui_condensor_Fan_1_Status, buffer_condensor);

            // Fan-2 status
            if (getFan2RelayStatus())
            {
                snprintf(buffer_fan2, sizeof(buffer_fan2), "FAN-2: ON - Working !");
                lv_obj_set_style_text_color(ui_condensor_Fan_2_Status, lv_color_hex(0x00BE10), LV_PART_MAIN); // 🟢 Green for normal state
            }
            else
            {
                snprintf(buffer_fan2, sizeof(buffer_fan2), "FAN-2: OFF - Not Working !");
                lv_obj_set_style_text_color(ui_condensor_Fan_2_Status, lv_color_hex(0xFF0000), LV_PART_MAIN); // 🔴 Red for warning
            }
            lv_label_set_text(ui_condensor_Fan_2_Status, buffer_fan2);

            // Debug Logs
            Serial.println("");
            Serial.println("📺 [UI-StatusScreen] Relay Status Updates:");

            Serial.printf(" - Evaporator: %s\n", getEvaporatorRelayStatus() ? "🟢 ON - Working !" : "🔴 OFF - Not Working !");
            Serial.printf(" - Compressor: %s\n", getCompressorRelayStatus() ? "🟢 ON - Working !" : "🔴 OFF - Not Working !");
            Serial.printf(" - Condenser: %s\n", getCondenserRelayStatus() ? "🟢 ON - Working !" : "🔴 OFF - Not Working !");
            Serial.printf(" - Fan-2: %s\n", getFan2RelayStatus() ? "🟢 ON - Working !" : "🔴 OFF - Not Working !");

            Serial.println("");
        }
    }
}

// update App status panel labels and Info
// static uint32_t lastUpdateAppLabels = 0;
// const uint32_t appLabelsUpdateInterval = 10000; // Update every 10 seconds

// void updateAppLabels()
// {
//     if (lv_scr_act() == ui_statusScreen && !lv_obj_has_flag(ui_networkStatusPanel, LV_OBJ_FLAG_HIDDEN))
//     {
//         uint32_t currentTime = millis();

//         if (currentTime - lastUpdateAppLabels >= appLabelsUpdateInterval)
//         {
//             lastUpdateAppLabels = currentTime;

//             char buffer_access_Point_wifi[32];
//             char buffer_access_Point_Password[32];
//         }
//     }
// }

// update Maintenance status panel labels
static uint32_t lastMaintenanceUpdateTime = 0;
const uint32_t UPDATE_INTERVAL_Maintenance = 3600000;
static bool firstMaintenanceRun = true;

void updateMaintenanceLabels()
{
    uint32_t currentTime = millis();

    if (firstMaintenanceRun || (currentTime - lastMaintenanceUpdateTime >= UPDATE_INTERVAL_Maintenance))
    {
        firstMaintenanceRun = false;
        lastMaintenanceUpdateTime = currentTime;

        int serviceInterval = getNumericSetting(SERVICE_INTERVAL);
        int hoursElapsed = getNumericSetting(HOURS_ELAPSED);

        serviceInterval = (serviceInterval <= 0) ? 365 : serviceInterval;
        hoursElapsed = (hoursElapsed < 0 || hoursElapsed > 87600) ? 0 : hoursElapsed;

        int daysElapsed = hoursElapsed / 24;
        int remainingHours = hoursElapsed % 24;
        int daysRemaining = max(0, serviceInterval - daysElapsed);

        char buffer[128];
        bool isServiceOverdue = daysRemaining <= 0;

        if (!isServiceOverdue)
        {
            snprintf(buffer, sizeof(buffer),
                     "SERVICES INTERVALS:\n %d Days %d Hours Elapsed - OK!",
                     daysElapsed, remainingHours);
        }
        else
        {
            snprintf(buffer, sizeof(buffer),
                     "MAINTENANCE ALERT: %d Hours Overdue!\n                 Service Required!",
                     hoursElapsed - (serviceInterval * 24));
        }

        if (ui_servicesintervals != NULL)
        {
            lv_label_set_text(ui_servicesintervals, buffer);
            lv_obj_set_style_text_color(ui_servicesintervals,
                                        isServiceOverdue ? lv_color_hex(0xFF0000) : lv_color_hex(0xFFFFFF),
                                        LV_PART_MAIN);
        }

        Serial.println("");
        Serial.println("📺 [UI-StatusScreen] Maintenance Status Updated");
        Serial.printf(" - Service Interval: %d Days\n", serviceInterval);
        Serial.printf(" - Hours Elapsed: %d Hours\n", hoursElapsed);
        Serial.printf(" - Days Elapsed: %d Days\n", daysElapsed);
        Serial.printf(" - Remaining Time: %d Days %d Hours\n", daysRemaining, remainingHours);

        if (isServiceOverdue)
        {
            Serial.println(" - Status: 🔴 ALERT - Maintenance Overdue! Immediate Service Required!");
        }
        else
        {
            Serial.printf(" - Status: 🟢 OK - %d Days Left Until Service.\n", daysRemaining);
        }

        Serial.println("");
    }
}

// update Info status panel labels
static uint32_t lastInfoUpdateTime = 0;
const uint32_t UPDATE_INTERVAL_Info = 12 * 60 * 60 * 1000; // 24 hours in milliseconds
static bool firstInfoRun = true;

void updateInfoLabels()
{
    uint32_t currentTime = millis();

    // First run or interval check
    if (firstInfoRun || (currentTime - lastInfoUpdateTime >= UPDATE_INTERVAL_Info))
    {
        firstInfoRun = false;
        lastInfoUpdateTime = currentTime;

        Serial.println("");
        Serial.println("📺 [UI-StatusScreen] Info Labels Updating");

        // Update labels
        int serialNumber = getNumericSetting(DEVICE_SERIAL_ID);
        int customerId = getNumericSetting(CUSTOMER_ID);
        int activationDate = getNumericSetting(ACTIVATION_DATE);

        char buffer[32];

        // Serial Number
        snprintf(buffer, sizeof(buffer), "Serial Number: %d", serialNumber);
        lv_label_set_text(ui_serial_number, buffer);

        // Customer ID
        snprintf(buffer, sizeof(buffer), "Customer ID: %d", customerId);
        lv_label_set_text(ui_customerID, buffer);

        // Activation Date
        snprintf(buffer, sizeof(buffer), "Activation Date: %04d-%02d-%02d",
                 activationDate / 10000,         // Year
                 (activationDate % 10000) / 100, // Month
                 activationDate % 100            // Day
        );
        lv_label_set_text(ui_activationDate, buffer);

        // Debug Log Footer
        Serial.println("________________________ UI-StatusScreen ________________________");
        Serial.println("");
    }
}

// Call in displayLoop()
void checkStatusUpdate()
{
    updateRelayLabels();
    updateTemperatureLabels();
    updateAlarmLabels();
    // updateAppLabels();
    updateMaintenanceLabels();
    updateInfoLabels();
}

// ============================ Screen Switching =========================

// Variables for touch timeout management
uint32_t lastTouchTime = 0;              // Last time the screen was touched
const uint32_t timeoutDuration = 120000; // Timeout duration in milliseconds

// Function to switch to the main screen and delete the previous screen
void switchToMainScreen()
{
    // Save the current screen before switching
    lv_obj_t *currentScreen = lv_scr_act();

    // Only delete the previous screen if it's not the main screen
    if (currentScreen != ui_mainScreen && currentScreen != NULL)
    {
        lv_obj_t *screenToDelete = currentScreen; // Create a temporary pointer for compatibility
        _ui_screen_delete(&screenToDelete);       // Pass as a pointer-to-pointer to delete the screen
    }

    // Change to the main screen (no animation)
    _ui_screen_change(&ui_mainScreen, LV_SCR_LOAD_ANIM_NONE, 0, 0, NULL);

    // floating objects reset after switch
    lv_obj_add_flag(ui_tempStatusPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_tempStatusButton, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_alarmStatusPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_alarmStatusButton, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_relayStatusPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_relayStatusButton, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_networkStatusPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_networkStatusButton, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_deviceInfoPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_serviceInfoButton, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_serviceInfoPanel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_deviceInfoButton, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_sensors_setup_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_factory_reset_container, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_QRcode_Tab, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Hide_QR_Code_Button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_show_QR_Code_Button, LV_OBJ_FLAG_HIDDEN);

    lv_obj_clear_flag(ui_returnButton14, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Advanced_Setting_Label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Setting_Save_btn_in_advanced_Screen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Setting_Save_btn_label_advanced_Screen, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_TabView1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Sensor_Setup_Button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Sensor_Setup_Button_Label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_open_factory_reset_panel, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_factory_reset_btn_Label, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_DEVICE_RESTART_Button, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_DEVICE_RESTART_Button_Label, LV_OBJ_FLAG_HIDDEN);

    lv_obj_add_flag(ui_sensors_setup_container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_clear_flag(ui_Sensor_Setup_Notice_Container, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Sensor_Setup_main_Container_1, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Sensor_Setup_main_Container_2, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Sensor_Setup_main_Container_3, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Sensor_Setup_main_Container_4, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Sensor_Setup_main_Container_5, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_Sensor_Setup_main_Container_6, LV_OBJ_FLAG_HIDDEN);
    lv_obj_add_flag(ui_factory_reset_container, LV_OBJ_FLAG_HIDDEN);
}

// Function to check if any important container is visible
bool isAnyContainerVisible()
{
    return !lv_obj_has_flag(ui_sensors_setup_container, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_Sensor_Setup_main_Container_1, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_Sensor_Setup_main_Container_2, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_Sensor_Setup_main_Container_3, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_Sensor_Setup_main_Container_4, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_Sensor_Setup_main_Container_5, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_Sensor_Setup_main_Container_6, LV_OBJ_FLAG_HIDDEN) ||
           !lv_obj_has_flag(ui_factory_reset_container, LV_OBJ_FLAG_HIDDEN);
}
// ============================ Display Flushing ==========================

// Display flushing function (to send data to the display)
void my_disp_flush(lv_disp_drv_t *disp, const lv_area_t *area, lv_color_t *color_p)
{
    uint32_t w = (area->x2 - area->x1 + 1);
    uint32_t h = (area->y2 - area->y1 + 1);

    // Start writing to the display
    tft.startWrite();
    tft.setAddrWindow(area->x1, area->y1, w, h);             // Set the address window for the specified area
    tft.pushColors((uint16_t *)&color_p->full, w * h, true); // Push the pixel data to the display
    tft.endWrite();                                          // End the write operation

    // Signal that flushing is done
    lv_disp_flush_ready(disp);
}

// ============================ Touch Input Management =====================

// Read the touchpad input and update the touch data
void my_touchpad_read(lv_indev_drv_t *indev_driver, lv_indev_data_t *data)
{
    uint16_t touchX, touchY;

    // Check if the screen was touched (return touch coordinates)
    bool touched = tft.getTouch(&touchX, &touchY, 400);

    if (!touched)
    {
        // If no touch is detected, set the state to released
        data->state = LV_INDEV_STATE_REL;
    }
    else
    {
        // If touched, set the state to pressed and reset the timeout timer
        data->state = LV_INDEV_STATE_PR;
        lastTouchTime = millis(); // Reset the touch timeout timer

#if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
        // Adjust touch coordinates for screen rotation (if needed)
        data->point.x = touchY;
        data->point.y = touchX;
#else
        data->point.x = touchX;
        data->point.y = touchY;
#endif
    }
}

// ============================ Display Setup ============================

// Setup function to initialize the display and touch
void displaySetup()
{
    Serial.println("📺 [Display] Initializing Screen...");

    buf1 = (lv_color_t *)heap_caps_malloc(SIZE_SCREEN_BUFFER * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    if (!buf1)
    {
        Serial.println("❌ [Error] Failed to allocate buffer1 in PSRAM!");
        return;
    }

    buf2 = (lv_color_t *)heap_caps_malloc(SIZE_SCREEN_BUFFER * sizeof(lv_color_t), MALLOC_CAP_SPIRAM);
    if (!buf2)
    {
        Serial.println("❌ [Error] Failed to allocate buffer2 in PSRAM!");
        heap_caps_free(buf1); // Free previously allocated memory
        return;
    }

    // Initialize display buffer for LVGL (drawing buffer)
    lv_disp_draw_buf_init(&draw_buf, buf1, buf2, SIZE_SCREEN_BUFFER);

    // Initialize LVGL library
    lv_init();

    // Initialize TFT display
    tft.begin();
    tft.setRotation(SCREEN_ROTATION); // Set the rotation of the display based on the defined constant

    // Calibrate the touch screen based on the screen rotation
#if SCREEN_ROTATION == 0
    uint16_t calData[5] = {230, 3580, 330, 3600, 4}; // Calibration data for rotation 0
#elif SCREEN_ROTATION == 1
    uint16_t calData[5] = {215, 3438, 281, 3430, 6}; // Calibration data for rotation 1
#elif SCREEN_ROTATION == 2
    uint16_t calData[5] = {230, 3580, 330, 3600, 2}; // Calibration data for rotation 2
#elif SCREEN_ROTATION == 3
    uint16_t calData[5] = {230, 3580, 330, 3600, 0}; // Calibration data for rotation 3
#endif

    // Set the touch screen calibration data
    tft.setTouch(calData);

    // Set up display driver for LVGL
    static lv_disp_drv_t disp_drv;
    lv_disp_drv_init(&disp_drv);       // Initialize the display driver
    disp_drv.hor_res = screenWidth;    // Set horizontal resolution
    disp_drv.ver_res = screenHeight;   // Set vertical resolution
    disp_drv.flush_cb = my_disp_flush; // Set the flushing callback function
    disp_drv.draw_buf = &draw_buf;     // Set the drawing buffer
    lv_disp_drv_register(&disp_drv);   // Register the display driver with LVGL

    // Set up touch input driver for LVGL
    static lv_indev_drv_t indev_drv;
    lv_indev_drv_init(&indev_drv);          // Initialize the input device driver
    indev_drv.type = LV_INDEV_TYPE_POINTER; // Set the input type to pointer (touch screen)
    indev_drv.read_cb = my_touchpad_read;   // Set the touch read callback function
    lv_indev_drv_register(&indev_drv);      // Register the input device driver with LVGL

    // Initialize user interface screens
    ui_init();

    // Attach event handlers for value updates
    // lv_obj_add_event_cb(ui_inlet_Arc, ui_event_inlet_Arc, LV_EVENT_VALUE_CHANGED, NULL);
    // lv_obj_add_event_cb(ui_outlet_Arc, ui_event_outlet_Arc, LV_EVENT_VALUE_CHANGED, NULL);

    // delay(500);

    lv_scr_load(ui_Boot_Screen);

    // Handle LVGL tasks
    // lv_timer_handler();

    Serial.println("📺 [Display] Screen, Touch and UI Setup completed ✅");
}

// ============================ Main Loop ==============================

// Main loop function for updating the display and checking touch timeout
void displayLoop()
{
    // Handle LVGL tasks
    lv_timer_handler();

    // Check for touch timeout and switch screens if no important container is visible
    if (millis() - lastTouchTime > timeoutDuration)
    {
        if (lv_scr_act() != ui_mainScreen && !isAnyContainerVisible())
        {
            switchToMainScreen();
            switchToMainScreen();
        }
    }

    // Update the inlet, outlet arc value periodically
    updateArcMainScreen();

    // Cooler status icons check function
    checkStatusIcon();

    // Status Labels in Status Screen
    checkStatusUpdate();

    // Small delay to allow LVGL to process tasks and avoid blocking
    delay(5);
}
