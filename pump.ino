/** Includes. */
#include "pump.h"
#include "pins.h"

void psm_wait_until_dry(const unsigned long dt, StateMachine& sm)
{
  // Turn off the pump
  digitalWrite(PUMP_PIN, LOW);

  // Switch states if the float switch is on
  if(digitalRead(EB_FLOAT_SWITCH_PIN) == HIGH)
    sm.set_active_node(1);
}

void psm_run_pump(const unsigned long dt, StateMachine& sm)
{
  // Timer used to run the pump. (In milliseconds)
  static unsigned long pump_timer = 0;

  // Turn on the pump
  digitalWrite(PUMP_PIN, HIGH);

  // Update pump timer
  pump_timer += dt;

  // If we have run for the requested amount of time...
  if(pump_timer >= PUMP_RUN_TIME)
  {
    // Reset pump timer
    pump_timer = 0;

    // If the float switch is off, go back to waiting for it to be on. 
    // Otherwise, idle for the requested amount of time and go back to
    // running the pump.
    sm.set_active_node(digitalRead(EB_FLOAT_SWITCH_PIN) == LOW ? 0 : 2);
  }
}

void psm_idle_pump(const unsigned long dt, StateMachine& sm)
{
  // Timer used to idle (In milliseconds)
  static unsigned long pump_timer = 0;

  // Turn off the pump
  digitalWrite(PUMP_PIN, LOW);
  
  // Update the pump timer
  pump_timer += dt;

  // If we have waited for the requested amount of time...
  if(pump_timer >= PUMP_WAIT_TIME)
  {
    // Reset the pump timer
    pump_timer = 0;

    // Go back to running the pump
    sm.set_active_node(1);
  } 
}
