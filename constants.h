#ifndef GH_CONSTANTS_H
#define GH_CONSTANTS_H

/** GLOBAL CONSTANTS **/

// Max, min, and trigger values sent by the sensor
#define DRY_VALUE 0
#define MOIST_VALUE 560
#define PUMP_ON_VALUE 100
#define PUMP_OFF_VALUE 200

// Moisture sensor pin
#define M_SENSOR_PIN A0

// Pump pin
#define PUMP_PIN 13

// Moisture reading LED pin
#define M_SENSOR_LED_PIN 11

// EarthBox float switch pin
#define EB_FLOAT_SWITCH_PIN 12

#endif
