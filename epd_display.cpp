#include "epd_display.h"
#include <Arduino.h>
#include <SPI.h>
#include "pins.h"

#define EPD_WIDTH  248
#define EPD_HEIGHT 128

void EPD_sendCommand(uint8_t command) {
  digitalWrite(PIN_DC, LOW);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(command);
  digitalWrite(PIN_CS, HIGH);
}

void EPD_sendData(uint8_t data) {
  digitalWrite(PIN_DC, HIGH);
  digitalWrite(PIN_CS, LOW);
  SPI.transfer(data);
  digitalWrite(PIN_CS, HIGH);
}

void EPD_reset() {
  digitalWrite(PIN_RST, LOW);
  delay(200);
  digitalWrite(PIN_RST, HIGH);
  delay(200);
}

void EPD_waitUntilIdle() {
  while (digitalRead(PIN_BUSY) == LOW) { // 0 = busy, 1 = idle
    delay(100);
  }
}

void EPD_clearScreen() {
  int width_in_bytes = (EPD_WIDTH + 7) / 8; // 1 byte = 8 pixels
  int height = EPD_HEIGHT;

  EPD_sendCommand(0x10); // DATA_START_TRANSMISSION_1
  for (int i = 0; i < width_in_bytes * height; i++) {
    EPD_sendData(0x00); // Black pixel
  }

  EPD_sendCommand(0x13); // DATA_START_TRANSMISSION_2
  for (int i = 0; i < width_in_bytes * height; i++) {
    EPD_sendData(0xFF); // Black pixel
  }

  EPD_sendCommand(0x12); // DISPLAY_REFRESH
  EPD_waitUntilIdle();
}
