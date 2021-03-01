/*
 * Base device configuration and interface
 */
#pragma once

#include "config.h"


/*
 * Driver
 */  
#define initialize_device_state(type) _initialize_##type##_device_state()


/*
 * Device state
 */
#define DEVICE_ENTRIES 64 
static uint32_t device_state[DEVICE_ENTRIES];

#define DEVICE_ID 1
#define LAYER_ID 1
#define DEVICE_FLAGS 0
#define PARKING_ID 3

#define DEVICE_TYPE sensor
#define DEVICE_NAME(type, id) "_" type "_" id "_"

static const uint32_t device_ID = DEVICE_ID;
static const uint32_t layer_ID = LAYER_ID;
static const uint32_t flags = DEVICE_FLAGS;
static const uint32_t parking_ID = PARKING_ID;
static const char *device_name = DEVICE_NAME(DEVICE_TYPE, DEVICE_ID);

#define DEVICE_ENTRY 0 /* Index into @device_state, etc. */
#define LAYER_ENTRY 1
#define FLAGS_ENTRY 2
#define PARKING_ENTRY 3
#define DATA_START 4


/*
 * Device initialization 
 */ 
void _initialize_sensor_device_state(void);

void _initialize_relayer_device_state(void);

void _initialize_central_device_state(void);
