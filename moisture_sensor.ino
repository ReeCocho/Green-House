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
  // Read the value off the moisture sensor (Ranges from 0 to 1024)
  auto val = analogRead(m_pin);

  // Clamp it between the minimum and maximum value
  if(val < MS_DRY_VALUE) val = MS_DRY_VALUE;
  if(val > MS_WET_VALUE) val = MS_WET_VALUE;

  // This big mess basically normalizes 'val' so it ranges from 0 to 1024 by first scaling it so
  // it's 0 when 'val' is MS_DRY_VALUE and 1 when 'val' is MS_WET_VALUE and then multiplying by 1024.
  return static_cast<uint16_t>((static_cast<float>(val - MS_DRY_VALUE) / static_cast<float>(MS_WET_VALUE - MS_DRY_VALUE)) * 1024.0f);
}
