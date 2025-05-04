#include "epd_display.h"
#include <Arduino.h>
#include <SPI.h>
#include "pins.h"
#include <GxEPD2_BW.h>
#include <Fonts/FreeMonoBold9pt7b.h>
#include <Fonts/FreeSans9pt7b.h>
#include <Fonts/FreeSansBold12pt7b.h>

#define USE_HSPI_FOR_EPD
#define EPD_WIDTH  200
#define EPD_HEIGHT 200

int partialUpdateCount = 0;
char partialUpdateCountString[16];
bool forceRefresh = false;

// Create display instance
// Parameters: CS, DC, RST, BUSY
GxEPD2_BW<GxEPD2_154_D67, GxEPD2_154_D67::HEIGHT> display(GxEPD2_154_D67(PIN_CS, PIN_DC, PIN_RST, PIN_BUSY));

// Use custom SPI instance
SPIClass mySPI(FSPI);  // or HSPI if needed (depends on your board)

void setupEPD() {
  pinMode(10, OUTPUT);
  pinMode(0, OUTPUT);
  pinMode(20, OUTPUT);
  
  mySPI.begin(PIN_SCK, -1, PIN_MOSI, PIN_CS); // SCK, MISO (not used), MOSI, SS

  display.epd2.selectSPI(mySPI, SPISettings(4000000, MSBFIRST, SPI_MODE0));
  display.init(115200);
  display.setRotation(0); // Adjust rotation as needed (0-3)
}

void batteryLow(const String& voltage, const String& moisture) {
      // 2. Display battery warning
      display.setTextColor(GxEPD_BLACK);  // Add this!
      display.setFullWindow();
      display.firstPage();
      do {
        display.fillScreen(GxEPD_WHITE);
        display.setFont(&FreeSansBold12pt7b);
        
        // Center "LOW BATTERY" horizontally
        int16_t tbx, tby;
        uint16_t tbw, tbh;
        String warning = "LOW BATTERY";
        display.getTextBounds(warning, 0, 0, &tbx, &tby, &tbw, &tbh);
        int x = (EPD_WIDTH - tbw) / 2;
        int y = (EPD_HEIGHT / 2) - (tbh / 2);
        display.setCursor(x, y);
        display.setTextColor(GxEPD_BLACK);
        display.print(warning);
  
        // Moisture at bottom
        display.setFont(&FreeSans9pt7b);
        display.setCursor(10, EPD_HEIGHT - 10);
        display.print(moisture);
  
      } while (display.nextPage());  
}

void updateDisplay(const String& battery, const String& moisture, const String& time = "") {
  display.setTextColor(GxEPD_BLACK);  // Add this!
  display.setPartialWindow(0, 0, 200, 200);
  display.firstPage();
  do {
    display.fillScreen(GxEPD_WHITE);

    display.setFont(&FreeSansBold12pt7b);
    display.setCursor(10, 20);
    display.print("PlantBud!");

    display.drawLine(0, 25, 200, 25, GxEPD_BLACK);

    display.setFont(&FreeSans9pt7b);
    display.setCursor(10, 60);
    display.print("Battery: ");
    display.println(battery + " V");

    display.setCursor(10, 90);
    display.print("Moisture: ");
    display.println(moisture + " %");

    if (time != "") {
      display.setCursor(10, 120);
      display.print("Updated: ");
      display.println(time);
    }

    sprintf(partialUpdateCountString, "%01d", partialUpdateCount);  // Example: "03:42"
    display.setCursor(10, 150);
    display.print("Count: ");
    display.println(partialUpdateCountString);

  } while (display.nextPage());

  // Optional cleanup after many partial updates
  partialUpdateCount++;
  if (partialUpdateCount >= 50) {
    display.setFullWindow();
    display.firstPage();
    do {
      display.fillScreen(GxEPD_WHITE);
    } while (display.nextPage());
    partialUpdateCount = 0;
  }
}
