/* Includes. */
#include "moisture_recording.h"
#include "commands.h"
#include "pump.h"
#include "pins.h"

/** Delta time variables. */
static unsigned long last_time;
static unsigned long current_time;

/** Pump state machine. */
static StateMachine psm = {};

/** Moisture sensor. */
MoistureSensor sensor = MoistureSensor(MOISTURE_SENSOR_PIN);

/** Moisture recording system. */
static MoistureRecording moisture_recording = MoistureRecording(sensor);

/** Command manager. */
static CommandManager commands = {};

/**
 * Command to clear moisture readings.
 */
void cmd_clear_moisture_values()
{
  Serial.println("Clearing values...");
  moisture_recording.clear_readings();
}

/**
 * Command to print moisture readings.
 */
void cmd_print_moisture_values()
{
  moisture_recording.print_readings();
}

/**
 * Command to print current moisture reading.
 */
void cmd_print_current_moisture()
{
  Serial.print("Current Moisture: ");
  Serial.println(sensor.read_value());
}

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

  node.func = &psm_wait_after_dry;
  psm.add_node(node);

  node.func = &psm_run_pump;
  psm.add_node(node);

  node.func = &psm_idle_pump;
  psm.add_node(node);

  // Initialize commands
  CommandManager::Command command = {};
  
  command.str = "clear_moisture_values";
  command.func = &cmd_clear_moisture_values;
  commands.add_command(command);

  command.str = "print_moisture_values";
  command.func = &cmd_print_moisture_values;
  commands.add_command(command);

  command.str = "print_current_moisture";
  command.func = &cmd_print_current_moisture;
  commands.add_command(command);

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
  
  // Poll for commands
  commands.poll();

  // Run the state machine
  psm.execute(dt);

  // Update moisture recording
  moisture_recording.update_system(dt);

  // Wait 1 millisecond so the sensors have time to update
  delay(1);
}
