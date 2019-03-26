/** Includes. */
#include <EEPROM.h>
#include "moisture_recording.h"

/* This variable is defined in Green-House.ino. */
extern EEPROMMemoryManager eeprom;

MoistureRecording::MoistureRecording() : m_sensor(NULL), m_timer(0)
{
  // Allocate memory for recording storage
  m_eeprom_mem = eeprom.alloc(1 + (sizeof(uint16_t) * MAX_MOISTURE_RECORDINGS));
}

MoistureRecording::MoistureRecording(MoistureSensor& sensor) : 
  m_sensor(&sensor), 
  m_timer(0)
{
  // Allocate memory for recording storage
  m_eeprom_mem = eeprom.alloc(1 + (sizeof(uint16_t) * MAX_MOISTURE_RECORDINGS));
}

void MoistureRecording::update_system(const unsigned long dt)
{ 
  // Update the internal timer
  m_timer += dt;

  // If enough time has passed...
  if(m_timer >= MOISTURE_RECORDING_TIME)
  {
    // Reset the timer
    m_timer = 0;

    // Read the current number of recordings
    const auto count = static_cast<unsigned char>(eeprom.read(m_eeprom_mem, 0));

    // Delete the oldest value
    if(count == MAX_MOISTURE_RECORDINGS)
    {
      // We do this basically by scooting all the recorded values backward in the eeprom
      for(int i = 0; i < MAX_MOISTURE_RECORDINGS - 1; ++i)
      {    
        // Read the next value
        uint16_t val = 0;
        char* bval = reinterpret_cast<char*>(&val);
        bval[0] = eeprom.read(m_eeprom_mem, 1 + ((i + 1) * sizeof(uint16_t)));
        bval[1] = eeprom.read(m_eeprom_mem, 2 + ((i + 1) * sizeof(uint16_t)));
        
        // Put the next value into the current values position
        eeprom.write(m_eeprom_mem, 1 + (count * sizeof(uint16_t)), bval[0]);
        eeprom.write(m_eeprom_mem, 2 + (count * sizeof(uint16_t)), bval[1]);
      }
    }
    else
    {
      // Update the number of recorded values
      eeprom.write(m_eeprom_mem, 0, count + 1);
    }

    /* Because you can't write anything larger than 1 byte at a time to the EEPROM,
     * we convert the current value in the moisture sensor into an array of bytes.
     * We then write each byte individualy into the EEPROM.
     */
    const auto val = m_sensor->read_value();
    const char* bval = reinterpret_cast<const char*>(&val);

    // Write each byte
    eeprom.write(m_eeprom_mem, 1 + (count * sizeof(uint16_t)), bval[0]);
    eeprom.write(m_eeprom_mem, 2 + (count * sizeof(uint16_t)), bval[1]);
  }
}

void MoistureRecording::clear_readings()
{
  // Just say that the number of readings is 0 (Big brain)
  eeprom.write(m_eeprom_mem, 0, 0);
}

void MoistureRecording::print_readings()
{
  // Read the number of recordings
  const auto count = static_cast<unsigned char>(eeprom.read(m_eeprom_mem, 0));

  // Print special message if it's empty
  if(count == 0)
  {
    Serial.println("No recordings...");
    return;
  }

  // Otherwise, print all values
  Serial.println("Recorded moisture values:");
  for(unsigned char i = 0; i < count; ++i)
  {
    /* We can't read anything larger than 1 byte at a time from the EEPROM,
     * so in order to read the uint16_t, we read each byte individually. Then,
     * we reconstruct it.
     */
    uint16_t val = 0;
    char* bval = reinterpret_cast<char*>(&val);
    bval[0] = eeprom.read(m_eeprom_mem, 1 + (i * sizeof(uint16_t)));
    bval[1] = eeprom.read(m_eeprom_mem, 2 + (i * sizeof(uint16_t)));

    // Print value and it's index
    Serial.print(i);
    Serial.print(") ");
    Serial.println(val);
  }
}
