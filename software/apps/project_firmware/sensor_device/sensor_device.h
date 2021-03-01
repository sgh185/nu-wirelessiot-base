/*
 * Settings for the sensor device
 */ 

#include "base_device.h"

static simple_ble_config_t ble_config = {
  .platform_id       = 0x4E,    
  .device_id         = device_ID,
  .adv_name          = device_name 
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};
