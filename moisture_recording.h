#ifndef GH_MOISTURE_RECORDING_H
#define GH_MOISTURE_RECORDING_H

/** Includes. */
#include "moisture_sensor.h"

/** Time (In milliseconds) in between moisture sensor recordings. */
#define MOISTURE_RECORDING_TIME 3600000

/** Maximum number of recordings stored. (Maximum value is 256, minimum is 1) */
#define MAX_MOISTURE_RECORDINGS 48

/**
 * Utility to record moisture sensor readings on the EEPROM.
 * @note The first byte in the EEPROM is used to store the current number of recordings.
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
   * @param Moisture sensor.
   */
  MoistureRecording(MoistureSensor& sensor);

  /**
   * Update the moisture recording system.
   * @param Time since last updating.
   */
  void update_system(const unsigned long dt);

  /**
   * Clears all moisture sensor readings.
   */
  void clear_readings();

  /**
   * Print all readings to serial output.
   */
  void print_readings();

private:

  /** Moisture sensor to record from. */
  MoistureSensor* m_sensor;

  /** Timer used for waiting to record. */
  unsigned long m_timer;
};

#endif
