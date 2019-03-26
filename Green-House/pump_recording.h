#ifndef GH_PUMP_RECORDING_H
#define GH_PUMP_RECORDING_H

/* Includes. */
#include <stdint.h>

/** Maximum number of recorded pump runs. */
#define MAX_PUMP_RECORDINGS 24

/**
 * A utility class to measure how many times the pump runs, and
 * how much time has passed in between pump runs. The way you
 * set this up is to just construct it like normal. Then, to use
 * it, you call 'pump_run' whenever the pump runs and 'update'
 * every loop while passing in the time in milliseconds since
 * 'update' was called.
 */
class PumpRecording
{
public:

  /**
   * Constructor.
   */
   PumpRecording();

  /**
   * Destructor.
   */
   ~PumpRecording();

  /**
   * Updates the internal timer.
   * @param dt Time in milliseconds since the pump last ran.
   */
   void update(const unsigned long dt);

   /**
    * Signal to the system that the pump has run.
    */
    void pump_run();

   /**
    * Print out the pump run times.
    */
    void print_pump_run_times();

    /**
     * Clear the stored pump run times.
     */
     void clear_pump_run_times();

private:

  /** Internal timer. */
  unsigned long m_timer;

  /** Allocated EEPROM memory. */
  int16_t m_eeprom_mem;
};

#endif
