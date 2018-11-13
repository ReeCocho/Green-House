#ifndef GH_PUMP_H
#define GH_PUMP_H

/** Includes. */
#include "state_machine.h"

// Pump run time (In milliseconds)
#define PUMP_RUN_TIME 10000

// Pump wait time (In milliseconds)
#define PUMP_WAIT_TIME 5000

/**
 * State machine node which waits until the EarthBox is dry.
 * @note See StateMachine::Node for parameter definitions.
 */
extern void psm_wait_until_dry(const unsigned long dt, StateMachine& sm);

/**
 * State machine node which runs the pump.
 * @note See StateMachine::Node for parameter definitions.
 */
extern void psm_run_pump(const unsigned long dt, StateMachine& sm);

/**
 * State machine node to which idles the pump
 * @note See StateMachine::Node for parameter definitions.
 */
extern void psm_idle_pump(const unsigned long dt, StateMachine& sm);

#endif
