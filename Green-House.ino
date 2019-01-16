/**
 * This is the primary source file for the whole greenhouse project. It combines
 * four main components together to make the whole thing work. The state machine
 * (see state_machine.h), the moisture sensor (see moisture_sensor.h), the 
 * moisture recorder (see moisture_recording.h), and the command manager
 * (see command_manager.h). The program begins in 'setup', where initialization
 * occurs (setting up the state machine, commands, pins, etc.) Afterwards, the
 * 'loop' function runs in an infinite loop as fast as possible. This is where
 * all the magic happens.
 */

/*
 * Include libraries. See the individual files for their description. 
 */
#include "eeprom_mem_manager.h"
#include "moisture_recording.h"
#include "pump_recording.h"
#include "commands.h"
#include "pump.h"
#include "pins.h"

/*
 * NOTE: Notice that all the variables are 'static' except for a select few. The 
 * 'static' keyword makes the variable only exist in the current file. The reason
 * some of the variables are not static is because they are used in other files.
 */

/*
 * Variables we use to compute the time in milliseconds between runs of 'loop'. 
 */
static unsigned long last_time;
static unsigned long current_time;

/**
 * EEPROM memory manager. Allows for easier control over read/write operations
 * on the EEPROM.
 */
EEPROMMemoryManager eeprom = {};


/** 
 * The pumps state machine controller. This object manages all the watering logic.
 */
static PumpStateMachine psm = { 13, 12, A0 };
static PumpStateMachine psm2 = { 11, 10, -1 };

/** 
 * Object to read values off of the moisture sensor. We pass 'MOISTURE_SENSOR_PIN'
 * (which is defined in pins.h) which is the pin that the moisture sensor is
 * connected to.
 */
MoistureSensor sensor = MoistureSensor(MOISTURE_SENSOR_PIN);

/**
 * Object which records moisutre sensor readings onto the Arduinos EEPROM.
 * EEPROM stands for "electrically erasable programmable read only memory".
 * It's a little chip on the Arduino which we can use to store data even if
 * the Arduino is shut off. We pass the 'sensor' object to the constructor
 * which is the moisutre sensor the object will record.
 */
static MoistureRecording moisture_recording = MoistureRecording(sensor);

/**
 * Object which is used to record how much time passes between consecutive
 * pump runs.
 */
PumpRecording pump_recording = {};

/** 
 * Object that manages commands that we can input using the serial port.
 */
static CommandManager commands = {};

/**
 * Command to clear moisture readings.
 */
void cmd_clear_moisture_values()
{
  // Print out a little message so we know it worked
  Serial.println("Clearing recorded moisture values...");

  // Actually clear the recorded values
  moisture_recording.clear_readings();
}

/**
 * Command to print moisture readings.
 */
void cmd_print_moisture_values()
{
  // The moisture recorder has its own function to print the values
  moisture_recording.print_readings();
}

/**
 * Command to print current moisture reading.
 */
void cmd_print_current_moisture()
{
  // Print out the value
  Serial.print("Current Moisture: ");
  Serial.println(sensor.read_value());
}

/**
 * Command to print recorded pump run times.
 */
void cmd_print_pump_run_times()
{
  // The pump recording class has its own function to do this
  pump_recording.print_pump_run_times();
}

/**
 * Command to clear recorded pump run times.
 */
void cmd_clear_pump_run_times()
{
  // Print out a little message so we know it worked
  Serial.println("Clearing recorded pump run times...");

  // Actually clear the recorded values
  pump_recording.clear_pump_run_times();
}

/**
 * This is the setup function. It's called once when the Arduino is turned on
 * and before 'loop' is executed.
 */
void setup() 
{
  // Initialize serial io
  Serial.begin(9600);
  
  // The pump pin is an output (Because we are turning the pump on and off.)
  pinMode(PUMP_PIN, OUTPUT);

  // The float switch is an input (Because we are reading whether it is on or off.)
  pinMode(EB_FLOAT_SWITCH_PIN, INPUT);

  // Delay a little bit so the pins can update
  delay(500);

  /*
   * Now we initialize the command manager. We create an instance of the
   * command managers 'Command' class. We set the string that we need to
   * input to execute the command, and the function to execute when that
   * command name is entered via the serial input.
   */
  
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

  command.str = "print_pump_run_times";
  command.func = &cmd_print_pump_run_times;
  commands.add_command(command);

  command.str = "clear_pump_run_times";
  command.func = &cmd_clear_pump_run_times;
  commands.add_command(command);

  /*
   * Now we setup the delta time variables. Delta time is measured in
   * milliseconds, so we just poll the global timer and set both values
   * to that.
   */
   
  last_time = millis();
  current_time = last_time;
}

/**
 * This is the loop function. It is executed as fast as possible by the Arduino
 * after 'setup' is called. This is where most of the logic for our program goes.
 */
void loop() 
{
  /*
   * Here we compute the time in milliseconds since 'loop' was executed.
   * We do this by setting the 'last' value to whatever the 'current' value
   * for the previous loop iteration was. Then, we updated the 'current' value
   * with the Arduinos internal timer. Finally, we compute the change in time
   * between the 'current' time and the 'last' time.
   */
  
  last_time = current_time;
  current_time = millis();
  const auto dt = current_time - last_time;
  
  // First, check if any commands were input
  commands.poll();

  // Next, run the state machine
  psm.m_state_machine.execute(dt);
  psm2.m_state_machine.execute(dt);

  // Then, update the pump timer
  pump_recording.update(dt);

  // Finally, update moisture recorder
  moisture_recording.update_system(dt);

  // Wait 1 millisecond so the sensors have time to update
  delay(1);
}
