/* Includes. */
#include <EEPROM.h>
#include "eeprom_mem_manager.h"

EEPROMMemoryManager::EEPROMMemoryManager() : m_allocs_len(0)
{
  // Allocate one byte for the allocation table so we can call 'realloc' on it.
  m_allocs = (EEPROMAlloc*)malloc(1);
}

EEPROMMemoryManager::~EEPROMMemoryManager()
{
  // Free the allocation table
  free(m_allocs);
}

int16_t EEPROMMemoryManager::alloc(const uint16_t len)
{
  // Precompute the number of bytes allocated so we can use it later
  const auto nba = num_bytes_allocated();
  
  // The number of bytes allocated + the size of our new allocation
  // must be less than or equal to the maximum number of bytes in
  // the EEPROM. Otherwise, we can't store the data.
  if(len + nba > static_cast<uint16_t>(EEPROM.length()))
    return -1;

  // Resize the allocation array to fit the new allocation
  m_allocs = (EEPROMAlloc*)realloc(m_allocs, sizeof(EEPROMAlloc) * ++m_allocs_len);

  // Add in the new allocation
  m_allocs[m_allocs_len - 1].len = len;
  m_allocs[m_allocs_len - 1].offset = nba;
  
  // The unique ID is just the index of the allocation
  return static_cast<int16_t>(m_allocs_len - 1);
}

char EEPROMMemoryManager::read(const int16_t id, const uint16_t i)
{
  return EEPROM.read(m_allocs[id].offset + i);
}

void EEPROMMemoryManager::write(const int16_t id, const uint16_t i, const char v)
{
  EEPROM.write(m_allocs[id].offset + i, v);
}

uint16_t EEPROMMemoryManager::num_bytes_allocated() const
{
  /* To compute the number of bytes allocated, we loop over
   * every allocation, and sum there lengths.
   */
  uint16_t len = 0;
  for(size_t i = 0; i < m_allocs_len; ++i)
    len += m_allocs[i].len;
  return len;
}
