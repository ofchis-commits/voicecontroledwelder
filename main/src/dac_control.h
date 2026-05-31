#ifndef DAC_CONTROL_H
#define DAC_CONTROL_H

#include <stdint.h>

/**
 * Initialize DAC for voltage output
 */
void dac_init(void);

/**
 * Set DAC output voltage (0-15V)
 * @param voltage Desired output voltage in volts
 */
void dac_set_voltage(float voltage);

#endif // DAC_CONTROL_H
