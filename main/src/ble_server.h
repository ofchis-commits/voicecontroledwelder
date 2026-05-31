#ifndef BLE_SERVER_H
#define BLE_SERVER_H

/**
 * Initialize BLE GATT server for welder control
 */
void ble_server_init(void);

/**
 * Update BLE characteristics with current welder state
 */
void ble_update_characteristics(float power, float frequency, float amplitude);

#endif // BLE_SERVER_H
