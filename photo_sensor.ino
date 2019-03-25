#include "photo_sensor.h"

PhotoSensor::PhotoSensor(const int pin) : m_pin(pin)
{

}

uint16_t PhotoSensor::read_value() const
{
  // Read value from the sensor
  auto v = analogRead(m_pin);

  // Clamp value
  if(v > PHOTO_SENSOR_DARKEST)
    v = PHOTO_SENSOR_DARKEST;
  else if(v < PHOTO_SENSOR_BRIGHTEST)
    v = PHOTO_SENSOR_BRIGHTEST;

  // Scale value from 0 to 1
  float scaled_v =  static_cast<float>(v - PHOTO_SENSOR_BRIGHTEST) / 
                    static_cast<float>(PHOTO_SENSOR_DARKEST - PHOTO_SENSOR_BRIGHTEST);

  // Reverse value because the photo sensor will actually give
  // higher values for darkness and lower values for brightness.
  scaled_v = 1.0f - scaled_v;

  // Return value scaled from 0 to 1023
  return static_cast<uint16_t>(scaled_v * 1023.0f);
}
