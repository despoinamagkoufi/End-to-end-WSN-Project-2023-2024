#include <math.h>
#include <stdio.h> // Include stdio.h for sprintf

#ifndef UTILS_H
#define UTILS_H

struct data_form {
  unsigned ID;
  unsigned count;
  double temp;
  double hum;
};

// Converts a double value to a string with two decimal places
void double2str(char *buf, double num) {
  int integer_part = (int)num;
  int fractional_part = (int)((num - integer_part) * 100);

  // Handle the absolute value manually
  if (fractional_part < 0) {
    fractional_part = -fractional_part;
  }

  // Ensure two decimal digits are printed, even if the fractional part is less than 10
  sprintf(buf, "%d.%02d", integer_part, fractional_part);
}

// Converts the raw temperature sensor value to a double value (Celsius)
double temperature_int2double(int value) {
  if (value != -1) {
    return (0.01 * value) - 39.60;  // Valid conversion
  }
  return -1.0;  // Use -1.0 for invalid readings
}

// Converts the raw humidity sensor value to a double value (%RH)
double humidity_int2double(int value) {
  if (value != -1) {
    double linear_humidity = (0.0405 * value) - 4.0;
    double correction_factor = (-2.8 * 0.000001) * (pow(value, 2));
    return linear_humidity + correction_factor;
  }
  return -1.0;  // Use -1.0 for invalid readings
}

#endif
