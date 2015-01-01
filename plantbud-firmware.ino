#include <Arduino.h>
#include <NimBLEDevice.h>
#include "pins.h"
#include "battery_monitor.h"
#include "moisture_sensor.h"
#include "epd_display.h"
#include "bluetooth.h"

#define BUTTON_PIN 9  // GPIO9 (BOOT button pin)

void setup() {
    // Start the serial monitor
    Serial.begin(115200);

    pinMode(BUTTON_PIN, INPUT);

    setupBluetooth();
    setupSensorPWM();
    setupBatteryMonitor();
    setupEPD();

}

void loop() {
  // Read sensor data
  float voltage = readBatteryVoltage();
  float moisture = readSensorData();
  int moisturePercentage = getMoisturePercent(moisture);

  // Format each field separately
  char batteryStr[16];
  char moistureStr[16];
  char timeStr[16];

  sprintf(batteryStr, "%.2f", voltage);          // Example: "4.19"
  sprintf(moistureStr, "%d", moisturePercentage); // Example: "56"

  // Get time (as HH:MM or use millis()/1000 for uptime)
  unsigned long seconds = millis() / 1000;
  int minutes = (seconds / 60) % 60;
  int hours = (seconds / 3600) % 24;
  sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds % 60);  // Example: "03:42"

  // Update E-Ink display
  updateDisplay(batteryStr, moistureStr, timeStr);

  if (isConnected()) {
    // Format and print over serial/Bluetooth
    char txString[64];
    sprintf(txString, "B:%.2fV M:%.1f(%d%%)", voltage, moisture, moisturePercentage);
    Serial.println(txString);
    notifyBluetooth(txString);
  }

  // Reboot if button is pressed
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed! Rebooting...");
    esp_restart();
  }

  delay(1000);  // Adjust as needed
}
