#ifndef GH_MOISTURE_RECORDING_H
#define GH_MOISTURE_RECORDING_H

/* Includes. */
#include <stdint.h>
#include "moisture_sensor.h"

/** Time (In milliseconds) in between moisture sensor recordings. */
#define MOISTURE_RECORDING_TIME 3600000

/** Maximum number of recordings stored. (Maximum value is 256, minimum is 1) */
#define MAX_MOISTURE_RECORDINGS 48

/**
 * Utility to record moisture sensor readings on the EEPROM. The way you set
 * this up is by constructing it and passing in a moisture sensor. This moisture
 * sensor is the one the system will record. The system uses 1 + (2 * MAX_MOISTURE_RECORDINGS) 
 * bytes in the EEPROM to store the sensor data. The first byte stores how many 
 * sensor readings the EEPROM is currently holding. Every recorded value is 
 * stored in a list after the first byte. Each element in that list is two bytes. 
 * To use the system, you call 'update_system' and pass in a delta time in 
 * milliseconds. Once an internal timer passes 'MOISTURE_RECORDING_TIME', the 
 * system will record a new moisture value. If the maximum number of moisture 
 * values is already stored, it will delete the oldest value so it can make room
 * for the new one.
 */
class MoistureRecording
{
public:

  /**
   * Default constructor.
   */
  MoistureRecording();

  /**
   * Constructor.
   * @param sensor Moisture sensor to record from.
   */
  MoistureRecording(MoistureSensor& sensor);

  /**
   * Update the moisture recording system.
   * @param dt Time since last updating.
   */
  void update_system(const unsigned long dt);

  /**
   * Clears all moisture sensor readings.
   */
  void clear_readings();

  /**
   * Prints all readings to serial output.
   */
  void print_readings();

private:

  /** Moisture sensor to record from. */
  MoistureSensor* m_sensor;

  /** Timer used for waiting to record. */
  unsigned long m_timer;

  /** Allocated EEPROM memory. */
  int16_t m_eeprom_mem;
};

#endif
