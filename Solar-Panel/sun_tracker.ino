/* Includes. */
#include "sun_tracker.h"

SunTracker::SunTracker(const int ps1, const int ps2) :
  m_photo_sensor_1(ps1),
  m_photo_sensor_2(ps2)
{

}

bool SunTracker::is_pointing_at_sun() const
{
  // Convert values to integers since we might get negatives
  const int16_t ps1_val = static_cast<const int16_t>(m_photo_sensor_1.read_value());
  const int16_t ps2_val = static_cast<const int16_t>(m_photo_sensor_2.read_value());

  // Compute difference between the vaues
  const int16_t diff = ps1_val - ps2_val;

  Serial.println(diff);

  // Compare difference with the sensitivity. Also, at least one of 
  // the photo sensors must illuminated, otherwise it's safe to assume
  // that it's night time.
  return (diff < SUN_TRACKER_SENSITIVITY && diff > -SUN_TRACKER_SENSITIVITY) &&
         (ps1_val > SUN_TRACKER_SHADOW_THRESHOLD || ps2_val > SUN_TRACKER_SHADOW_THRESHOLD);
}
