// BLE Service example app
//
// Creates a BLE service and blinks an LED

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

static simple_ble_service_t led_service = {{
  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
}};



/*
 * State necessary to handle LED blinking
 */ 
#define DELAY_PERIOD 500
static simple_ble_char_t led_blink_state_char = {.uuid16 = 0x1089};
static bool led_blink_state = false;

static void _handle_blink(ble_evt_t const *p_ble_evt)
{
    /*
     * Check for LED characteristic
     */
    if (!(simple_ble_is_char_event(p_ble_evt, &led_blink_state_char))) return;
    printf("_handle_blink: current state: %d\n", led_blink_state); 


    /*
     * Turn off the LED if necessary
     */ 
    if (!led_blink_state)
    {
	printf("_handle_blink: turning off the lights ...\n");
	nrf_gpio_pin_clear(LED1);
	return;
    }


    /*
     * Otherwise, blink the lights
     */
    printf("_handle_blink: turning on the lights ...\n");
    while (1)
    {
	// nrf_delay_ms(DELAY_PERIOD);
	nrf_gpio_pin_toggle(LED1);
    }	


    return;
}


/*
 * State necessary to handle printing 
 */ 
static simple_ble_char_t led_print_state_char = {.uuid16 = 0x1090};
static char print_buf[32];

static void _handle_printing(ble_evt_t const *p_ble_evt)
{
    /*
     * Check for printing characteristic
     */
    if (!(simple_ble_is_char_event(p_ble_evt, &led_print_state_char))) return;


    /*
     * Print out the buffer 
     */ 
    printf("_handle_printing: current state: %s\n", print_buf); 
    return;
}


/*
 * State necessary to handle buttons
 */ 
static simple_ble_char_t led_button_state_char = {.uuid16 = 0x1091};
static uint8_t button_no;



/*******************************************************************************
 *   State for this application
 ******************************************************************************/
// Main application state
simple_ble_app_t* simple_ble_app;

void ble_evt_write(ble_evt_t const* p_ble_evt) 
{
    /*
     * Handle LED blinking characteristic
     */ 
    _handle_blink(p_ble_evt);


    /*
     * Handle printing characteristic
     */
    _handle_printing(p_ble_evt);


    return;
}

int main(void) {

  printf("Board started. Initializing BLE: \n");

  // Setup LED GPIO
  nrf_gpio_cfg_output(LED1);

  // Setup BLE
  simple_ble_app = simple_ble_init(&ble_config);

  simple_ble_add_service(&led_service);

  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(led_blink_state), (uint8_t*)&led_blink_state,
      &led_service, &led_blink_state_char);

  simple_ble_add_characteristic(1, 1, 0, 0,
      sizeof(print_buf), (uint8_t*)&print_buf,
      &led_service, &led_print_state_char);


  // Start Advertising
  simple_ble_adv_only_name();

  while(1) {
    power_manage();
  }
}

