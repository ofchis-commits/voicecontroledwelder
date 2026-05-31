#include "adc_control.h"
#include "esp_adc/adc_oneshot.h"
#include "esp_log.h"

static const char *TAG = "ADC_Control";

// ADC configuration
#define ADC_CHANNEL ADC_CHANNEL_7  // GPIO35 for ESP32-P4
#define ADC_UNIT ADC_UNIT_1
#define ADC_ATTEN ADC_ATTEN_DB_12
#define ADC_BITWIDTH ADC_BITWIDTH_12

static adc_oneshot_unit_handle_t adc_handle = NULL;

void adc_init(void)
{
    adc_oneshot_unit_init_cfg_t init_config = {
        .unit_id = ADC_UNIT,
        .ulp_mode = ADC_ULP_MODE_DISABLE,
    };
    
    ESP_ERROR_CHECK(adc_oneshot_new_unit(&init_config, &adc_handle));
    
    adc_oneshot_chan_cfg_t config = {
        .bitwidth = ADC_BITWIDTH,
        .atten = ADC_ATTEN,
    };
    
    ESP_ERROR_CHECK(adc_oneshot_config_channel(adc_handle, ADC_CHANNEL, &config));
    ESP_LOGI(TAG, "ADC initialized on unit %d, channel %d", ADC_UNIT, ADC_CHANNEL);
}

float adc_read_potentiometer(void)
{
    int adc_reading = 0;
    ESP_ERROR_CHECK(adc_oneshot_read(adc_handle, ADC_CHANNEL, &adc_reading));
    return (float)adc_reading;
}
