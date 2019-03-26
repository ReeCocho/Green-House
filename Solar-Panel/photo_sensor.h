#ifndef SP_PHOTO_SENSOR
#define SP_PHOTO_SENSOR

/* Includes. */

/** Value representing the darkest possible output from the photo sensor. */
#define PHOTO_SENSOR_DARKEST 1023

/** Value representing the brightest possible output from the photo sensor. */
#define PHOTO_SENSOR_BRIGHTEST 20

/**
 * Utility class to detect how much sunlight there is using a photoresistor.
 */
class PhotoSensor
{
public:

  /**
   * Constructor.
   * @param pin The pin used by the sensor.
   * @note 'pin' must be compatible with 'analogRead'.
   */
   PhotoSensor(const int pin);

  /**
   * Read a value from the photo sensor.
   * @return The value of the photo sensor.
   * @note 0 is darkest and 1023 brightest.
   */
   uint16_t read_value() const;
  
private:

  /** Pin the sensor uses. */
  int m_pin;
};

#endif
