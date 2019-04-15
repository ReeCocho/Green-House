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
  Serial.begin(9600);

  // This disables every pin at the start of the program,
  // reducing power consumption by a little bit.
  for (byte i = 0; i <= A5; ++i)
  {
    pinMode (i, OUTPUT);
    digitalWrite (i, LOW);
  }

  pinMode(11, INPUT);
  pinMode(10, INPUT);

  // Initialize systems                         
  motor_controller = { 12, 11 };
  sun_tracker = { A0, A1 };
}

void loop() 
{  
  Serial.print(digitalRead(11));
  Serial.print(" ");
  Serial.println(digitalRead(10));
  
  if(digitalRead(11) == HIGH || digitalRead(10) == HIGH)
    motor_controller.set_state(MotorController::State::Clockwise);
  else
    motor_controller.set_state(MotorController::State::Off);
  
  //motor_controller.set_state(MotorController::State::Clockwise);
  //delay(3000);
  //motor_controller.set_state(MotorController::State::Off);
  //delay(3000);
  //motor_controller.set_state(MotorController::State::CounterClockwise);
  //delay(3000);
  
  // sun_tracker.is_pointing_at_sun();
}
