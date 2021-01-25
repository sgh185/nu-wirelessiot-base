#include <stdbool.h>
#include <stdint.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin definitions
#include "nrf52840dk.h"

/*
 * Helper macros
 */ 
#define AI __attribute__((always_inline))
#define DELAY_TIME 500


/*
 * State
 */ 
bool display_currently_on = false;

 
/*
 * Lights drivers
 */ 
AI void __toggle_and_delay(uint32_t pin_number)
{
    nrf_gpio_pin_toggle(pin_number);
    nrf_delay_ms(DELAY_TIME);


    return;
}


AI void turn_on_lights_display(void)
{
    /*
     * TOP 
     *
     * Turn on/off the LEDs in the following order:
     * 1) ON, LED 1
     * 2) ON, LED 2
     * 3) ON, LED 3
     * 4) ON, LED 4
     * 5) OFF, LED 1
     * 6) OFF, LED 2
     * 7) OFF, LED 3
     * 8) OFF, LED 4
     *
     * NOTE --- Assume some caller up in the call stack 
     * initializes AND the lights are off prior 
     */ 
    
    /* Steps 1-4 */
    __toggle_and_delay(LED1);
    __toggle_and_delay(LED2);
    __toggle_and_delay(LED3);
    __toggle_and_delay(LED4);

    
    /* Steps 5-8 */
    __toggle_and_delay(LED1);
    __toggle_and_delay(LED2);
    __toggle_and_delay(LED3);
    __toggle_and_delay(LED4);

    
    /*
     * Mark global state
     */ 
    display_currently_on = true;
   

    return;
}


AI void turn_off_lights_display(void)
{
    /*
     * Turns all lights off
     */ 
    nrf_gpio_pin_clear(LED1);
    nrf_gpio_pin_clear(LED2);
    nrf_gpio_pin_clear(LED3);
    nrf_gpio_pin_clear(LED4);


    /*
     * Mark global state
     */ 
    display_currently_on = false;


    return;
}


/*
 * Button handlers
 */ 
AI void handle_button_1(void)
{
    /*
     * Turn on lights if not on already 
     */ 
    if (!display_currently_on) turn_on_lights_display();
    return;
}


AI void handle_button_2(void)
{
    /*
     * Turn off lights if they're already on 
     */ 
    if (display_currently_on) turn_off_lights_display();
    return;
}


int main(void) 
{
    /*
     * LED --- Initialization
     */
    nrf_gpio_cfg_output(LED1);
    nrf_gpio_cfg_output(LED2);
    nrf_gpio_cfg_output(LED3);
    nrf_gpio_cfg_output(LED4);


    /*
     * Button --- Initialization, only using buttons 1 and 2
     */  
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    nrf_gpio_cfg_input(BUTTON2, NRF_GPIO_PIN_PULLUP);


    /*
     * Execute indefinitely, poll 
     */  
    while (1)
    {
        /*
         * Handle button 1 --- "ON trigger"
         */ 
        if (nrf_gpio_pin_read(BUTTON1)) handle_button_1();


        /*
         * Handle button 2 --- "OFF trigger"
         */ 
        if (nrf_gpio_pin_read(BUTTON2)) handle_button_2();
    }


    return 0;
}

