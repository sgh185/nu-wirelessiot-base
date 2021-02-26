/*
 * Code for sensor/edge nodes of the system
 */ 

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrfx_gpiote.h"
#include "simple_ble.h"

#include "nrf52840dk.h"

#include "config.h"

#define AI __attribute__((always_inline)) inline

/*
 * Sensor node state
 */
static const uint32_t device_ID = DEVICE_ID;
static const uint32_t layer_ID = LAYER_ID;
static const uint32_t flags = DEVICE_FLAGS;
static const uint32_t parking_ID = PARKING_ID;


// Intervals for advertising and connections
static simple_ble_config_t ble_config = {
  // c0:98:e5:4e:xx:xx
  .platform_id       = 0x4E,    // used as 4th octect in device BLE address
  .device_id         = device_ID,
  .adv_name          = "Nordic_Blinky", // used in advertisements if there is room
  .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS),
  .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS),
  .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS),
};

static simple_ble_service_t button_service = {{
  .uuid128 = {0x70,0x6C,0x98,0x41,0xCE,0x43,0x14,0xA9,
              0xB5,0x4D,0x22,0x2B,0x88,0x10,0xE6,0x32}
}};

#define NUM_ENTRIES 64
#define DEVICE_ENTRY 0
#define LAYER_ENTRY 1
#define FLAGS_ENTRY 2
#define PARKING_ENTRY 3
#define DATA_START 4

static simple_ble_char_t button_state_char = {.uuid16 = 0x1091};
static uint32_t device_state[64];


/*
 * Sensor-node specific initialization
 */
void initialize_sensor_device_state(void)
{
    /*
     * Zero out the device state
     */
    memset(
	device_state,
	0,
	sizeof(uint32_t) * NUM_ENTRIES
    );


    /*
     * Initialize the device state according to the
     * data/state encoding for each sensor device
     */ 
    device_state[DEVICE_ENTRY] = device_ID;
    device_state[LAYER_ENTRY] = layer_ID;
    device_state[FLAGS_ENTRY] = flags;
    device_state[PARKING_ENTRY] = parking_ID;


    return;
}


/*
 * State necessary to handle magnetometer data
 *
 * NOTE --- This is abstracted as button presses for now to
 * enable simplicity in debugging and building the rest of 
 * the system in BLE. 
 */ 
#define BUTTON_DEBUG 1
#define BUTTON_PRINT if (BUTTON_DEBUG) printf

#define BUTTON_ID_VAR(num) static uint8_t BUTTON##num##_id = num;  
#define BUTTON_IDS \
    BUTTON_ID_VAR(1) \
    BUTTON_ID_VAR(2) \
    BUTTON_ID_VAR(3) \
    BUTTON_ID_VAR(4)

BUTTON_IDS

// #define BUTTON_HANDLER(num) if (!(nrf_gpio_pin_read(BUTTON##num))) { button_no = BUTTON##num##_id; }
#define BUTTON_HANDLER(num) case BUTTON##num : { device_state[DATA_START] = BUTTON##num##_id; nrf_gpio_pin_toggle(LED##num); break; }
#define HANDLE_ALL_BUTTONS \
    BUTTON_HANDLER(1) \
    BUTTON_HANDLER(2) \
    BUTTON_HANDLER(3) \
    BUTTON_HANDLER(4)



/*
 * Button and interrupt handlers
 */  
void button_handler(
    nrfx_gpiote_pin_t pin, 
    nrf_gpiote_polarity_t action
) 
{
    /*
     * Set button state (will be replaced soon with
     * magnetometor state), and set LED.
     */ 
    switch(pin) { HANDLE_ALL_BUTTONS }

    
    /*
     * Perform BLE notifications
     */
    BUTTON_PRINT("_handle_button_presses: button_no is now %lu\n", device_state[DATA_START]);
    simple_ble_notify_char(&button_state_char);
     
    
    return;
}


static void gpio_in_with_interrupt(
    nrfx_gpiote_pin_t pin, 
    nrf_gpio_pin_pull_t pull, 
    nrfx_gpiote_evt_handler_t handler
)
{
    // Configure I/O pin
    // HITOLO: Sense high-to-low transition
    // Alternatives: LOTOHI or TOGGLE (for either)
    nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = pull;
    ret_code_t err_code = nrfx_gpiote_in_init(pin, &in_config, handler);
    APP_ERROR_CHECK(err_code);

    // Enable the interrupts
    nrfx_gpiote_in_event_enable(pin, true);


    return;
}



/*******************************************************************************
 *   State for this application
 ******************************************************************************/

/*
 * Main application state
 */ 
simple_ble_app_t* simple_ble_app;


void ble_evt_write(ble_evt_t const* p_ble_evt) {}


int main(void) 
{
    printf("Board started. Initializing BLE: \n");

    /*
     * Seup LEDs, buttons 
     */ 
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


    /*
     * Intterupt setup
     */ 
    ret_code_t err_code = nrfx_gpiote_init();
    APP_ERROR_CHECK(err_code);

    gpio_in_with_interrupt(BUTTON1, NRF_GPIO_PIN_PULLUP, button_handler);
    gpio_in_with_interrupt(BUTTON2, NRF_GPIO_PIN_PULLUP, button_handler);
    gpio_in_with_interrupt(BUTTON3, NRF_GPIO_PIN_PULLUP, button_handler);
    gpio_in_with_interrupt(BUTTON4, NRF_GPIO_PIN_PULLUP, button_handler);
    
    
    /*
     * Handle BLE
     */  
    simple_ble_app = simple_ble_init(&ble_config);

    simple_ble_add_service(&button_service);


    /*
     * Set up sensor device state
     */ 
    initialize_sensor_device_state();


    /*
     * Add buttons
     */ 
    simple_ble_add_characteristic(
	1, 0, 1, 0,
	sizeof(device_state), (uint8_t*)&device_state,
	&button_service, &button_state_char
    );


    /*
     * Start advertising
     */ 
    simple_ble_adv_only_name();


    /*
     * Do nothing ... 
     */ 
    while(1) power_manage();	
}


