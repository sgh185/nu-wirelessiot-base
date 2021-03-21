/*
 * Profiler implementation
 */ 

#include "profile.h"


/*
 * Profiler state
 */  
uint32_t start = 0, stop = 0, elapsed = 0;
uint32_t ih_start = 0, ih_elapsed = 0;
uint32_t evt_start = 0, evt_elapsed = 0;
uint32_t adv_start = 0, adv_elapsed = 0;


/*
 * Profiler methods 
 */  
void resume_profiling(void)
{
    DWT->CTRL |= 0x1;
    return;
}


void pause_profiling(void)
{
    DWT->CTRL &= 0x0;
    return;
}


void start_profiling(void)
{
    /*
     * Enable DWT
     */
    CoreDebug->DEMCR |= 0x01000000;


    /*
     * Set up cycle counter
     */ 
    DWT->CYCCNT = 0;
    DWT->CTRL |= 0x1;


    /*
     * Start measurements
     */ 
    start = DWT->CYCCNT;


    return;
}


void stop_profiling(void)
{
    stop = DWT->CYCCNT;
    DWT->CTRL &= 0x0;
    elapsed = stop - start;
    return;
}


void print_profile(void)
{
    printf("cycles: %lu\n", elapsed);
    printf("ih: %lu\n", ih_elapsed);
    printf("evt: %lu\n", evt_elapsed);
    printf("adv: %lu\n", adv_elapsed);
    ih_elapsed = 0;
    evt_elapsed = 0;
    adv_elapsed = 0;
    return;
}

