#ifndef EPD_DISPLAY_H
#define EPD_DISPLAY_H

#include <Arduino.h>
#include "pins.h"

// Function declarations
void setupEPD();
void batteryLow(const String& voltage, const String& moisture);
void updateDisplay(const String& battery, const String& moisture, const String& time);

#endif // EPD_DISPLAY_H
