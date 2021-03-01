/*
 * Implementation for button-based proxy to 
 * mimic magnetometor functionality
 */
#include "proxy_button.h"


/*
 * Button and interrupt handlers
 */  
void _button_handler(
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
    BUTTON_PRINT(
	"_button_handler: device_state[%d] = %lu\n", 
	DATA_START, 
	device_state[DATA_START]
    );
   
    
    return;
}


static void _gpio_in_with_interrupt(
    nrfx_gpiote_pin_t pin, 
    nrf_gpio_pin_pull_t pull, 
    nrfx_gpiote_evt_handler_t handler
)
{
    /*
     * Configure I/O pin
     * HITOLO: Sense high-to-low transition
     * Alternatives: LOTOHI or TOGGLE (for either)
     */ 
    nrfx_gpiote_in_config_t in_config = NRFX_GPIOTE_CONFIG_IN_SENSE_HITOLO(true);
    in_config.pull = pull;
    ret_code_t err_code = nrfx_gpiote_in_init(pin, &in_config, handler);
    APP_ERROR_CHECK(err_code);


    /*
     * Enable the interrupts 
     */ 
    nrfx_gpiote_in_event_enable(pin, true);


    return;
}


static void initialize_proxy_button(void)
{
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

    _gpio_in_with_interrupt(BUTTON1, NRF_GPIO_PIN_PULLUP, _button_handler);
    _gpio_in_with_interrupt(BUTTON2, NRF_GPIO_PIN_PULLUP, _button_handler);
    _gpio_in_with_interrupt(BUTTON3, NRF_GPIO_PIN_PULLUP, _button_handler);
    _gpio_in_with_interrupt(BUTTON4, NRF_GPIO_PIN_PULLUP, _button_handler);


    return; 
}
