#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"

// Pin definitions
#include "nrf52840dk.h"


/*
 * Helper macros
 */ 
#define AI __attribute__((always_inline)) inline
#define DELAY_TIME 500

 
/*
 * Lights drivers
 */ 
static AI void __toggle_and_delay(uint32_t pin_number)
{
    nrf_gpio_pin_toggle(pin_number);
    nrf_delay_ms(DELAY_TIME);


    return;
}


static AI void do_lights_display(void)
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
     * NOTE --- Assume some caller up in the call stack initializes 
     */ 
    
    /* Steps 1-4 */
    printf("\tsteps 1-4\n");
    __toggle_and_delay(LED1);
    __toggle_and_delay(LED2);
    __toggle_and_delay(LED3);
    __toggle_and_delay(LED4);

    
    /* Steps 5-8 */
    printf("\tsteps 5-8\n");
    __toggle_and_delay(LED1);
    __toggle_and_delay(LED2);
    __toggle_and_delay(LED3);
    __toggle_and_delay(LED4);


    return;
}


/*
 * Button handlers
 */ 
static AI void handle_button(uint32_t invocation_no)
{
    /*
     * Output debugging and turn on the display
     */ 
    printf("Invocation %lu!\n", invocation_no);
    do_lights_display();
    printf("Done!\n");


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
    nrf_gpio_pin_set(LED1);
    nrf_gpio_pin_set(LED2);
    nrf_gpio_pin_set(LED3);
    nrf_gpio_pin_set(LED4);

    
    /*
     * Button --- Initialization, only using button 1
     */  
    nrf_gpio_cfg_input(BUTTON1, NRF_GPIO_PIN_PULLUP);
    
    
    printf("Done initialization!\n");


    /*
     * Execute indefinitely, poll 
     */  
    uint32_t invocation = 0;
    while (1)
    {
        /*
         * Handle button 1 --- "ON trigger"
         */ 
        if (!nrf_gpio_pin_read(BUTTON1)) 
	{
	    handle_button(invocation);
	    invocation++;
	}
    }


    return 0;
}

