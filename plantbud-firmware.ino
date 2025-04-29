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

}

void loop() {  
    // Send the data over Bluetooth
    if(isConnected()) {
        // Variables to hold battery and moisture data
        char txString[64];
      
        // Get battery voltage and moisture data
        float voltage = readBatteryVoltage();
        float moisture = readSensorData();
        int moisturePercentage = getMoisturePercent(moisture);
      
        // Format data to send via Bluetooth
        sprintf(txString, "B:%.2fV M:%.1f(%d%%)", voltage, moisture, moisturePercentage);
        Serial.println(txString);
    
        notifyBluetooth(txString);
    }
  
    // Reboot if button is pressed
    if (digitalRead(BUTTON_PIN) == LOW) {
        Serial.println("Button pressed! Rebooting...");
        esp_restart();  // Reboot the board
    }
}
