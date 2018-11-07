#include "state_machine.h"
#include "constants.h"

// Pump run time (In milliseconds)
#define PUMP_RUN_TIME 15000

// Pump wait time (In milliseconds)
#define PUMP_WAIT_TIME 5000

/** STRUCTURES AND CLASSES **/

// Structure which contains a node in a state machine
struct StateMachineNode
{
  // Function to run for the node 
  // First parameter is time in milliseconds since last iteration of the state machine.
  // Second parameter is a pointer to some generic data used by the node
  void(*func)(const unsigned long, void*);
};

/** GLOBAL VARIABLES **/

// Pump state machine
static struct
{
  // Nodes
  StateMachineNode nodes[3];

  // Last active node
  size_t last_active_node;

  // Active node
  size_t active_node;
  
} pump_state_machine;

/** UTILITIES **/

// Clamp a float between a low and high number
inline static float clamp(float v, float lo, float hi)
{
  return v > hi ? hi : (v < lo ? lo : v);
}

/** PROGRAM **/

void init_state_machine()
{
  // Setup pump state machine
  pump_state_machine.last_active_node = 0;
  pump_state_machine.active_node = 0;
  pump_state_machine.nodes[0].func = &psm_wait_until_dry;
  pump_state_machine.nodes[1].func = &psm_run_pump;
  pump_state_machine.nodes[2].func = &psm_idle_pump;
}

void run_state_machine(const unsigned long dt)
{
  pump_state_machine.nodes[pump_state_machine.active_node].func(dt, NULL);
}

// Update the state machine
static void psm_update_state(const size_t new_state)
{
  pump_state_machine.last_active_node = pump_state_machine.active_node;
  pump_state_machine.active_node = new_state;
}

// State machine node which waits until the moisture sensor reads dry.
static void psm_wait_until_dry(const unsigned long, void*)
{
  // Turn off the pump
  digitalWrite(PUMP_PIN, LOW);

  // Switch states if the sensor reads above the pump on threshold
  if(digitalRead(EB_FLOAT_SWITCH_PIN) == HIGH)
    psm_update_state(1);
}

// State machine node which runs the pump for 10 seconds.
static void psm_run_pump(const unsigned long dt, void*)
{
  // Timer used to run the pump (In milliseconds)
  static unsigned long pump_timer = 0;

  // Number of times the pump has run sequentially
  static char pump_run_counter = 0;

  // Turn on the pump
  digitalWrite(PUMP_PIN, HIGH);

  // Update pump timer
  pump_timer += dt;

  // If we have run for 10 seconds...
  if(pump_timer >= PUMP_RUN_TIME)
  {
    // Reset pump timer
    pump_timer = 0;

    // If the last state was waiting until dry, reset the
    // pump run counter.
    if(pump_state_machine.last_active_node == 0)
      pump_run_counter = 0;

    // Increment the pump run counter
    ++pump_run_counter;

    // If we reached our target moisture, go back to waiting
    // until dry. Otherwise, idle the pump for 5 seconds.
    psm_update_state(digitalRead(EB_FLOAT_SWITCH_PIN) == LOW ? 0 : 2);
  }
}

// State machine node which idles the pump for 5 seconds
static void psm_idle_pump(const unsigned long dt, void*)
{
  // Timer used to idle (In milliseconds)
  static unsigned long pump_timer = 0;

  // Turn off the pump
  digitalWrite(PUMP_PIN, LOW);
  
  // Update the pump timer
  pump_timer += dt;

  // If we have waited for 5 seconds...
  if(pump_timer >= PUMP_WAIT_TIME)
  {
    // Reset the pump timer
    pump_timer = 0;

    // Update state
    psm_update_state(1);
  }
}

void force_stop_pump()
{
  Serial.println("Forcing pump to stop...");
}

void force_start_pump()
{
  Serial.println("Forcing pump to start...");
  psm_update_state(1);
}

void resume_pump()
{
  Serial.println("Resuming normal pump operation...");
}
