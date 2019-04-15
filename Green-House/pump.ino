/** Includes. */
#include "pump.h"
#include "pins.h"
#include "cpp_mem.h"

/* So we can use the global stuff. */
extern PumpRecording pump_recording;

PumpStateMachine::PumpStateMachine(const int pump_pin, const int float_switch, const int m_sensor, const int r_float_switch) :
  m_pump_pin(pump_pin),
  m_float_switch(float_switch),
  m_reservoir_float_switch(r_float_switch),
  m_state_machine({})
{
  // Setup pins
  pinMode(pump_pin, OUTPUT);
  pinMode(float_switch, INPUT); 
  pinMode(m_reservoir_float_switch, INPUT);
  
  // Create the moisture sensor
  m_moisture_sensor = m_sensor < 0 ? NULL : new MoistureSensor(m_sensor);

  // Setup the state machine
  StateMachine::Node node = {};
  node.data = (void*)this;
  
  node.func = &psm_wait_until_dry;
  m_state_machine.add_node(node);

  node.func = &psm_wait_after_dry;
  m_state_machine.add_node(node);

  node.func = &psm_run_pump;
  m_state_machine.add_node(node);

  node.func = &psm_idle_pump;
  m_state_machine.add_node(node);
}

PumpStateMachine::~PumpStateMachine()
{
  delete m_moisture_sensor;
}

void psm_wait_until_dry(const unsigned long dt, StateMachine& sm, void* data)
{
  PumpStateMachine* psm = static_cast<PumpStateMachine*>(data);
  
  // Turn off the pump
  digitalWrite(psm->m_pump_pin, LOW);

  // Switch states if the float switch is on
  if(digitalRead(psm->m_float_switch) != HIGH)
  {
    // Moisture sensor is optional
    if(psm->m_moisture_sensor != NULL && psm->m_moisture_sensor->read_value() > PUMP_SENSOR_DRY)
      return;
    
    sm.set_active_node(1);
  }
}

void psm_wait_after_dry(const unsigned long dt, StateMachine& sm, void* data)
{
  PumpStateMachine* psm = static_cast<PumpStateMachine*>(data);
  
  // Turn off the pump
  digitalWrite(psm->m_pump_pin, LOW);

  // Update timer
  psm->m_wait_after_dry_timer += dt;

  // If we have waited for the requested time...
  if(psm->m_wait_after_dry_timer >= PUMP_WAIT_TIME_AFTER_DRY)
  {
    // Reset timer
    psm->m_wait_after_dry_timer = 0;

    // Move to psm_run_pump
    sm.set_active_node(2);

    // Say we've run the pump
    pump_recording.pump_run();
  }
}

void psm_run_pump(const unsigned long dt, StateMachine& sm, void* data)
{
  PumpStateMachine* psm = static_cast<PumpStateMachine*>(data);

  // Turn on the pump only if the reservoir is not empty
  if(digitalRead(psm->m_reservoir_float_switch) == LOW)
    digitalWrite(psm->m_pump_pin, HIGH);

  // Update pump timer
  psm->m_run_pump_timer += dt;

  // If we have run for the requested amount of time...
  if(psm->m_run_pump_timer >= PUMP_RUN_TIME)
  {
    // Reset pump timer
    psm->m_run_pump_timer = 0;

    // If the float switch is off, go back to waiting for it to be on. 
    // Otherwise, idle for the requested amount of time and go back to
    // running the pump.
    sm.set_active_node(digitalRead(psm->m_float_switch) == LOW ? 0 : 3);
  }
}

void psm_idle_pump(const unsigned long dt, StateMachine& sm, void* data)
{
  PumpStateMachine* psm = static_cast<PumpStateMachine*>(data);
  
  // Turn off the pump
  digitalWrite(psm->m_pump_pin, LOW);
  
  // Update the pump timer
  psm->m_idle_pump_timer += dt;

  // If we have waited for the requested amount of time...
  if(psm->m_idle_pump_timer >= PUMP_WAIT_TIME)
  {
    // Reset the pump timer
    psm->m_idle_pump_timer = 0;

    // Go back to running the pump
    sm.set_active_node(2);
  } 
}
