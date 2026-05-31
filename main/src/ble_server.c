#include "ble_server.h"
#include "esp_log.h"
#include "esp_gap_ble_api.h"
#include "esp_gattc_api.h"
#include "esp_gatts_api.h"
#include "esp_gatt_defs.h"
#include "esp_attr.h"
#include "esp_bt_defs.h"
#include "nvs_flash.h"

static const char *TAG = "BLE_Server";

#define GATTS_TAG "GATTS_DEMO"
#define SVC_INST_ID 0
#define GATTS_CHAR_UUID_TEST_A 0x00FF
#define GATTS_CHAR_UUID_TEST_B 0xFF01
#define GATTS_CHAR_UUID_TEST_C 0xFF02
#define GATTS_NUM_HANDLE 4
#define PREPARE_BUF_MAX_SIZE 9
#define CHAR_DECLARATION_SIZE (sizeof(uint8_t))

static uint8_t char1_str[] = {0x11, 0x22, 0x33};
static esp_gatt_char_prop_t a_property = 0;
static esp_gatt_char_prop_t b_property = 0;
static esp_gatt_char_prop_t c_property = 0;

static esp_attr_value_t gatts_demo_char1_val = 
{
    .attr_max_len = PREPARE_BUF_MAX_SIZE,
    .attr_len     = sizeof(char1_str),
    .attr_value   = char1_str,
};

static uint8_t adv_service_uuid128[32] = {
    /* LSB <--------------------------------------------------------------------------------> MSB */
    0xfb, 0x34, 0x9b, 0x5f, 0x80, 0x00, 0x00, 0x80, 0x00, 0x10, 0x00, 0x00, 0xFF, 0x00, 0x00, 0x00,
};

static esp_ble_adv_data_t adv_data = {
    .set_scan_rsp = false,
    .include_name = true,
    .include_txpower = true,
    .min_interval = 0x0006,
    .max_interval = 0x0010,
    .appearance = 0x00,
    .manufacturer_len = 0,
    .p_manufacturer_data =  NULL,
    .service_data_len = 0,
    .p_service_data = NULL,
    .service_uuid_len = sizeof(adv_service_uuid128),
    .p_service_uuid = adv_service_uuid128,
    .flag = (ESP_BLE_ADV_FLAG_GEN_DISC | ESP_BLE_ADV_FLAG_BREDR_NOT_SPT),
};

static esp_ble_adv_params_t adv_params = {
    .adv_int_min        = 0x20,
    .adv_int_max        = 0x40,
    .adv_type           = ADV_TYPE_IND,
    .own_addr_type      = BLE_ADDR_TYPE_PUBLIC,
    .channel_map        = ADV_CHNL_ALL,
    .adv_filter_policy = ADV_FILTER_ALLOW_SCAN_ANY_CON_ANY,
};

void ble_gap_event_handler(esp_gap_ble_cb_event_t event, esp_ble_gap_cb_param_t *param)
{
    switch (event) {
        case ESP_GAP_BLE_ADV_DATA_SET_COMPLETE_EVT:
            esp_ble_gap_start_advertising(&adv_params);
            break;
        case ESP_GAP_BLE_ADV_START_COMPLETE_EVT:
            if (param->adv_start_cmpl.status == ESP_BT_STATUS_SUCCESS) {
                ESP_LOGI(TAG, "ADV start success");
            }
            break;
        default:
            break;
    }
}

void ble_gatts_event_handler(esp_gatts_cb_event_t event, esp_gatt_if_t gatts_if,
                             esp_ble_gatts_cb_param_t *param)
{
    switch (event) {
        case ESP_GATTS_REG_EVT:
            ESP_LOGI(TAG, "REG_EVT");
            break;
        case ESP_GATTS_READ_EVT:
            ESP_LOGI(TAG, "READ_EVT");
            break;
        case ESP_GATTS_WRITE_EVT:
            ESP_LOGI(TAG, "WRITE_EVT, write value:");
            break;
        case ESP_GATTS_EXEC_WRITE_EVT:
            ESP_LOGI(TAG, "EXEC_WRITE_EVT");
            break;
        case ESP_GATTS_MTU_EVT:
            ESP_LOGI(TAG, "MTU_EVT, MTU %d", param->mtu.mtu);
            break;
        case ESP_GATTS_CONF_EVT:
            ESP_LOGI(TAG, "CONF_EVT");
            break;
        case ESP_GATTS_START_EVT:
            ESP_LOGI(TAG, "START_EVT");
            break;
        case ESP_GATTS_STOP_EVT:
            ESP_LOGI(TAG, "STOP_EVT");
            break;
        case ESP_GATTS_CONNECT_EVT:
            ESP_LOGI(TAG, "CONNECT_EVT");
            break;
        case ESP_GATTS_DISCONNECT_EVT:
            ESP_LOGI(TAG, "DISCONNECT_EVT, reason = %x", param->disconnect.reason);
            break;
        default:
            break;
    }
}

void ble_server_init(void)
{
    ESP_ERROR_CHECK(esp_ble_gap_register_callback(ble_gap_event_handler));
    ESP_ERROR_CHECK(esp_ble_gatts_register_callback(ble_gatts_event_handler));
    
    ESP_ERROR_CHECK(esp_ble_gap_set_device_name("Welder_Control"));
    ESP_ERROR_CHECK(esp_ble_gap_config_adv_data(&adv_data));
    
    ESP_LOGI(TAG, "BLE GATT Server initialized");
}

void ble_update_characteristics(float power, float frequency, float amplitude)
{
    // This would be called to update BLE characteristics with current values
    ESP_LOGI(TAG, "BLE Update - Power: %.2fV, Freq: %.2fHz, Amp: %.1f%%",
             power, frequency, amplitude);
}
