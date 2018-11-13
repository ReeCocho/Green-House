/** Includes. */
#include "moisture_sensor.h"

MoistureSensor::MoistureSensor() : m_pin(-1)
{
  
}

MoistureSensor::MoistureSensor(const int pin) : m_pin(pin)
{
  
}

uint16_t MoistureSensor::read_value() const
{
  auto val = analogRead(m_pin);
  if(val < MS_DRY_VALUE) val = MS_DRY_VALUE;
  if(val > MS_WET_VALUE) val = MS_WET_VALUE;
  return static_cast<uint16_t>((static_cast<float>(val - MS_DRY_VALUE) / static_cast<float>(MS_WET_VALUE - MS_DRY_VALUE)) * 1024.0f);
}
