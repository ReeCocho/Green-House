/** Includes. */
#include "state_machine.h"
#include "commands.h"
#include "pump.h"
#include "pins.h"

/** Delta time variables. */
static unsigned long last_time;
static unsigned long current_time;

/** Pump state machine. */
static StateMachine psm = {};

/** Command manager. */
static CommandManager commands = {};

void setup() 
{
  // Initialize serial monitoring
  Serial.begin(9600);
  
  // Initialize pins
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(EB_FLOAT_SWITCH_PIN, INPUT);
  delay(500);
  
  // Initialize state machine
  StateMachine::Node node = {};
  
  node.func = &psm_wait_until_dry;
  psm.add_node(node);

  node.func = &psm_run_pump;
  psm.add_node(node);

  node.func = &psm_idle_pump;
  psm.add_node(node);

  // Initialize timer
  last_time = millis();
  current_time = last_time;
}

void loop() 
{
  // Update delta time
  last_time = current_time;
  current_time = millis();
  const auto dt = current_time - last_time;

  Serial.println(digitalRead(EB_FLOAT_SWITCH_PIN));

  // Poll for commands
  commands.poll();

  // Run the state machine
  psm.execute(dt);

  // Wait 1 millisecond so the sensors have time to update
  delay(1);
}
