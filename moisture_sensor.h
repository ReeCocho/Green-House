#ifndef GH_MOISTURE_SENSOR_H
#define GH_MOISTURE_SENSOR_H

// Current value from the moisture sensor
extern int moisture_sensor_value;

// Initialize the moisture sensor
extern void init_moisture_sensor();

// Update sensor median
extern void update_sensor_median(const unsigned long dt);

// Update the moisture sensor led
extern void update_moisture_led(const unsigned long dt);

#endif
