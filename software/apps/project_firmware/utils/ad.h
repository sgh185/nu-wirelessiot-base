/*
 * Interface defining the standard advertisement packet
 * used by all devices attached to the parking sensor
 * system
 */ 

#include "utils.h"


/*
 * Set up advertisement array 
 *
 * @the_ad will contain two structures (tentatively):
 * - Flags (type=0x1): Contains all necessary bitwise
 *   information necessary for the system protocol
 * - DeviceID (type=0x10): Contains all info about 
 *   sender's device/layer IDs and target IDs
 *
 *
 * Layout: (0x?? indicates actual, encoded info)
 * indx: 0		1		2		3
 * data: 0x2 (len) 	0x1 (tid)	0x??		0x6 (len)	
 * 
 * indx: 4		5		6		7
 * data: 0x10 (tid) 	0x??		0x??		0x??
 *
 * indx: 8		9
 * data: 0x??		0x??
 */ 
#define AD_SIZE 10 /* (1 + FLAGS_LEN) + (1 + DID_LEN) */
extern uint8_t the_ad[AD_SIZE];


/*
 * ---------- Structures ----------
 */ 

/*
 * Set up structure-level info
 */ 
#define NUM_STRUCTURES 2


/*
 * Set up "flags" structure
 */ 
#define FLAGS_TYPE 0x1
#define FLAGS_LEN 0x2 /* 1 byte of info */
#define POS_IS_OCCUPIED 0
#define POS_REDUNDANCY 1
#define POS_IS_RELAY 2
#define POS_DIRECTION 3
#define POS_IS_ACK 4
extern uint8_t ad_flags_info; 


/*
 * Set up "device ID" structure
 */ 
#define DID_TYPE 0x10
#define DID_LEN 0x6 /* 5 bytes of info */
#define POS_SDEV_ID 0
#define POS_SLAYER_ID 1
#define POS_TDEV_ID 2
#define POS_TLAYER_ID 3
#define POS_SECTOR_NO 4
extern uint8_t ad_did_info[DID_LEN - 1];


/*
 * ---------- Methods ----------
 */ 

/*
 * Helper methods
 */ 
uint8_t _set_bit(
    uint8_t octet,
    uint8_t index
    bool bit 
);

bool _get_bit(
    uint8_t octet,
    uint8_t index
);


/*
 * Handlers for "flags" structure
 */ 
#define get_occupied_flag() _get_bit(ad_flags_info, POS_IS_OCCUPIED) 
#define get_redundancy_flag() _get_bit(ad_flags_info, POS_REDUNDANCY) 
#define get_relay_flag() _get_bit(ad_flags_info, POS_IS_RELAY) 
#define get_direction_flag() _get_bit(ad_flags_info, POS_DIRECTION) 
#define get_ack_flag() _get_bit(ad_flags_info, POS_IS_ACK) 

#define set_occupied_flag(bit) _set_bit(ad_flags_info, POS_IS_OCCUPIED, bit) 
#define set_redundancy_flag(bit) _set_bit(ad_flags_info, POS_REDUNDANCY, bit) 
#define set_relay_flag(bit) _set_bit(ad_flags_info, POS_IS_RELAY, bit) 
#define set_direction_flag(bit) _set_bit(ad_flags_info, POS_DIRECTION, bit) 
#define set_ack_flag(bit) _set_bit(ad_flags_info, POS_IS_ACK, bit) 


/*
 * Handlers for "device ID" structure
 */ 
#define get_sender_device_id() ad_did_info[POS_SDEV_ID]
#define get_sender_layer_id() ad_did_info[POS_SLAYER_ID]
#define get_target_device_id() ad_did_info[POS_TDEV_ID]
#define get_target_layer_id() ad_did_info[POS_TLAYER_ID]
#define get_sector_num() ad_did_info[POS_SECTOR_NO]

#define set_sender_device_id(val) ad_did_info[POS_SDEV_ID] = val
#define set_sender_layer_id(val) ad_did_info[POS_SLAYER_ID] = val
#define set_target_device_id(val) ad_did_info[POS_TDEV_ID] = val
#define set_target_layer_id(val) ad_did_info[POS_TLAYER_ID] = val
#define set_sector_num(val) ad_did_info[POS_SECTOR_NO] = val








