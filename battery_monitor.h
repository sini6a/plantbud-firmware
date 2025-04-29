#ifndef BATTERY_MONITOR_H
#define BATTERY_MONITOR_H

#include <driver/adc.h>
#include <pins.h>

// ADC Channel and attenuation setup
extern adc1_channel_t channel;
extern adc_atten_t atten;
extern float adcToBatteryFactor;

// Function declarations
void setupBatteryMonitor();
float readBatteryVoltage();

#endif // BATTERY_MONITOR_H
