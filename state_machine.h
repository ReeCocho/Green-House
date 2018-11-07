#ifndef GH_STATE_MACHINE_H
#define GH_STATE_MACHINE_H

// Initialize the state machine
extern void init_state_machine();

// Run the state machine
// @param Time in miliseconds since last execution
extern void run_state_machine(const unsigned long dt);

#endif
