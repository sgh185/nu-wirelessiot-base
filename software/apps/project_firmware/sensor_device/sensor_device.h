/*
 * Settings for the sensor device
 */ 

#include "ad.h"


/*
 * ---------- Global state ----------
 */ 

/*
 * State for advertising/scanning phase
 */ 
static bool is_advertising_and_scanning = false;


/*
 * State for energy savings 
 */ 
static bool is_night_time = false;


/*
 * Sensor device configuration. NOTE --- We don't actually
 * utilize the device_id field. Why? We forgot about it, and
 * we don't have enough time to modify our implementation
 */ 
static simple_ble_config_t ble_config = {
  .platform_id       = 0x4E,    
  .device_id         = device_ID, /* Set at compile time */
  .adv_name          = DEVICE_NAME(DEVICE_TYPE, DEVICE_ID_STR), // device_name, /* Set at compile time */
  .adv_interval      = MSEC_TO_UNITS(500, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};


/*
 * ---------- Timer Info ----------
 */ 
#define START_UPDATE_INTERVAL 10000 // 300000 /* 5 minutes, subject to change */
// #define SCHED_QUEUE_SIZE 32
#define SCHED_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE
static uint32_t update_interval = START_UPDATE_INTERVAL;

