/*
 * Profiling methods for ParkingBLE system
 */

#include "config.h"

/*
 * Profiler state
 */  
extern uint32_t start, stop, elapsed;
extern uint32_t ih_start, ih_elapsed ;
extern uint32_t evt_start, evt_elapsed;
extern uint32_t adv_start, adv_elapsed;



/*
 * Profiler methods 
 */  
void resume_profiling(void);

void pause_profiling(void);

void start_profiling(void);

void stop_profiling(void);

void print_profile(void);
