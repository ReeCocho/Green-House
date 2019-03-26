/* Includes. */
#include "motor_controller.h"

MotorController::MotorController(const int p1, const int p2) :
  m_pos_motor_terminal(p1),
  m_neg_motor_terminal(p2)
{
  // Update the pin modes
  pinMode(m_pos_motor_terminal, OUTPUT);
  pinMode(m_neg_motor_terminal, OUTPUT);
}

void MotorController::set_state(const State ns)
{
  switch(ns)
  {
  case State::Off:
    digitalWrite(m_pos_motor_terminal, LOW);
    digitalWrite(m_neg_motor_terminal, LOW);
    break;

  case State::Clockwise:
    digitalWrite(m_pos_motor_terminal, HIGH);
    digitalWrite(m_neg_motor_terminal, LOW);
    break;

  case State::CounterClockwise:
    digitalWrite(m_pos_motor_terminal, LOW);
    digitalWrite(m_neg_motor_terminal, HIGH);
    break;
  }
}
