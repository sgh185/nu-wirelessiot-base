/*
 * Code for sensor/edge nodes of the system
 */ 
#include "ads.h"
#include "sensor_device.h"


/*
 * NOTE --- See sensor_device.h for global state, declarations, etc.
 */


/*
 * ---------- Setup ----------
 */ 

/*
 * Timer definition for magnetometor update
 */ 
APP_TIMER_DEF(sensor_update_timer);


/*
 * Main application state
 */ 
simple_ble_app_t* simple_ble_app;


/*
 * ---------- Helpers ----------
 */ 
AI void start_ads_and_scans(void)
{
    /*
     * TOP --- Start advertising "the_ad," start scanning 
     * for acknowledgements, and set status flags
     */ 
    is_advertising_and_scanning = true;
    simple_ble_adv_raw(the_ad, AD_SIZE);
    scanning_start();


    return;
}


AI void stop_ads_and_scans(void)
{
    /*
     * TOP --- Stop advertising "the_ad", stop scans, and
     * reset status flag for acks and scans
     */ 
    is_advertising_and_scanning = false;
    advertising_stop();
    scanning_stop();


    return;
}


/*
 * ---------- Callbacks/Handlers ----------
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
     * If we're still advertising and scanning from the previous
     * iteration, stop all prior ads/scans before proceeding
     */ 
    if (is_advertising_and_scanning) stop_ads_and_scans();


    /*
     * Fetch the magnetometor data, do nothing if 
     * there is no change in the outcome
     */ 
    bool parking_spot_status = synthesize_magnetometer_data();
    if (parking_spot_status == get_occupied_flag()) return;


    /*
     * Set the status to the new value 
     */ 
    set_occupied_flag(parking_spot_status);
 

    /*
     * We're ready to send the new advertisement 
     * and start scanning for acks
     */ 
    start_ads_and_scans();


    return;
}


void ble_evt_adv_report (ble_evt_t const* p_ble_evt) 
{
    /*
     * TOP --- Event handler for receiving an advertisement
     * during the ads/scans phase --- filter ads and process
     * only ads that pertain to this particular node
     */ 

    /*
     * Fetch event data/info
     */ 
    ble_gap_evt_adv_report_t const *adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t const *ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
    uint8_t *adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
    uint16_t *adv_len = adv_report->data.len; // length of advertisement payload data


    /*
     * Filter the info for this device 
     */ 
    if (!is_ad_for_this_device(adv_buf)) return;


    /*
     * Got info specifically for this device! If this device
     * has already received an ack (i.e. it's stopped advertising
     * and scanning) don't do anything else!
     */
    if (!is_advertising_and_scanning) return;


    /*
     * Restructure the data as an acknowledgement
     */ 
    ack *the_ack = 
	process_event_as_ack(
	    adv_buf, 
	    adv_len
	);
   

    /*
     * Process the acknowledgement --- perform any system-wide
     * or device-specific commands originating from the central
     * device (i.e. it's night time or resend data, etc.) 
     *
     * This call will turn off advertising/scanning
     */
    process_ack(the_ack); 


    return;
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
     * Create and staart timer
     */ 
    app_timer_create(&sensor_update_timer, APP_TIMER_MODE_REPEATED, update_callback);	
    app_timer_start(sensor_update_timer, APP_TIMER_TICKS(UPDATE_INTERVAL), NULL);


    /*
     * Do nothing ... 
     */ 
    while(1) power_manage();	
}


