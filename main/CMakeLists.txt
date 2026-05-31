idf_component_register(
    SRCS "src/main.c" 
         "src/adc_control.c"
         "src/pwm_driver.c"
         "src/dac_control.c"
         "src/ble_server.c"
         "src/ui_display.c"
    INCLUDE_DIRS "src"
    REQUIRES esp_system
             esp_common
             freertos
             esp_adc
             esp_timer
             nvs_flash
             bt
             driver
)
