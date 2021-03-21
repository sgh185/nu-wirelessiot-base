/*
 * Settings for the sensor device
 */ 

#include "ad.h"
#include "icm20948.h"
#include "profile.h"


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
#if SIMULATE
/* 15, 30 seconds */
static const uint32_t intervals[2] = { 15000 /* normal speed */, 30000 /* throttle down */ } ;
#else
/* 5, 30 mins */
static const uint32_t intervals[2] = { 300000 /* normal speed */, 1800000 /* throttle down */ } ;
#endif



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
// #define SCHED_QUEUE_SIZE 32
#define SCHED_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE
static uint32_t update_interval = intervals[0] ;

