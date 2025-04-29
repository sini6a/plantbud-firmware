#include "moisture_sensor.h"
#include <Arduino.h>
#include "pins.h"

#define DRY_TIME 46    // microseconds for dry (you must measure)
#define WET_TIME 115   // microseconds for wet (you must measure)
#define MAX_DISCHARGE_TIME_US 1000000  // 50ms timeout (adjust based on experiment)

// Initialize global variables
adc1_channel_t sensor = ADC1_CHANNEL_1;
adc_atten_t atten_sensor = ADC_ATTEN_DB_11;

void setupSensorPWM() {
  pinMode(SENSOR_PWM, OUTPUT);
  digitalWrite(SENSOR_PWM, LOW);

  adc1_config_width(ADC_WIDTH_BIT_12);
  adc1_config_channel_atten(sensor, atten_sensor);
}

float readSensorData() {
  digitalWrite(SENSOR_PWM, HIGH);
  delay(1000); // Charge for 1ms

  // Step 2: Set pin to high impedance (disconnect)
  pinMode(SENSOR_PWM, LOW); // Important!

  unsigned long startTime = micros();
  unsigned long elapsedTime = 0;
  unsigned long sum = 0;

  while (true) {
    int adcValue = adc1_get_raw(sensor); // Read the discharge voltage
  
    if (adcValue <= 1700) {
      // Voltage dropped below threshold
      elapsedTime = micros() - startTime;
      break;
    }
  
    if ((micros() - startTime) > MAX_DISCHARGE_TIME_US) {
      // Timeout
      elapsedTime = MAX_DISCHARGE_TIME_US;
      break;
    }
  }

  // Optional: set back to OUTPUT LOW to reset state
  pinMode(SENSOR_PWM, OUTPUT);
  digitalWrite(SENSOR_PWM, LOW);

  delay(500); // Small pause between measurements

  return elapsedTime / 1000; // Time to discharge
}

float averageSensorReadings() {
  const int N = 10;
  unsigned long sum = 0;
  for (int i = 0; i < N; i++) {
    sum += readSensorData();
  }
  return sum / (float)N;
}

float getMoisturePercent(unsigned long elapsedTime) {
  float moisture = 100.0 * (elapsedTime - DRY_TIME) / (WET_TIME - DRY_TIME);
  if (moisture < 0) moisture = 0;
  if (moisture > 100) moisture = 100;
  return moisture;
}
