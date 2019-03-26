#ifndef GH_EEPROM_MEM_MANAGER_H
#define GH_EEPROM_MEM_MANAGER_H

/* Includes. */
#include <stdlib.h>

/**
 * Object which manages memory in the EEPROM. To use it,
 * you construct the object like normal. Then, whenever
 * you want to allocate memory you call the 'alloc' method.
 * Memory is only released upon restarting the Arduino.
 */
class EEPROMMemoryManager
{
public:

  /**
   * Constructor.
   */
   EEPROMMemoryManager();

  /**
   * Destructor.
   */
   ~EEPROMMemoryManager();

  /**
   * Allocate a chunk of memory in the EEPROM.
   * @param len Number of bytes to allocate.
   * @return Unique identifier for the allocation.
   * @note Returns -1 when an error occured during allocation.
   */
   int16_t alloc(const uint16_t len);

  /**
   * Read a byte at the given index for the given allocation.
   * @param id Allocation's unique ID.
   * @param i Index to read at.
   * @return Value of the byte at the given location.
   * @note This function does no bounds checking.
   */
   char read(const int16_t id, const uint16_t i);

   /**
    * Write a byte to the given index for the given allocation.
    * @param id Allocation's unique ID.
    * @param i Index to write at.
    * @param v Value to write.
    */
    void write(const int16_t id, const uint16_t i, const char v);

  /**
   * Compute the number of bytes currently allocated.
   * @return Number of bytes allocated.
   */
   uint16_t num_bytes_allocated() const;

private:

  /**
   * Information about allocated EEPROM memory.
   */
   struct EEPROMAlloc
   {
      /** Number of bytes allocated. */
      uint16_t len;

      /** Offset in bytes from 0 where the allocation begins in the EEPROM. */
      uint16_t offset;
   };

   /** List of EEPROM allocations. */
   EEPROMAlloc* m_allocs;

   /** Number of EEPROM allocations. */
   size_t m_allocs_len;
};

#endif
