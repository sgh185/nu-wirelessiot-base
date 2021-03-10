/*
 * Implementation of portions of the ad interface
 */

#include "ad.h"

/*
 * ---------- Definitions ----------
 */ 
uint8_t the_ad[AD_SIZE] = {
    
    /* Manu info structure */ 
    0x0a /* len */, 0xFF /* tid */, 0, 0, 0, 0, 0, 0, 0, 0x00 /* mi.0 */, 0x31, /* mi.1 */ 

};

uint8_t manu_info[MANU_INFO_SIZE] = { 0x00, 0x31 } ;


/*
 * ---------- Methods ----------
 */ 
uint8_t _set_bit(
    uint8_t octet,
    uint8_t index,
    bool bit 
)
{
    /*
     * Set @index_th bit to @bit and return the
     * resulting value paired with @octet 
     */
    if (bit) return (octet | (((uint8_t) bit) << index));
    else return (octet & (((uint8_t) bit) << index));
}


bool _get_bit(
    uint8_t octet,
    uint8_t index
)
{
    /*
     * Fetch the @index_th bit from @octet 
     */  
    return ((octet >> index) & 1);
}


void initialize_ad_for_sensor_device(void)
{
    /*
     * TOP --- Set defaults necessary for sensor devices. The
     * only info/values that should change post iniitialization
     * is the occupied flag
     *
     * Flags structure should all be zeroed for sensor devices
     * - Not occupied, not relaying, sending up (i.e. 0), not
     *   an ack. (Redundancy off as default)
     *
     * Manu info. structure should be initialized instead
     * - Sender info can be filled out, target info can stay
     *   zeroed since that signifies the central node anyway
     *   (where device ID and layer ID are both 0).  
     */     
    set_sender_device_id(device_ID);
    set_sender_layer_id(layer_ID);
    set_sender_parking_id(parking_ID);


    return;
}


void initialize_ad_for_relayer_device(void)
{
    /*
     * TOP --- Set defaults necessary for relayer devices.  
     *
     * Flags structure should all be zeroed for relayer devices
     * except for the relaying flag (which should obviously 
     * be set)
     *
     * Manu. info structure should be initialized instead
     * - Sender info can be filled out, target info can stay
     *   zeroed, and left subject to change over time. 
     */     
    set_relay_flag(1); 
    set_sender_device_id(device_ID);
    set_sender_layer_id(layer_ID);


    return;
}

