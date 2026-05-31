#include <stdio.h>
#include <string.h>
#include "freertos/FreeRTOS.h"
#include "freertos/task.h"
#include "esp_log.h"
#include "esp_system.h"
#include "nvs_flash.h"
#include "esp_bt.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatt_defs.h"
#include "esp_attr.h"
#include "esp_gatts_api.h"

#include "adc_control.h"
#include "dac_control.h"
#include "pwm_driver.h"
#include "ble_server.h"
#include "ui_display.h"

static const char *TAG = "WelderControl";

// Global control variables
typedef struct {
    float power_setpoint;      // 0-15V
    float pulse_frequency;     // 0.5-10 Hz
    float pulse_amplitude;     // 0-20% above setpoint
    float potentiometer_value; // Raw ADC value
} welder_control_t;

static welder_control_t welder_state = {
    .power_setpoint = 5.0f,
    .pulse_frequency = 1.0f,
    .pulse_amplitude = 0.0f,
    .potentiometer_value = 0.0f
};

/**
 * Task to read potentiometer and update power output
 */
void potentiometer_read_task(void *pvParameter)
{
    while (1) {
        welder_state.potentiometer_value = adc_read_potentiometer();
        // Convert ADC reading to voltage (0-15V)
        welder_state.power_setpoint = (welder_state.potentiometer_value / 4095.0f) * 15.0f;
        dac_set_voltage(welder_state.power_setpoint);
        vTaskDelay(pdMS_TO_TICKS(100)); // 100ms update rate
    }
}

/**
 * Task to generate pulse waveform
 */
void pulse_generation_task(void *pvParameter)
{
    while (1) {
        if (welder_state.pulse_frequency > 0) {
            float period_ms = 1000.0f / welder_state.pulse_frequency;
            float on_time_ms = period_ms / 2.0f; // 50% duty cycle
            
            // Calculate pulse voltage
            float pulse_voltage = welder_state.power_setpoint + 
                                 (welder_state.power_setpoint * welder_state.pulse_amplitude / 100.0f);
            
            pwm_set_frequency(welder_state.pulse_frequency);
            pwm_set_amplitude(pulse_voltage);
        }
        vTaskDelay(pdMS_TO_TICKS(50));
    }
}

/**
 * Task for BLE communication
 */
void ble_communication_task(void *pvParameter)
{
    while (1) {
        // BLE event handling is done in callbacks
        vTaskDelay(pdMS_TO_TICKS(100));
    }
}

void app_main(void)
{
    ESP_LOGI(TAG, "Starting Welder Pulse Control System");
    
    // Initialize NVS
    esp_err_t ret = nvs_flash_init();
    if (ret == ESP_ERR_NVS_NO_FREE_PAGES || ret == ESP_ERR_NVS_NEW_VERSION_FOUND) {
        ESP_ERROR_CHECK(nvs_flash_erase());
        ret = nvs_flash_init();
    }
    ESP_ERROR_CHECK(ret);
    
    // Initialize Bluetooth controller
    esp_bt_controller_config_t bt_cfg = BT_CONTROLLER_INIT_CONFIG_DEFAULT();
    ESP_ERROR_CHECK(esp_bt_controller_init(&bt_cfg));
    ESP_ERROR_CHECK(esp_bt_controller_enable(ESP_BT_MODE_BLE));
    
    // Initialize Bluedroid stack
    ESP_ERROR_CHECK(esp_bluedroid_init());
    ESP_ERROR_CHECK(esp_bluedroid_enable());
    
    // Initialize peripherals
    ESP_LOGI(TAG, "Initializing ADC for potentiometer");
    adc_init();
    
    ESP_LOGI(TAG, "Initializing DAC for voltage output");
    dac_init();
    
    ESP_LOGI(TAG, "Initializing PWM for pulse control");
    pwm_init();
    
    ESP_LOGI(TAG, "Initializing BLE server");
    ble_server_init();
    
    ESP_LOGI(TAG, "Initializing LCD display");
    ui_display_init();
    
    // Create tasks
    xTaskCreate(potentiometer_read_task, "pot_read", 2048, NULL, 5, NULL);
    xTaskCreate(pulse_generation_task, "pulse_gen", 2048, NULL, 5, NULL);
    xTaskCreate(ble_communication_task, "ble_comm", 2048, NULL, 4, NULL);
    
    ESP_LOGI(TAG, "Welder control system initialized successfully");
}
