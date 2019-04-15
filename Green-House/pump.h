#ifndef GH_PUMP_H
#define GH_PUMP_H

/**
 * The first node to run is 'psm_wait_until_dry'. It waits until the moisture sensor detects that
 * the soil is dry. When it is dry, it moves to 'psm_wait_after_dry' where the system idles for
 * 'PSM_WAIT_TIME_AFTER_DRY' milliseconds. Then, the system moves to 'psm_run_pump' which will
 * run the pumps for 'PSM_PUMP_RUN_TIME' milliseconds. Then the system will move to 'psm_idle_pump'
 * which will halt the system for 'PSM_PUMP_WAIT_TIME' milliseconds, and then check if the reservoir
 * still needs to be filled. If it does, it will move back to 'psm_run_pump'. Otherwise, it will
 * move to 'psm_wait_after_full' which waits for 'PSM_PUMP_WAIT_TIME_AFTER_FULL' milliseconds, and
 * then moves back to 'psm_wait_until_dry'.
 */

/* Includes. */
#include "state_machine.h"

/** Time to wait after the moisture sensor reads dry. (In milliseconds) */
#define PUMP_WAIT_TIME_AFTER_DRY 10800000

/** Pump run time. (In milliseconds) */
#define PUMP_RUN_TIME 10000

/** Pump wait time. (In milliseconds) */
#define PUMP_WAIT_TIME 5000

/** Value from the moisture sensor we consider dry. */
#define PUMP_SENSOR_DRY 250

/**
 * Pump state machine manager.
 */
class PumpStateMachine
{
public:

  /**
   * Constructor.
   * @param pump_pin Digital pin used for the pump.
   * @param float_switch Float switch inside the Earth Box reservoir.
   * @param m_sensor Optional moisture sensor pin. (Make it negative if it is not used).
   * @param r_float_switch Reservoirs float switch.
   */
  PumpStateMachine(const int pump_pin, const int float_switch, const int m_sensor, const int r_float_switch);

  /**
   * Destructor.
   */
  ~PumpStateMachine();

  /** State machine controller. */
  StateMachine m_state_machine;
  
  /** Pump pin. */
  const int m_pump_pin;

  /** Earth Box float switch pin. */
  const int m_float_switch;

  /** Optional moisture sensor. */
  MoistureSensor* m_moisture_sensor;

  /** The reservoirs float switch. */
  const int m_reservoir_float_switch;

  /** Timer used for waiting after the pump has run dry. */
  unsigned long m_wait_after_dry_timer;

  /** Timer used for running the pump for 'PUMP_RUN_TIME' milliseconds. */
  unsigned long m_run_pump_timer;

  /** Timer used to idle the pump after running. */
  unsigned long m_idle_pump_timer;
};

/**
 * State machine node which waits until the EarthBox is dry.
 * @note See StateMachine::Node for parameter definitions.
 * @note This is state 0.
 */
extern void psm_wait_until_dry(const unsigned long dt, StateMachine& sm);

/**
 * State machine node which waits after the EarthBox is dry.
 * @note See StateMachine::Node for parameter definitions.
 * @note This is state 1.
 */
extern void psm_wait_after_dry(const unsigned long dt, StateMachine& sm);

/**
 * State machine node which runs the pump.
 * @note See StateMachine::Node for parameter definitions.
 * @note This is state 2.
 */
extern void psm_run_pump(const unsigned long dt, StateMachine& sm);

/**
 * State machine node to which idles the pump
 * @note See StateMachine::Node for parameter definitions.
 * @note This is state 3.
 */
extern void psm_idle_pump(const unsigned long dt, StateMachine& sm);

#endif
