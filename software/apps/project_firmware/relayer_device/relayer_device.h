/*
 * Interface and settings for relayer devices
 */ 
#pragma once

#include "ad.h"
#include "ring_buffer.h"



/*
 * ---------- Global state ----------
 */ 

#define MIN_LEVEL 4
static bool waiting_for_ack = false;
static uint8_t ack_ref_count = 0;
static bool throttle_down = false;


/*
 * State for relayed data (upwards)
 *
 * NOTE --- Obviously, these aren't properly 
 * designed software caches. For example, the 
 * eviction policy is "kick into oblivion." 
 */ 
#define BUF_VAL_MAX 255
ring_buffer parking_IDs_cache = {
    .buf = { BUF_VAL_MAX },
    .next_index_to_fill = 0
};

ring_buffer corresponding_seq_no_cache = {
    .buf = { BUF_VAL_MAX },
    .next_index_to_fill = 0
};


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
 * ---------- Methods ----------
 */ 

/*
 * Cache handlers
 */ 
static bool is_ad_already_handled(
    uint8_t sender_parking_ID,
    uint8_t sender_seq_no
);

static void place_ad_into_cache(
    uint8_t sender_parking_ID,
    uint8_t sender_seq_no
); 

static void print_cache(void);



/*
 * Filtering methods
 */ 
static bool should_handle_ad(
    uint8_t *recv_ad,
    uint16_t recv_ad_len
);

static void handle_ad_for_relaying(uint8_t *recv_ad);


