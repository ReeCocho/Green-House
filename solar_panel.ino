#include "solar_panel.h"

SolarPanel::SolarPanel(const int mpp, const int mpn, const int psp) :
  m_motor_pin_positive(mpp),
  m_motor_pin_negative(mpn),
  m_photo_sensor(PhotoSensor(psp)),
  m_movement(Movement::Stop)
{
  // Update pin modes
  pinMode(m_motor_pin_positive, OUTPUT);
  pinMode(m_motor_pin_negative, OUTPUT);
}

void SolarPanel::update(const unsigned long dt)
{
  // Update the motor
  switch(m_movement)
  {
  case Movement::TurnRight:
    digitalWrite(m_motor_pin_positive, HIGH);
    digitalWrite(m_motor_pin_negative, LOW);
    break;
  case Movement::TurnLeft:
    digitalWrite(m_motor_pin_positive, LOW);
    digitalWrite(m_motor_pin_negative, HIGH);
    break;
  case Movement::Stop:
    digitalWrite(m_motor_pin_positive, LOW);
    digitalWrite(m_motor_pin_negative, LOW);
    break;
  }
}
