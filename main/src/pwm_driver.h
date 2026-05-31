#ifndef PWM_DRIVER_H
#define PWM_DRIVER_H

#include <stdint.h>

/**
 * Initialize PWM for pulse control
 */
void pwm_init(void);

/**
 * Set PWM frequency
 * @param frequency Pulse frequency in Hz (0.5-10 Hz)
 */
void pwm_set_frequency(float frequency);

/**
 * Set PWM amplitude/duty cycle
 * @param amplitude Amplitude as percentage (0-100%)
 */
void pwm_set_amplitude(float amplitude);

#endif // PWM_DRIVER_H
