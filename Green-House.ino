/** INCLUDES */
#include <EEPROM.h>
#include "constants.h"
#include "state_machine.h"
#include "moisture_sensor.h"
#include "commands.h"

/** GLOBAL VARIABLES **/

// Timer used to calculate delta time
static unsigned long last_dt_timer = 0;
static unsigned long cur_dt_timer = 0;

// Update EEPROM with sensor values
void update_eeprom(const unsigned long dt)
{
  // Timer used to update EEPROM every hour
  static unsigned long eeprom_timer = 0;

  // Add delta time
  eeprom_timer += dt;

  // If an hour has passed...
  if(eeprom_timer >= 3600000)
  {
    // Read EEPROM counter
    char count = EEPROM.read(0);

    // Stop if the count is at a maximum
    if(count == 255)
    {
      eeprom_timer = 0;
      return;
    }
    
    // Update counter
    EEPROM.write(0, count + 1);

    // Convert the median value into a short and separate the lower and upper byte
    const uint16_t val = static_cast<uint16_t>(moisture_sensor_value);
    const uint8_t* bval = reinterpret_cast<const uint8_t*>(&val);

    // Write sensor value
    for(int i = 0; i < sizeof(uint16_t); ++i)
      EEPROM.write(1 + i + (count * sizeof(uint16_t)), bval[i]);

    // Reset timer
    eeprom_timer = 0;
  }
}

void setup() 
{
  // Initialize serial output
  Serial.begin(9600);
  
  // Setup digital pins
  pinMode(M_SENSOR_LED_PIN, OUTPUT);
  pinMode(PUMP_PIN, OUTPUT);
  pinMode(EB_FLOAT_SWITCH_PIN, INPUT);
  delay(500);

  // Initialize the state machine
  init_state_machine();

  // Initialize the moisture sensor
  init_moisture_sensor();

  // Initialize delta time timer
  last_dt_timer = millis();
  cur_dt_timer = last_dt_timer;
}

void loop() 
{
  // Update delta time
  last_dt_timer = cur_dt_timer;
  cur_dt_timer = millis();
  const auto dt = cur_dt_timer - last_dt_timer;

  // Process user input
  process_input();

  // Update EEPROM
  update_eeprom(dt);

  // Update sensor median
  update_sensor_median(dt);

  // Update moisture sensor LED
  update_moisture_led(dt);

  // Run state machine
  run_state_machine(dt);
  
  // Wait 1 milliseconds so the sensors have time to update
  delay(1);
}
