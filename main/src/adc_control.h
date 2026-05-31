#ifndef ADC_CONTROL_H
#define ADC_CONTROL_H

#include <stdint.h>

/**
 * Initialize ADC for potentiometer reading
 */
void adc_init(void);

/**
 * Read potentiometer value (0-4095)
 */
float adc_read_potentiometer(void);

#endif // ADC_CONTROL_H
