#include "dac_control.h"
#include "driver/dac_continuous.h"
#include "esp_log.h"
#include "driver/gpio.h"

static const char *TAG = "DAC_Control";

// DAC configuration for voltage output
#define DAC_CHANNEL_1 0              // GPIO17 for ESP32-P4
#define DAC_CHANNEL_2 1              // GPIO18 for ESP32-P4
#define DAC_RESOLUTION 8             // 8-bit DAC (0-255)
#define DAC_MAX_VOLTAGE 15.0f        // Maximum 15V output

static dac_continuous_handle_t dac_handle = NULL;

void dac_init(void)
{
    dac_continuous_config_t config = {
        .chan_mask = DAC_CHANNEL_MASK_CH0,  // Use DAC channel 0 (GPIO17)
        .desc_num = 4,
        .buf_size = 1024,
        .freq_hz = 1000,
        .offset = 0,
        .clk_src = DAC_CLK_SRC_DEFAULT,
        .chan_mode = DAC_CHANNEL_MODE_SIMUL,
    };
    
    ESP_ERROR_CHECK(dac_continuous_new_channels(&config, &dac_handle));
    ESP_ERROR_CHECK(dac_continuous_enable(dac_handle));
    ESP_LOGI(TAG, "DAC initialized on GPIO17 (Channel 0)");
}

void dac_set_voltage(float voltage)
{
    // Clamp voltage to valid range
    if (voltage < 0.0f) voltage = 0.0f;
    if (voltage > DAC_MAX_VOLTAGE) voltage = DAC_MAX_VOLTAGE;
    
    // Convert voltage to 8-bit DAC value
    // DAC output is 0-3.3V for 0-255
    // For 0-15V range, use external amplifier (3.3V * gain = 15V)
    // Gain = 15V / 3.3V = 4.55
    uint8_t dac_value = (uint8_t)((voltage / 3.3f) * 255.0f);
    
    uint8_t data[1] = {dac_value};
    ESP_ERROR_CHECK(dac_continuous_write(dac_handle, data, 1, NULL, -1));
    ESP_LOGI(TAG, "DAC output set to %.2f V (DAC value: %d)", voltage, dac_value);
}
