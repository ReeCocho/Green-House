#ifndef GH_SOLAR_PANEL_H
#define GH_SOLAR_PANEL_H

/* Includes. */
#include "photo_sensor.h"

class SolarPanel
{
public:

  /**
   * Types of movement the solar panel motor can make.
   */
  enum class Movement
  {
    /** Turn to the right. */
    TurnRight,

    /** Turn to the left. */
    TurnLeft,

    /** Remain stationary. */
    Stop
  };

  /**
   * Constructor.
   * @param mpp Pin used to switch between positive and negative for the positive motor pin.
   * @param mpn Pin used to switch between positive and negative for the negative motor pin.
   * @param psp Pin used by the photo sensor.
   */
  SolarPanel(const int mpp, const int mpn, const int psp);

  /**
   * Update the solar panel.
   * @param dt Time in milliseconds since last updating the solar panel.
   */
  void update(const unsigned long dt);

private:

  /** Relay pin to switch the positive motor terminal. */
  int m_motor_pin_positive;

  /** Relay pin to switch the negative motor terminal. */
  int m_motor_pin_negative;

  /** Photo sensor used to control the motor. */
  PhotoSensor m_photo_sensor;

  /** Current movement the motor is performing. */
  Movement m_movement;
};

#endif
