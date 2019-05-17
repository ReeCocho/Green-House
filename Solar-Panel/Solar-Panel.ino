/* Includes. */
#include "photo_sensor.h"
#include "motor_controller.h"
#include "sun_tracker.h"

// Motor controller
MotorController motor_controller = { 12, 11 };

// Sun tracker
SunTracker sun_tracker = { A0, A1 };

void setup() 
{
  // We need to immedietly disable the motor so it doesn't break anything
  Serial.begin(9600);
    
  // This disables every pin at the start of the program,
  // reducing power consumption by a little bit.
  for (byte i = 0; i <= A5; ++i)
  {
    pinMode (i, OUTPUT);
    digitalWrite(i, LOW);
  }

  // Limit switch pins
  pinMode(10, INPUT);
  pinMode(9, INPUT);

  // Initialize systems                         
  motor_controller = { 12, 11 };
  sun_tracker = { A0, A1 };
}

/**
 * Turn in a direction until we find the sun or hit a limit switch.
 * @param d Direction to turn.
 * @return True if we found the sun. False if we hit a limit switch.
 */
static bool turn_to_sun(const MotorController::State d)
{
  // While we aren't pointing at the sun...
  while(!sun_tracker.is_pointing_at_sun())
  {
    // Turn in the requested direction
    motor_controller.set_state(d);

    // If we hit a limit switch, return false
    if(digitalRead(10) != HIGH || digitalRead(9) != HIGH) 
    {
      // Back up a little bit
      if(d == MotorController::State::Clockwise)
        motor_controller.set_state(MotorController::State::CounterClockwise);
      else
        motor_controller.set_state(MotorController::State::Clockwise);

      delay(750);
      motor_controller.set_state(MotorController::State::Off);
        
      return false;
    }
  }

  motor_controller.set_state(MotorController::State::Off);

  // Return true if we found the sun
  return true;
}

void loop()
{
  /*
  //motor_controller.set_state(MotorController::State::Clockwise);
  delay(1000);
  motor_controller.set_state(MotorController::State::Off);
  delay(1000);
  motor_controller.set_state(MotorController::State::CounterClockwise);
  delay(1000);
  motor_controller.set_state(MotorController::State::Off);
  delay(1000);
  */

  /*
  while(digitalRead(11) == HIGH && digitalRead(10) == HIGH)
    motor_controller.set_state(MotorController::State::CounterClockwise);

  motor_controller.set_state(MotorController::State::Off);
  delay(3600000);
  */

  ///*
  // Turn counter clockwise first
  if(!turn_to_sun(MotorController::State::CounterClockwise))
    turn_to_sun(MotorController::State::Clockwise);

  // Turn off the motor
  motor_controller.set_state(MotorController::State::Off);

  // Wait for an hour before trying to find the sun again
  delay(3600000);
  //*/
}
