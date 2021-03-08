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


