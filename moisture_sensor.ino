#include "constants.h"
#include "moisture_sensor.h"

/** CONSTANTS **/

// Window size for sensor median filter
#define MEDIAN_WINDOW_SIZE 8

// Number of elements in the window
#define MEDIAN_WINDOW_ELEM_COUNT (1+(MEDIAN_WINDOW_SIZE*2))

/** UTILITIES **/

// Bubble sort utility
static void sort(int a[], int size)
{
  // Sort sensor values (Bubble sort)
  for(int i=0; i<(size-1); i++) 
    for(int o=0; o<(size-(i+1)); o++) 
      if(a[o] > a[o+1]) 
      {
        int t = a[o];
        a[o] = a[o+1];
        a[o+1] = t;
      }
}

/** GLOBALS **/

// Sensor value median filter
static int sensor_values[MEDIAN_WINDOW_ELEM_COUNT] = {};
int moisture_sensor_value = 0;

void init_moisture_sensor()
{
  // Setup sensor median
  const auto sensor_val = analogRead(M_SENSOR_PIN);
  for(size_t i = 0; i < MEDIAN_WINDOW_ELEM_COUNT; ++i)
    sensor_values[i] = sensor_val;
}

void update_moisture_led(const unsigned long dt)
{
  // Timer used to print the moisture level every second
  static unsigned long print_timer = 0;
  
  // Normalize LED voltage from 0 to 1
  const auto val = moisture_sensor_value - DRY_VALUE;
  const float led_voltage = static_cast<float>(val) / static_cast<float>(MOIST_VALUE - DRY_VALUE);

  // // Map normalized LED voltage onto an exponential curve
  // const float volt_curve = clamp(((exp(led_voltage) - 1.0f) / (exp(1) - 1.0f)), 0.0f, 1.0f);

  // Update print timer
  print_timer += dt;

  // Print moisture level and reset timer if a second has passed
  if(print_timer >= 1000)
  {
    Serial.print("Moisture level ");
    Serial.print(moisture_sensor_value);
    Serial.println("%");
    print_timer = 0;
  }
  
  // analogWrite(M_SENSOR_LED_PIN, static_cast<int>(volt_curve * 256.0f));
}

void update_sensor_median(const unsigned long dt)
{
  // Timer used to update sensor median every second
  static unsigned long median_timer = 0;

  // Add delta time
  median_timer += dt;

  // If 1 second has passed...
  if(median_timer >= 1000)
  {
    // Update sensor value median
    for(size_t i = 0; i < MEDIAN_WINDOW_ELEM_COUNT - 1; ++i)
      sensor_values[i] = sensor_values[i + 1];
    sensor_values[MEDIAN_WINDOW_ELEM_COUNT - 1] = analogRead(M_SENSOR_PIN);

    // Sort median values
    int sorted_vals[MEDIAN_WINDOW_ELEM_COUNT] = {};
    memcpy(sorted_vals, sensor_values, MEDIAN_WINDOW_ELEM_COUNT * sizeof(int));
    sort(sorted_vals, MEDIAN_WINDOW_ELEM_COUNT);

    // Update sensor value
    moisture_sensor_value = sorted_vals[MEDIAN_WINDOW_SIZE];

    // Reset timer
    median_timer = 0;
  }
}
