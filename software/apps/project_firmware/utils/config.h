/*
 * Configurations for parking-sensor system
 */ 
#pragma once

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "app_scheduler.h"
#include "app_timer.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "nrf_power.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"

#include "simple_ble.h"
#include "nrf52840dk.h"

#define AI __attribute__((always_inline)) inline

