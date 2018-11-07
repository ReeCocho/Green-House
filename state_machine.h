#ifndef GH_STATE_MACHINE_H
#define GH_STATE_MACHINE_H

// Initialize the state machine
extern void init_state_machine();

// Run the state machine
// @param Time in miliseconds since last execution
extern void run_state_machine(const unsigned long dt);

// Forces the pump to stop running
extern void force_stop_pump();

// Forces the pump to run through one cycle
extern void force_start_pump();

// Resumes normal pump operation
extern void resume_pump();

#endif
