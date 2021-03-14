/*
 * Code for relayer nodes of the system
 */ 
#include "relayer_device.h"


/*
 * ---------- Methods ----------
 */ 

/*
 * Cache handlers
 */ 
static bool is_ad_already_handled(
    uint8_t sender_parking_ID,
    uint8_t sender_seq_no
)
{
    /*
     * TOP --- Find if @sender_parking_ID and 
     * @sender_seq_no is part of both caches, 
     * return false otherwise 
     */ 

    /*
     * Search for @sender_parking_ID in the "parking_IDs_cache"
     */ 
    uint8_t indices_found[16];
    uint8_t number_of_occurrences = 0;
    rb_find_all(
	sender_parking_ID,
	parking_IDs_cache,
	indices_found,
	&number_of_occurrences
    );


    /*
     * Search for @sender_seq_no within "corresponding_seq_no_cache"
     */ 
    for (int i = 0 ; i < number_of_occurrences ; i++)
	if (rb_get(indices_found[i], corresponding_seq_no_cache) == sender_seq_no)
	    return true;


    return false;
}


static void place_ad_into_cache(
    uint8_t sender_parking_ID,
    uint8_t sender_seq_no
)
{
    /*
     * TOP --- Push @sender_parking_ID and @sender_seq_no
     * to their respective caches
     */ 
    rb_push(
	sender_parking_ID,
	parking_IDs_cache
    );

    rb_push(
	sender_seq_no,
	corresponding_seq_no_cache
    );


    return;    
}


void print_cache(void)
{
    /*
     * TOP --- Print all contents in "parking_IDs_cache"
     * and "corresponding_seq_no_cache" 
     */  
    printf("caches (formatted [pid, seq]):\n");
    for (uint8_t i = 0 ; i < DEFAULT_RING_BUF_SIZE ; i++)
	printf(
	    "[%u, %u] ",
	    rb_get(i, parking_IDs_cache),
	    rb_get(i, corresponding_seq_no_cache)
	);
    printf("\n");


    return;
}



/*
 * Filtering methods
 */ 
static bool should_handle_ad(
    uint8_t *recv_ad,
    uint16_t recv_ad_len
)
{
    /*
     * TOP --- The relayer should handle the received
     * advertisement IFF it's originating from adjacent
     * layers. Otherwise this is not an ad we should
     * be relaying
     *
     * This information is always stored in the sender
     * layer ID within the advertisement.
     */

    /*
     * Check if @recv_ad is from a system device
     */ 
    if (!is_ad_from_a_system_device(recv_ad, recv_ad_len)) return false;


    /*
     * Fetch the sender layer ID from @recv_ad
     */
    uint8_t recv_layer_id = get_recv_sender_layer_id(recv_ad);
    

    /*
     * Check if the fetched layer ID is adjacent to this
     * device, otherwise it's not something to handle 
     */ 
    if (true
	&& ((layer_ID + 1) != recv_layer_id)
	&& ((layer_ID - 1) != recv_layer_id)) return false;


    return true;
}


/*
 * Setup methods
 */ 
static void handle_ad_for_relaying(uint8_t *recv_ad)
{
    /*
     * TOP 
     *
     * Part 1:
     * To handle @recv_ad, we can set the proper fields
     * within @recv_ad in order to relay it:
     * 1) Set the relay flag
     * 2) Set the sender device and layer IDs with *this* 
     * 	  device's info --- overwriting this information
     * 	  will work b/c the central node will idenfity 
     * 	  values based on the parking ID --- acks and filtering
     * 	  can only be possible with setting the sender info
     * 	  to originate from the "closest"/adjacent layer
     *
     * Part 2:
     * The acknowledgement should also be setup --- this
     * is done almost entirely at initialization --- we just
     * simply need to change the target dev/layer ID to the 
     * sender's info so it's ready to be received properly by 
     * the sender 
     *
     * NOTE --- Part 2 has now been encoded directlyu into 
     * @recv_ad because the data that both parts modify are
     * mutually exclusive --- they can be advertised and 
     * handled in both layer above and layer below
     */ 

    /*
     * <Part 2.> 
     */ 
    set_recv_ack_flag(recv_ad, 1);
    set_recv_target_device_id(recv_ad, (get_recv_sender_device_id(recv_ad)));
    set_recv_target_layer_id(recv_ad, (get_recv_sender_layer_id(recv_ad)));


    /*
     * <Part 1.> 
     */ 
    set_recv_relay_flag(recv_ad, 1);
    set_recv_sender_device_id(recv_ad, device_ID);
    set_recv_sender_layer_id(recv_ad, layer_ID); 


    /*
     * We're ready to listen for acks in particular
     * IFF we're not the central node --- i.e. the 
     * layer ID of this device is 0 (only the central
     * node can have a layer ID of 0)
     */ 
    waiting_for_ack = (true && (layer_ID != 0));


    /*
     * Debugging
     */ 
    printf("relayer_device: ack setup: ");
    print_buffer(recv_ad, AD_SIZE);
    

    return;
}


void ble_evt_adv_report (ble_evt_t const* p_ble_evt) 
{
    /*
     * TOP --- Event handler for receiving an advertisement
     *
     * There are two parts to handle here:
     * 1) Ads from an adjacent layer to relay
     * 2) Acknowledgments for a pending relay
     *
     * Part 1:
     * When receiving ads from adjacent layers, they
     * must be checked against the cache to see if they
     * have been recently handled. Otherwise, handle them
     * and enter the pipeline:
     * 
     * a) receive/filter -> b) no cache hit -> c) add to cache -> 
     * d) send a set number of acks -> e) relay info to respective 
     * layer -> f) wait for ack -> g) listen for everything else
     *
     * NOTE --- parts d and e have been combined. Both the ack
     * and the advertisement to send are combined into one.
     *
     * Part 2:
     * If we're waiting for an acknowledgement, then
     * the relayer will filter out all other ads. 
     *
     * Part 1 is a-e
     * Part 2 is f
     * Standard scanning is g, started in main
     *
     * Part 1 cannot happen if Part 2 is in progress
     * for some respective advertisement
     */ 

    /*
     * Fetch event data/info
     */ 
    ble_gap_evt_adv_report_t const *adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t *adv_buf = adv_report->data.p_data; 
    uint16_t adv_len = adv_report->data.len;


    /*
     * <Part 2.> (f) 
     */ 
    if (waiting_for_ack)
	if (is_ad_ack_for_this_device(adv_buf, adv_len))
	{ 
	    if ((ack_ref_count++) == MIN_LEVEL) 
		waiting_for_ack = ack_ref_count = 0; 

	    return;
	}
    
    
    /*
     * <Part 1.> (a)
     */ 
    if (!should_handle_ad(adv_buf, adv_len)) return;


    /*
     * <Part 1.> (b)
     */ 
    uint8_t sender_parking_ID = get_recv_sender_parking_id(adv_buf),
	    sender_seq_no = get_recv_sender_seq_no(adv_buf); 

    if (is_ad_already_handled(sender_parking_ID, sender_seq_no)) return;
    printf("relayer_device: new ad to handle\n");
    print_buffer(adv_buf, adv_len);


    /*
     * <Part 1.> (c)
     */ 
    place_ad_into_cache(
	sender_parking_ID,
	sender_seq_no
    );

    
    /*
     * Debugging
     */ 
    print_cache();


    /*
     * <Part 1.> (d, e)
     */
    advertising_stop();
    handle_ad_for_relaying(adv_buf);
    simple_ble_adv_raw(adv_buf, AD_SIZE);


    /*
     * Debugging
     */ 
    printf(
	"fetched info:\nparking_id: %d\ndevice_id: %d\nlayer_id %d\n\n\n",
	get_recv_sender_parking_id(adv_buf),
	get_recv_sender_device_id(adv_buf),
	get_recv_sender_layer_id(adv_buf)
    );

    
    return;
}


/*
 * Main application state
 */ 
simple_ble_app_t* simple_ble_app;


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
     * Debugging
     */  
    printf(
	"\n\nrelayer_device:\nparking_id: %d\ndevice_id: %d\nlayer_id %d\n",
	get_sender_parking_id(),
	get_sender_device_id(),
	get_sender_layer_id()
    );


    /*
     * Start scanning
     */ 
    scanning_start();
    
    
    /*
     * Do nothing ... 
     */ 
    while(1) power_manage();	
}


