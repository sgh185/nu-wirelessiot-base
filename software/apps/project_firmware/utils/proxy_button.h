/*
 * Interface for button-based proxy to 
 * mimic magnetometor functionality
 */
#include "config.h"
#include "base_device.h"
#include "base_advertiser.h"


/*
 * Helpers to organize the button handler 
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

#define BUTTON_HANDLER(num) case BUTTON##num : { device_state[DATA_START] = BUTTON##num##_id; nrf_gpio_pin_toggle(LED##num); break; }
#define HANDLE_ALL_BUTTONS \
    BUTTON_HANDLER(1) \
    BUTTON_HANDLER(2) \
    BUTTON_HANDLER(3) \
    BUTTON_HANDLER(4)


/*
 * Button and interrupt handlers
 */  
void _button_handler(
    nrfx_gpiote_pin_t pin, 
    nrf_gpiote_polarity_t action
); 

static void _gpio_in_with_interrupt(
    nrfx_gpiote_pin_t pin, 
    nrf_gpio_pin_pull_t pull, 
    nrfx_gpiote_evt_handler_t handler
);

static void initialize_proxy_button(void);
