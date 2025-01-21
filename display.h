#ifndef DISPLAY_H
#define DISPLAY_H

#include <Arduino.h>
#include <lvgl.h>
#include <TFT_eSPI.h>
#include "tempSensor.h"
#include "setting.h"
#include "alarm.h"
#include "relayControl.h"

// Define screen rotation
#define SCREEN_ROTATION 1

#if (SCREEN_ROTATION == 1) || (SCREEN_ROTATION == 3)
constexpr uint16_t screenWidth = 320;
constexpr uint16_t screenHeight = 240;
#else
constexpr uint16_t screenWidth = 240;
constexpr uint16_t screenHeight = 320;
#endif

#define SIZE_SCREEN_BUFFER (screenWidth * screenHeight / 8)

void displaySetup();
void displayLoop();

#endif // DISPLAY_H
