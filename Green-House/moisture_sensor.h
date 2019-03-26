#ifndef GH_MOISTURE_SENSOR_H
#define GH_MOISTURE_SENSOR_H

/** Minimum theoretical value the moisture sensor will read. (Dry) */
#define MS_DRY_VALUE 0

/** Maximum theoretical value the moisture sensor will read. (Wet) */
#define MS_WET_VALUE 560

/**
 * A basic utility class which treats a pin as a moisture sensor.
 * You can poll the current value of the moisture sensor using 'read_value'.
 */
class MoistureSensor
{
public:

  /**
   * Default constructor.
   */
  MoistureSensor();

  /**
   * Constructor.
   * @param pin Analog pin the sensor is connected too.
   */
  MoistureSensor(const int pin);

  /**
   * Read a value from the moisture sensor.
   * @return The return value ranges from 0 to 1024, where 0 is the minimum and 1024 is the maximum.
   */
  uint16_t read_value() const;

private:

  /** Moisture sensor pin. */
  const int m_pin;
};

#endif
