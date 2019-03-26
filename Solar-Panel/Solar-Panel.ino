/* Includes. */
#include "photo_sensor.h"
#include "motor_controller.h"

// Primary photo sensor used to find the sun
PhotoSensor photo_sensor = { 13 };

// Motor controller
MotorController motor_controller = { 12, 11 };

void setup() 
{
  Serial.begin(9600);
}

void loop() 
{  
  
}
