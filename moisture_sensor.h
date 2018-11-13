#ifndef GH_MOISTURE_SENSOR_H
#define GH_MOISTURE_SENSOR_H

/** Minimum value from the moisture sensor. (Dry) */
#define MS_DRY_VALUE 0

/** Maximum value from the moisture sensor. (Wet) */
#define MS_WET_VALUE 560

/**
 * Moisture sensor utility class.
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
   * @param Pin the sensor is connected too.
   */
  MoistureSensor(const int pin);

  /**
   * Read a value from the moisture sensor.
   * @return Value ranging from 0 to 1024, where 0 is the minimum and 1024 is the maximum.
   */
  uint16_t read_value() const;

private:

  /** Moisture sensor pin. */
  const int m_pin;
};

#endif
