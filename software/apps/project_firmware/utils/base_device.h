/*
 * Base device configuration and interface
 */
#pragma once

#include "config.h"


/*
 * Device state
 */
#define DEVICE_ID 1
#define DEVICE_ID_STR "1"
#define LAYER_ID 1
#define DEVICE_FLAGS 0
#define PARKING_ID 3

#define DEVICE_TYPE "sensor"
#define DEVICE_NAME(type, id) "_" type "_" id "_"

static const uint8_t device_ID = DEVICE_ID;
static const uint8_t layer_ID = LAYER_ID;
static const uint8_t flags = DEVICE_FLAGS;
static const uint8_t parking_ID = PARKING_ID;
