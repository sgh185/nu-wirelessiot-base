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

#define AI __attribute__((always_inline)) inline

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
#define DELAY_PERIOD 200
static simple_ble_char_t led_blink_state_char = {.uuid16 = 0x1089};
static bool led_blink_state = false;
AI static void _handle_blink_event(ble_evt_t const *p_ble_evt)
{
    /*
     * Check for LED characteristic 
     */
    if (!(simple_ble_is_char_event(p_ble_evt, &led_blink_state_char))) return;


    /*
     * Turn off the LED if "led_blink_state" is 
     * not set --- otherwise, the global can only
     * *pause* the LED in its state when unset 
     */
    if (!led_blink_state) nrf_gpio_pin_set(LED1);
    else nrf_gpio_pin_clear(LED1);


    /*
     * Debugging
     */ 
    printf("_handle_blink_event: led_blink_state is now %d\n", led_blink_state);

    
    return;
}


/*
 * State necessary to handle printing 
 */ 
#define BUF_SIZE 32
static simple_ble_char_t print_state_char = {.uuid16 = 0x1090};
static char print_buf[BUF_SIZE];
AI static void _handle_printing_event(ble_evt_t const *p_ble_evt)
{
    /*
     * Check for printing characteristic
     */
    if (!(simple_ble_is_char_event(p_ble_evt, &print_state_char))) return;


    /*
     * Print out the buffer 
     */ 
    printf("_handle_printing_event: current state: %s\n", print_buf); 


    /*
     * Reset the buffer --- it's possible the uploaded text
     * in a subsequent write has fewer characters than an
     * older write, but the older write's chars will stay
     * in the buffer and get printed
     */ 
    memset(
	print_buf,
	0,
	BUF_SIZE
    );


    return;
}


/*
 * State necessary to handle buttons 
 */ 
#define BUTTON_DEBUG 0
#define BUTTON_PRINT if (BUTTON_DEBUG) printf

#define BUTTON_ID_VAR(num) static uint8_t BUTTON##num##_id = num;  
#define BUTTON_IDS \
    BUTTON_ID_VAR(1) \
    BUTTON_ID_VAR(2) \
    BUTTON_ID_VAR(3) \
    BUTTON_ID_VAR(4)

BUTTON_IDS

#define BUTTON_HANDLER(num) if (!(nrf_gpio_pin_read(BUTTON##num))) { button_no = BUTTON##num##_id; }
#define HANDLE_ALL_BUTTONS \
    BUTTON_HANDLER(1) \
    BUTTON_HANDLER(2) \
    BUTTON_HANDLER(3) \
    BUTTON_HANDLER(4)


static simple_ble_char_t button_state_char = {.uuid16 = 0x1091};
static uint32_t button_no = 0;
AI static void _handle_button_presses(void)
{
    /*
     * Set "button_no" global to the button pressed
     */
    HANDLE_ALL_BUTTONS
    BUTTON_PRINT("_handle_button_presses: button_no is now %lu\n", button_no); 
    return;
}



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
    _handle_blink_event(p_ble_evt);


    /*
     * Handle printing characteristic
     */
    _handle_printing_event(p_ble_evt);


    return;
}

int main(void) {

    printf("Board started. Initializing BLE: \n");

    // Setup LED GPIO
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);
    nrf_gpio_pin_set(LED1);
    nrf_gpio_pin_set(LED2);
    nrf_gpio_pin_set(LED3);
    nrf_gpio_pin_set(LED4);
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON3, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON4, NRF_GPIO_PIN_PULLUP);


    // Setup BLE
    simple_ble_app = simple_ble_init(&ble_config);

    simple_ble_add_service(&led_service);


    /*
     * Add blinking
     */ 
    simple_ble_add_characteristic(
	1, 1, 0, 0,
	sizeof(led_blink_state), (uint8_t*)&led_blink_state,
	&led_service, &led_blink_state_char
    );


    /*
     * Add printing
     */ 
    simple_ble_add_characteristic(
	1, 1, 0, 0,
	sizeof(print_buf), (uint8_t*)&print_buf,
	&led_service, &print_state_char
    );


    /*
     * Add buttons
     */ 
    simple_ble_add_characteristic(
	1, 0, 1, 0,
	sizeof(button_no), (uint8_t*)&button_no,
	&led_service, &button_state_char
    );


    /*
     * Start advertising
     */ 
    simple_ble_adv_only_name();


    /*
     * Acrobatics --- Blink and notify if possible
     */ 
    while(1) 
    {
	/*
	 * Delay first
	 */ 
	nrf_delay_ms(DELAY_PERIOD);


	/*
	 * Toggle the LED IFF "led_blink_state" is set
	 */ 
	if (led_blink_state) nrf_gpio_pin_toggle(button_no);


	/*
	 * Write to "random_value" and notify based on the
	 * current global state
	 */ 
	_handle_button_presses();
	simple_ble_notify_char(&button_state_char);
    }

}

