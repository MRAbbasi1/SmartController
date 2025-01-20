// -----------------------------------
// Display and Touch Management Module
// -----------------------------------

#include "display.h"
#include "ui.h"

// ============================ Display Setup ============================

// Declare a buffer for drawing
static lv_disp_draw_buf_t draw_buf;
static lv_color_t buf[SIZE_SCREEN_BUFFER]; // Buffer for screen content

TFT_eSPI tft = TFT_eSPI(screenWidth, screenHeight); // Initialize TFT display with width and height

// ============================ Touch Timeout Management =========================

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

    // Small delay to allow LVGL to process tasks and avoid blocking
    delay(5);
}
