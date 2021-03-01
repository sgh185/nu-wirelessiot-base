/*
 * Configurations for parking-sensor system
 */ 
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "simple_ble.h"
#include "nrf52840dk.h"

#define AI __attribute__((always_inline)) inline

