/*
 * Proper utilities necessary for this system
 */

#pragma once

#include "config.h"

void scanning_stop(void); 

bool synthesize_magnetometer_data(void);

void print_buffer(
    uint8_t *adv_buf, 
    uint16_t adv_len
);
