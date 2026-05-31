#ifndef UI_DISPLAY_H
#define UI_DISPLAY_H

/**
 * Initialize the LCD display
 */
void ui_display_init(void);

/**
 * Update display with current control values
 */
void ui_display_update(float power, float frequency, float amplitude);

#endif // UI_DISPLAY_H
