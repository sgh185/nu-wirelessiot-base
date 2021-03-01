/*
 * Code for sensor/edge nodes of the system
 */ 
#include "sensor_device.h"


/*
 * ---------- Timer Settings ----------
 */ 
#define SCHED_QUEUE_SIZE 32
#define SCHED_EVENT_DATA_SIZE APP_TIMER_SCHED_EVENT_DATA_SIZE


/*
 * Timer definition for per-magnetometor update
 */ 
APP_TIMER_DEF(sensor_update_timer);


/*
 * Timer definition for sensor advertising interval
 */ 
APP_TIMER_DEF(sensor_adv_timer);


/*
 * Timer definition for sensor acknowledgement timeout
 */ 
APP_TIMER_DEF(sensor_ack_timer);


/*
 * Main application state
 */ 
simple_ble_app_t* simple_ble_app;


/*
 * ---------- Callbacks ----------
 */ 
void update_callback(void *context)
{
    /*
     * TOP
     *
     * In this callback, synthesize the magnetometor
     * data if necessary, package the corresponding data
     * into an advertisement, and start sending
     */ 

    /*
     * Fetch the magnetometor data, do nothing if 
     * there is no change in the outcome
     */ 
    bool parking_spot_status = synthesize_data();
    if (parking_spot_status == current_status) return;


    /*
     * Set the status to the new value 
     */ 
    current_status = parking_spot_status;
 

    /*
     * Formulate the new advertisement to send, and
     * start sending new data
     */ 
    rebuild_ad_package();
    simple_ble_adv_raw(the_ad, AD_SIZE);


    /*
     * Start the advertisement timeout timer --- this
     * will dictate how long we advertise new info for
     */ 
    app_timer_start(sensor_adv_timer, APP_TIMER_TICKS(ADV_INTERVAL), NULL);


    return;
}


void adv_callback(void *context)
{
    /*
     * TOP
     *
     * In this callback, we must have executed at least
     * ADV_INTERVAL amount of time sending out ads. Kill
     * the ads and switch to listening for acknowledgements
     * (i.e. ads) from the relayers in the layer above
     */ 
   
    /*
     * We're done advertising for now --- stop
     */  
    advertising_stop();


    /*
     * Start the timer for the acknowledgement listening
     * period --- NOTE --- this MUST execute before scanning
     * because found ads will generate events and process
     */
    app_timer_start(sensor_ack_timer, APP_TIMER_TICKS(ACK_INTERVAL), NULL);


    /*
     * Start listening for acknowledgements 
     */
    scanning_start();


    return;
}


void ack_callback(void *context)
{
    /*
     * TOP 
     *
     * In this callback, we've timed out --- i.e. we haven't
     * heard back from the relayer nodes on if it's received
     * the data the sensor node has sent. We'll try again.
     */ 
}


int main(void) 
{
    /*
     * Start ...
     */ 
    printf("Board started. Initializing BLE: \n");

 
    /*
     * Handle BLE
     */  
    simple_ble_app = simple_ble_init(&ble_config);

   
    /*
     * Initialize scheduler, timers
     */ 
    APP_SCHED_INIT(
	SCHED_EVENT_DATA_SIZE, 
	SCHED_QUEUE_SIZE
    );
    
    app_timer_init();


    /*
     * Create all three timers
     */ 
    app_timer_create(&sensor_update_timer, APP_TIMER_MODE_REPEATED, update_callback);	
    app_timer_create(&sensor_adv_timer, APP_TIMER_MODE_SINGLE_SHOT, adv_callback);	
    app_timer_create(&sensor_ack_timer, APP_TIMER_SINGLE_SHOT, ack_callback); 

    
    /*
     * Start update timer
     */
    app_timer_start(sensor_update_timer, APP_TIMER_TICKS(UPDATE_INTERVAL), NULL);


    /*
     * Do nothing ... 
     */ 
    while(1) power_manage();	
}


