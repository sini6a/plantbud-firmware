#include <Arduino.h>
#include <NimBLEDevice.h>
#include "pins.h"
#include "battery_monitor.h"
#include "moisture_sensor.h"
#include "epd_display.h"
#include "bluetooth.h"

#define BUTTON_PIN 9  // GPIO9 (BOOT button pin)
float voltage, moisture;
int moisturePercentage;
bool showWarning = true;

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
  // Read sensor data first
  voltage = readBatteryVoltage();
  moisture = readSensorData();
  moisturePercentage = getMoisturePercent(moisture);

  // Format each field separately
  char batteryStr[16];
  char moistureStr[16];
  char timeStr[16];

  sprintf(batteryStr, "%.2f", voltage);
  sprintf(moistureStr, "%d", moisturePercentage);

  // Get time (HH:MM:SS)
  unsigned long seconds = millis() / 1000;
  int minutes = (seconds / 60) % 60;
  int hours = (seconds / 3600) % 24;
  sprintf(timeStr, "%02d:%02d:%02d", hours, minutes, seconds % 60);

  // Battery low: Show warning once, then skip display updates
  if (voltage <= 3.30) {
    if (showWarning) {
      batteryLow(batteryStr, moistureStr);  // Show warning once
      showWarning = false;
    }
  } else {
    // Battery OK: Resume normal display
    updateDisplay(batteryStr, moistureStr, timeStr);
    showWarning = true;  // Allow warning again if battery drops later
  }

  // Send data via Bluetooth if connected
  if (isConnected()) {
    char txString[64];
    sprintf(txString, "B:%.2fV M:%.1f(%d%%)", voltage, moisture, moisturePercentage);
    Serial.println(txString);
    notifyBluetooth(txString);
  }

  // Check for button press to reboot
  if (digitalRead(BUTTON_PIN) == LOW) {
    Serial.println("Button pressed! Rebooting...");
    esp_restart();
  }

  delay(1000); // Adjust interval as needed
}
