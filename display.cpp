// -----------------------------------
// Display, Touch and UI Module
// -----------------------------------

#include "display.h"
#include "ui.h"

// ============================ Display Setup ============================

// Declare a buffer for drawing
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SIZE_SCREEN_BUFFER]; // Buffer for screen content

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); // Initialize TFT display with width and height

// ============================ Update Functions for MainScreen ============================

// Time interval for updating the arc value
const uint32_t updateArcMainScreenInterval = 5000; // in milliseconds
uint32_t lastUpdateTime = 0;                       // Stores the last update time

// main screen Arc graph
void updateArcMainScreen()
{
    // Check if the user is on the main screen
    if (lv_scr_act() == ui_mainScreen)
    {
        // Get the current time
        uint32_t currentTime = millis();

        // Check if the update interval has passed
        if (currentTime - lastUpdateTime >= updateArcMainScreenInterval)
        {
            // Update the last update time
            lastUpdateTime = currentTime;

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
            }
            else
            {
                // Set error value for invalid temperature
                lv_arc_set_value(ui_inlet_Arc, 0); // Default value for invalid data
                snprintf(inletLabelText, sizeof(inletLabelText), "Error\n\nINLET");
            }

            // Check if the outlet temperature is valid
            if (!isnan(outletTempMainScreen) &&
                outletTempMainScreen != DEVICE_DISCONNECTED_C &&
                outletTempMainScreen > -55 && outletTempMainScreen < 125)
            {
                // Update the arc value and label with valid temperature
                lv_arc_set_value(ui_outlet_Arc, static_cast<int>(outletTempMainScreen));
                snprintf(outletLabelText, sizeof(outletLabelText), "%.1f C°\n\nOUTLET", outletTempMainScreen);
            }
            else
            {
                // Set error value for invalid temperature
                lv_arc_set_value(ui_outlet_Arc, 0); // Default value for invalid data
                snprintf(outletLabelText, sizeof(outletLabelText), "Error\n\nOUTLET");
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
unsigned long lastDoorCheckTime = 0;            // Last time door status was checked
const unsigned long DOOR_CHECK_INTERVAL = 5000; // Interval for checking door status

// Timing variables for periodic checks in the main screen
unsigned long lastDeviceCheckTime = 0;            // Last check time for device working status
const unsigned long DEVICE_CHECK_INTERVAL = 5000; // Interval for checking device status

unsigned long lastHighTempCheckTime = 0;             // Last check time for high temperature alarm status
const unsigned long HIGH_TEMP_CHECK_INTERVAL = 5000; // Interval for checking high temperature alarm status

unsigned long lastPressureCheckTime = 0;            // Last check time for high pressure alarm status
const unsigned long PRESSURE_CHECK_INTERVAL = 5000; // Interval for checking high pressure alarm status

unsigned long lastRelayCheckTime = 0;            // Last time the relay status was checked
const unsigned long RELAY_CHECK_INTERVAL = 5000; // Interval to check relay status

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
    unsigned long currentMillis = millis(); // Get the current system time

    // Check if the interval for the device power status has passed
    if (currentMillis - lastDeviceCheckTime >= DEVICE_CHECK_INTERVAL)
    {
        lastDeviceCheckTime = currentMillis; // Update the last check time

        Serial.println("----------------UI-MainScreen----------------");

        // Check if the device power status has changed
        if (getChangedFlagTemp("boolean", DEVICE_ON))
        {
            // Update the device status
            isDeviceOn = getBooleanSetting(DEVICE_ON);
            resetChangedFlagTemp("boolean", DEVICE_ON);

            // Update the icons based on the new device status
            updateDeviceIcons();
        }

        Serial.println("________________________________________________");
    }
}

// Function to periodically check the high temperature alarm status
void checkHighTempAlarmStatus()
{
    unsigned long currentMillis = millis(); // Get the current system time

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
    unsigned long currentMillis = millis(); // Get the current system time

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
    unsigned long currentMillis = millis(); // Get the current time

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
    unsigned long currentMillis = millis(); // Get the current system time

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
            //
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

// ============================ Screen Switching =========================

// Variables for touch timeout management
uint32_t lastTouchTime = 0;              // Last time the screen was touched
const uint32_t timeoutDuration = 120000; // Timeout duration in milliseconds

// ============================ Screen Switching ==========================

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
    Serial.println("🔁 Initializing Display...");

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

    // Initialize display buffer for LVGL (drawing buffer)
    lv_disp_draw_buf_init(&draw_buf, buf, NULL, SIZE_SCREEN_BUFFER);

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
    lv_obj_add_event_cb(ui_inlet_Arc, ui_event_inlet_Arc, LV_EVENT_VALUE_CHANGED, NULL);
    lv_obj_add_event_cb(ui_outlet_Arc, ui_event_outlet_Arc, LV_EVENT_VALUE_CHANGED, NULL);

    Serial.println("📺 [Display] Display, Touch and UI Setup completed ✅");
}

// ============================ Main Loop ==============================

// Main loop function for updating the display and checking touch timeout
void displayLoop()
{
    // Handle LVGL tasks such as redrawing and handling animations
    lv_timer_handler();

    // Check for touch timeout and switch screens if necessary
    if (millis() - lastTouchTime > timeoutDuration)
    {
        if (lv_scr_act() != ui_mainScreen) // Only if not already on the main screen
        {
            switchToMainScreen();
        }
    }

    // Update the inlet, outlet arc value periodically
    updateArcMainScreen();

    // Cooler status icons check function
    checkStatusIcon();

    // Small delay to allow LVGL to process tasks and avoid blocking
    delay(5);
}
