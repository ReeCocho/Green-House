/** Includes. */
#include <EEPROM.h>
#include "moisture_recording.h"

MoistureRecording::MoistureRecording() : m_sensor(NULL), m_timer(0)
{
  
}

MoistureRecording::MoistureRecording(MoistureSensor& sensor) : 
  m_sensor(&sensor), 
  m_timer(0)
{
  
}

void MoistureRecording::update_system(const unsigned long dt)
{
  // Update timer
  m_timer += dt;

  // If enough time has passed...
  if(m_timer >= MOISTURE_RECORDING_TIME)
  {
    // Reset the timer
    m_timer = 0;

    // Read the number of recordings
    const auto count = static_cast<const unsigned char>(EEPROM.read(0));

    // Stop if the count is at maximum
    if(count == MAX_MOISTURE_RECORDINGS)
      return;

    // Update count
    EEPROM.write(0, count + 1);

    // Read the current moisture value from the sensor
    // and separate the lower and upper byte.
    const auto val = m_sensor->read_value();
    const uint8_t* bval = reinterpret_cast<const uint8_t*>(&val);

    // Write the sensor value
    EEPROM.write(1 + (count * sizeof(uint16_t)), bval[0]);
    EEPROM.write(2 + (count * sizeof(uint16_t)), bval[1]);
  }
}

void MoistureRecording::clear_readings()
{
  // Just say that the number of readings is 0 (Big brain)
  EEPROM.write(0, 0);
}

void MoistureRecording::print_readings()
{
  // Read the number of recordings
  const auto count = static_cast<const unsigned char>(EEPROM.read(0));

  // Print special message if it's empty
  if(count == 0)
  {
    Serial.println("No recordings...");
    return;
  }

  // Print all values
  Serial.println("Recorded moisture values:");
  for(unsigned char i = 0; i < count; ++i)
  {
    // Read value
    uint16_t val = 0;
    uint8_t* bval = reinterpret_cast<uint8_t*>(&val);
    bval[0] = EEPROM.read(1 + (count * sizeof(uint16_t)));
    bval[1] = EEPROM.read(2 + (count * sizeof(uint16_t)));

    // Print value and it's index
    Serial.print(i);
    Serial.print(") ");
    Serial.println(val);
  }
}
