/*
 * Settings for the sensor device
 */ 

#include "base_device.h"


/*
 * ---------- Global state ----------
 */ 

/*
 * State for advertising/scanning phase
 */ 
____ the_ad = {};

static bool is_advertising_and_scanning = false;


/*
 * State for parking spot
 */ 
static bool curent_status = false;


/*
 * State for energy savings 
 */ 
static bool is_night_time = false;




/*
 * Sensor device configuration
 */ 
static simple_ble_config_t ble_config = {
  .platform_id       = 0x4E,    
  .device_id         = device_ID, /* Set at compile time */
  .adv_name          = device_name /* Set at compile time */
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};



