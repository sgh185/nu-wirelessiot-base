/*
 * Implementation of portions of the ad interface
 */

#include "ad.h"

/*
 * ---------- Definitions ----------
 */ 
uint8_t the_ad[AD_SIZE] = { 0 };
uint8_t ad_flags_info = 0;
uint8_t ad_did_info[DID_LEN - 1] = { 0 };


/*
 * ---------- Methods ----------
 */ 
uint8_t _set_bit(
    uint8_t octet,
    uint8_t index
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
     * Device ID structure should be initialized instead
     * - Sender info can be filled out, target info can stay
     *   zeroed since that signifies the central node anyway
     *   (where device ID and layer ID are both 0). Sector
     *   number is default 0. 
     */     
    set_sender_device_id(device_ID);
    set_sender_layer_id(layer_ID);
    set_sector_num(DEFAULT_SECTOR_NO); 


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
     * Device ID structure should be initialized instead
     * - Sender info can be filled out, target info can stay
     *   zeroed, and left subject to change over time. Sector
     *   number is default 0. 
     */     
    set_relay_flag(1); 
    set_sender_device_id(device_ID);
    set_sender_layer_id(layer_ID);
    set_sector_num(DEFAULT_SECTOR_NO); 


    return;
}

