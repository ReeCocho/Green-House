#ifndef SP_MOTOR_CONTROLLER_H
#define SP_MOTOR_CONTROLLER_H

/**
 * Class used to control the solar panel motor. The way it works is by
 * using two relays. The relays switch between positive and negative
 * for each terminal on the motor. This way, we can run the motor both
 * clockwise and counterclockwise and stop it when we want. The relays
 * should be wired such that when the positive motor terminal is set
 * to its on state, and the negative terminal is set to its off state,
 * the motor turns clockwise. The inverse being true for counterclockwise
 * motion.
 */
class MotorController
{
public:

  /**
   * Motor state.
   */
  enum class State
  {
      Off,
      Clockwise,
      CounterClockwise
  };

  /**
   * Constructor.
   * @param p1 Pin used to switch the positive terminal on the motor.
   * @param p2 Pin used to switch the negative terminal on the motor.
   */
  MotorController(const int p1, const int p2);

  /**
   * Update the motors state.
   * @param ns New motor state.
   */
  void set_state(const State ns);

private:

  /** Positive motor terminal. */
  int m_pos_motor_terminal;

  /** Negative m. */
  int m_neg_motor_terminal;
};

#endif
