#ifndef SP_SUN_TRACKER_H
#define SP_SUN_TRACKER_H

/* Includes. */
#include "photo_sensor.h"

/**
 * Value below which the sun tracker will consider a photo sensor
 * in shadow or darkness.
 */
#define SUN_TRACKER_SHADOW_THRESHOLD 100

/** 
 * Sensitivity value for the sun tracker. Values can range from 1
 * to 1023. The higher the value, the more sensitive it is.
 */
#define SUN_TRACKER_SENSITIVITY 10

/**
 * Object which uses two photo sensors to detect
 * when it is facing the sun.
 */
class SunTracker
{
public:

  /**
   * Constructor.
   * @param ps1 Pin for the first photo sensor.
   * @param ps2 Pin for the second photo sensor.
   */
  SunTracker(const int ps1, const int ps2);

  /**
   * Returns if the sun tracker is pointing at the sun or not.
   * @return If the sun tracker is pointing at the sun or not.
   */
  bool is_pointing_at_sun() const;

private:

  /** First photo sensor. */
  PhotoSensor m_photo_sensor_1;

  /** Second photo sensor. */
  PhotoSensor m_photo_sensor_2;
};

#endif
