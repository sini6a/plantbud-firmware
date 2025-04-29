#ifndef EPD_DISPLAY_H
#define EPD_DISPLAY_H

#include <Arduino.h>
#include "pins.h"

// Function declarations
void EPD_sendCommand(uint8_t command);
void EPD_sendData(uint8_t data);
void EPD_reset();
void EPD_waitUntilIdle();
void EPD_clearScreen();

#endif // EPD_DISPLAY_H
