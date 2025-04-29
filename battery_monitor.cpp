#include "battery_monitor.h"
#include <Arduino.h>
#include <pins.h>

// Define constants for resistors
const float R1 = 33000.0;  // ohms
const float R2 = 100000.0; // ohms

// Initialize global variables
adc1_channel_t channel = ADC1_CHANNEL_3;
adc_atten_t atten = ADC_ATTEN_DB_2_5;
float adcToBatteryFactor = 0;

void setupBatteryMonitor() {
  pinMode(MOSFET_ENABLE_PIN, OUTPUT);
  digitalWrite(MOSFET_ENABLE_PIN, LOW);

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(channel, atten);

  // Calculate the factor based on your divider
  adcToBatteryFactor = (4.3 / 4095.0) * ((R1 + R2) / R2);
  Serial.print("adcToBatteryFactor = ");
  Serial.println(adcToBatteryFactor, 8);
}

float readBatteryVoltage() {
  digitalWrite(MOSFET_ENABLE_PIN, HIGH);
  delay(800); // Wait for stabilization

  int total = 0;
  for (int i = 0; i < 10; i++) {
    total += adc1_get_raw(channel);
    delay(20);
  }
  int adcValue = total / 10;

  digitalWrite(MOSFET_ENABLE_PIN, LOW);

  float voltage = (adcValue / 4095.0) * 1.1;
  float batteryVoltage = voltage * 4.6;

  return batteryVoltage;
}
