/* Includes. */
#include "pump_recording.h"

/* This variable is defined in Green-House.ino. */
extern EEPROMMemoryManager eeprom;

PumpRecording::PumpRecording() : m_timer(0)
{
  // Allocate memory in the EEPROM to store pump recordings
  m_eeprom_mem = eeprom.alloc(1 + (sizeof(unsigned long) * MAX_PUMP_RECORDINGS));
}

PumpRecording::~PumpRecording()
{
  
}

void PumpRecording::update(const unsigned long dt)
{
  // Just add the delta time to the timer
  m_timer += dt;
}

void PumpRecording::pump_run()
{
  // Read the current number of pump recordings
  const auto count = static_cast<const unsigned char>(eeprom.read(m_eeprom_mem, 0));

  // Stop if the number of recorded values is at maximum
  if(count == MAX_PUMP_RECORDINGS)
    return;

  // Update the number of recorded values
  eeprom.write(m_eeprom_mem, 0, count + 1);

  /* Because you can't write anything larger than 1 byte at a time to the EEPROM,
   * we convert the timer into an array of bytes. Then, we write each byte 
   * individualy into the EEPROM.
   */
  const char* bval = reinterpret_cast<const char*>(&m_timer);

  // Write each byte
  for(size_t i = 0; i < sizeof(unsigned long); ++i)
    eeprom.write(m_eeprom_mem, 1 + i + (count * sizeof(unsigned long)), bval[i]);

  // Reset timer
  m_timer = 0;
}

void PumpRecording::print_pump_run_times()
{
  // Read the current number of pump recordings
  const auto count = static_cast<unsigned char>(eeprom.read(m_eeprom_mem, 0));

  // Print a special message for 0 recorded values
  if(count == 0)
  {
    Serial.println("No recorded pump runs...");
    return;
  }

  // Otherwise, print all values
  Serial.println("Recorded pump run times (In seconds):");
  for(unsigned char i = 0; i < count; ++i)
  {
    /* We can't read anything larger than 1 byte at a time from the EEPROM,
     * so in order to read the unsigned long, we read each byte individually. Then,
     * we reconstruct it.
     */
    unsigned long val = 0;
    char* bval = reinterpret_cast<char*>(&val);
    for(size_t j = 0; j < sizeof(unsigned long); ++j)
      bval[j] = eeprom.read(m_eeprom_mem, 1 + j + (i * sizeof(unsigned long)));

    // Print value and it's index
    Serial.print(i);
    Serial.print(") ");
    Serial.println(val/1000);
  }
}

void PumpRecording::clear_pump_run_times()
{
  // Just set the number of recordings to 0 (Big brain)
  eeprom.write(m_eeprom_mem, 0, 0);
}
