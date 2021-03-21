/*
 * Code for sensor/edge nodes of the system
 */ 
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
 * Sensor setup
 */ 
#if !SIMULATE
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);
#endif


/*
 * ---------- Helpers ----------
 */ 
void start_ads_and_scans(void)
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


void stop_ads_and_scans(void)
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


void handle_throttle(uint8_t *adv_buf)
{
    /*
     * Fetch the throttle flag of @adv_buf
     */  
    bool throttle = get_recv_throttle_flag(adv_buf);


    /*
     * Make a change if "throttle_down" IFF there's
     * a change from the current value
     */ 
    if (throttle == (get_throttle_flag())) return;


    /*
     * Stop the timer, reset the value in "the_ad,"
     * and start the timer with the new interval 
     */ 
    set_throttle_flag(throttle);
    update_interval = intervals[((uint8_t) throttle)];
    app_timer_stop(sensor_update_timer);
    app_timer_start(
	sensor_update_timer, 
	APP_TIMER_TICKS(update_interval), 
	NULL
    );


    /*
     * Debugging
     */ 
    DEBUG_PRINT(
	"sensor_device: interval now %ld\n", 
	update_interval
    );


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
     * Fetch the magnetometor and/or accelerometer, do nothing if 
     * there is no change in the outcome (unless simulating)
     */ 
    bool parking_spot_status = 
	(SIMULATE) ? 
	(!get_occupied_flag()) : /* Alternating values */
	(synthesize_data()) ;	

    if (parking_spot_status == get_occupied_flag()) return;


    /*
     * Debugging
     */ 
    DEBUG_PRINT("sensor_device: parking_spot_status: %d\n", parking_spot_status);


    /*
     * Set the status to the new value, update 
     * the sequence number of the new ad 
     */ 
    set_occupied_flag(parking_spot_status);
    incr_device_seq_no(); 


    /*
     * We're ready to send the new advertisement 
     * and start scanning for acks
     */ 
    start_ads_and_scans();
    DEBUG_PRINT("sensor_device: starting ads and scans!\n");


    return;
}


void ble_evt_adv_report (ble_evt_t const *p_ble_evt) 
{
    /*
     * TOP --- Event handler for receiving an advertisement
     * during the ads/scans phase --- filter ads and process
     * only ads that pertain to this particular node
     */ 

    /*
     * If this device has already received an ack (i.e. it's 
     * stopped advertising and scanning) we don't do anything!
     * 
     * Killswitch, may be unnecessary, but in place to handle
     * corner cases where there's overlap between interrupt 
     * handlers and event handlers (unclear)
     */
    if (!is_advertising_and_scanning) return;


    /*
     * Fetch event data/info
     */ 
    ble_gap_evt_adv_report_t const *adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t *adv_buf = adv_report->data.p_data; 
    uint16_t adv_len = adv_report->data.len; 


    /*
     * Determine whether or not the advertisement is
     * an ack for this specific device --- since we're
     * already scanning, if the ad doesn't fit these
     * conditions, we don't need to do anything else 
     */ 
    if (!is_ad_ack_for_this_device(adv_buf, adv_len)) return;


    /*
     * Debugging
     */ 
    DEBUG_PRINT("sensor_device: received an ack for this device!\n");


    /*
     * Handle throttle --- i.e. check the throttle flag
     * of the ack and change the update timer interval 
     * if necessary
     */ 
    handle_throttle(adv_buf);
    

    /*
     * At this point, we know we have an ack for this
     * device! We have a number of options. Currently,
     * we'll do the simplest operation --- we'll stop
     * advertising and scanning
     */ 
    stop_ads_and_scans();
    DEBUG_PRINT("sensor_device: stopping ads and scans\n");
    
    
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
     * Initialization
     */ 
#if !SIMULATE
    initialize_sensor(&twi_mngr_instance);
#endif

    initialize_ad(sensor);     
    
    APP_SCHED_INIT(
	SCHED_EVENT_DATA_SIZE, 
	SCHED_QUEUE_SIZE
    );
    
    app_timer_init();


    /*
     * Create and start timer
     */ 
    app_timer_create(&sensor_update_timer, APP_TIMER_MODE_REPEATED, update_callback);	
    app_timer_start(sensor_update_timer, APP_TIMER_TICKS(update_interval), NULL);


    /*
     * Debugging
     */ 
    printf(
	"\n\nsensor_device:\nparking_id: %d\ndevice_id: %d\nlayer_id %d\n",
	get_sender_parking_id(),
	get_sender_device_id(),
	get_sender_layer_id()
    );


    /*
     * Do nothing ... 
     */ 
    while(1) power_manage();

}


