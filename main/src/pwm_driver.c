#include "pwm_driver.h"
#include "esp_log.h"
#include "driver/ledc.h"

static const char *TAG = "PWM_Driver";

// PWM configuration
#define PWM_GPIO_PIN GPIO_NUM_1      // GPIO1 for ESP32-P4
#define LEDC_TIMER LEDC_TIMER_0
#define LEDC_MODE LEDC_LOW_SPEED_MODE
#define LEDC_CHANNEL LEDC_CHANNEL_0
#define LEDC_DUTY_RES LEDC_TIMER_13_BIT  // 13-bit resolution for duty
#define LEDC_FREQUENCY 1              // Base frequency 1 Hz, modulated by software
#define LEDC_DUTY_MAX ((1 << 13) - 1) // Maximum duty value

void pwm_init(void)
{
    // Prepare and then apply the LEDC PWM timer configuration
    ledc_timer_config_t ledc_timer = {
        .speed_mode = LEDC_MODE,
        .timer_num = LEDC_TIMER,
        .duty_resolution = LEDC_DUTY_RES,
        .freq_hz = 1000,  // Higher base frequency for smooth control
        .clk_cfg = LEDC_AUTO_CLK
    };
    
    ESP_ERROR_CHECK(ledc_timer_config(&ledc_timer));
    
    // Prepare and then apply the LEDC PWM channel configuration
    ledc_channel_config_t ledc_channel = {
        .speed_mode = LEDC_MODE,
        .channel = LEDC_CHANNEL,
        .timer_sel = LEDC_TIMER,
        .intr_type = LEDC_INTR_DISABLE,
        .gpio_num = PWM_GPIO_PIN,
        .duty = 0,  // Set duty to 0%
        .hpoint = 0
    };
    
    ESP_ERROR_CHECK(ledc_channel_config(&ledc_channel));
    ESP_LOGI(TAG, "PWM initialized on GPIO%d", PWM_GPIO_PIN);
}

void pwm_set_frequency(float frequency)
{
    // Frequency modulation is done at the application level
    // This function would be called to set the base frequency
    if (frequency < 0.5f) frequency = 0.5f;
    if (frequency > 10.0f) frequency = 10.0f;
    
    ESP_LOGI(TAG, "PWM frequency set to %.2f Hz", frequency);
}

void pwm_set_amplitude(float amplitude)
{
    // Clamp amplitude to valid range (0-20V typical welder output)
    if (amplitude < 0.0f) amplitude = 0.0f;
    if (amplitude > 20.0f) amplitude = 20.0f;
    
    // Convert voltage to duty cycle percentage
    // Assuming 20V max output corresponds to 100% duty
    uint32_t duty = (uint32_t)((amplitude / 20.0f) * LEDC_DUTY_MAX);
    
    ESP_ERROR_CHECK(ledc_set_duty(LEDC_MODE, LEDC_CHANNEL, duty));
    ESP_ERROR_CHECK(ledc_update_duty(LEDC_MODE, LEDC_CHANNEL));
    
    ESP_LOGI(TAG, "PWM amplitude set to %.2f V (duty: %lu)", amplitude, duty);
}
