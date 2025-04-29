#ifndef MOISTURE_SENSOR_H
#define MOISTURE_SENSOR_H

#include <driver/adc.h>
#include <pins.h>

extern adc1_channel_t sensor;
extern adc_atten_t atten_sensor;

// Function declarations
void setupSensorPWM();
float readSensorData();
float averageSensorReadings();
float getMoisturePercent(unsigned long elapsedTime);

#endif // MOISTURE_SENSOR_H
