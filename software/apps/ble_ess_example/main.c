// BLE Service example app
//
// Creates a BLE environmental sensing service

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = 0xAABB,
  .adv_name          = "CS397/497", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t environmental_sensing_service = {{
  .uuid128 = {0xFB,0x34,0x9B,0x5F,0x80,0x00,0x00,0x80,
              0x00,0x10,0x00,0x00,0x1A,0x18,0x00,0x00}
}};


static simple_ble_char_t temperature_char = {.uuid16 = 0x2A6E};
static int16_t the_temp = 0xFC7C; // -9.00 C


static simple_ble_char_t elevation_char = {.uuid16 = 0x2A6C};
static int8_t the_elevation[3] = {0x08, 0x39, 0x00}; // 146 m


static simple_ble_char_t pressure_char = {.uuid16 = 0x2A6D};
static uint32_t the_pressure = 1024044; // ~102404.39 pascals

static simple_ble_char_t humidity_char = {.uuid16 = 0x2A6F};
static uint16_t the_humidity = 6200; // 62%


static simple_ble_char_t wind_chill_char = {.uuid16 = 0x2A79};
static int8_t the_wind_chill = 0xF2; // -14 C


/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) {
  printf("Got write to a characteristic!\n");
}

int main(void) {

    printf("Board started. Initializing BLE: \n");

    // Setup BLE
    simple_ble_app = simple_ble_init(&ble_config);

    simple_ble_add_service(&environmental_sensing_service);

    // Start Advertising
    simple_ble_adv_only_name();


    /*
     * Temperature 
     */ 
    simple_ble_add_characteristic(
	1, 0, 0, 0,
	sizeof(the_temp), (uint8_t*)&the_temp,
	&environmental_sensing_service, &temperature_char
    );


    /*
     * Elevation 
     */ 
    simple_ble_add_characteristic(
	1, 0, 0, 0,
	sizeof(the_elevation), (uint8_t*)&the_elevation,
	&environmental_sensing_service, &elevation_char
    );


    /*
     * Pressure 
     */ 
    simple_ble_add_characteristic(
	1, 0, 0, 0,
	sizeof(the_pressure), (uint8_t*)&the_pressure,
	&environmental_sensing_service, &pressure_char
    );


    /*
     * Humidity 
     */ 
    simple_ble_add_characteristic(
	1, 0, 0, 0,
	sizeof(the_humidity), (uint8_t*)&the_humidity,
	&environmental_sensing_service, &humidity_char
    );


    /*
     * Wind chill 
     */ 
    simple_ble_add_characteristic(
	1, 0, 0, 0,
	sizeof(the_wind_chill), (uint8_t*)&the_wind_chill,
	&environmental_sensing_service, &wind_chill_char
    );


    while(1) {
	power_manage();
    }
}

