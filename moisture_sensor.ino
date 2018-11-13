/** Includes. */
#include "moisture_sensor.h"

MoistureSensor::MoistureSensor(const int pin) : m_pin(pin)
{
  
}

float MoistureSensor::read_value() const
{
  auto val = analogRead(m_pin);
  if(val < MS_DRY_VALUE) val = MS_DRY_VALUE;
  if(val > MS_WET_VALUE) val = MS_WET_VALUE;
  return static_cast<float>(val - MS_DRY_VALUE) / static_cast<float>(MS_WET_VALUE - MS_DRY_VALUE);
}
