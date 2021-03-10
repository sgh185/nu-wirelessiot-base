/*
 * Interface defining the standard advertisement packet
 * used by all devices attached to the parking sensor
 * system
 */ 
#pragma once

#include "base_device.h"
#include "utils.h"


/*
 * Set up advertisement array 
 *
 * @the_ad will contain two structures (tentatively):
 * - Flags (type=0x1): Contains all necessary bitwise
 *   information necessary for the system protocol
 * - Manufacturer Info (type=0xFF): Repurposed, contains
 *   all info about sender's device/layer IDs and target IDs
 *
 *
 * Layout: (0x?? indicates actual, encoded info)
 * indx: 0		1		2		3
 * data: 0x2 (len) 	0x1 (tid)	0x?? (flags)	0x8 (len)	
 * 
 * indx: 4		5		6		7
 * data: 0xFF (tid) 	0x?? (sdid)	0x?? (slid)	0x?? (tdid)
 *
 * indx: 8		9		10		11
 * data: 0x?? (tlid)	0x?? (pid)	0x?? (seq)	0x00 (mi.0)
 *
 * indx: 12		
 * data: 0x31 (mi.1)
 */ 
#define AD_SIZE 13 /* (1 + FLAGS_LEN) + (1 + MANU_LEN) */
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
#define FLAGS_OFFSET 2 /* Info starts at the_ad[FLAGS_OFFSET] */
/* Bitwise for all positions */
#define POS_IS_OCCUPIED 0  
#define POS_REDUNDANCY 1
#define POS_IS_RELAY 2
#define POS_DIRECTION 3
#define POS_IS_ACK 4


/*
 * Set up "device ID" structure
 */ 
#define MANU_TYPE 0x10
#define MANU_LEN 0x8 /* 7 bytes of info */
#define MANU_OFFSET 5 /* Info starts at the_ad[MANU_OFFSET] */
#define MANU_INFO 0x0031 /* Masquerading as "Gibsons Guitars" */
#define MANU_INFO_SIZE 2 /* 2 bytes of manufacturer ID */
/* Bytewise for all positions */
#define POS_SDEV_ID (0 + MANU_OFFSET)
#define POS_SLAYER_ID (1 + MANU_OFFSET)
#define POS_TDEV_ID (2 + MANU_OFFSET)
#define POS_TLAYER_ID (3 + MANU_OFFSET)
#define POS_PARKING_ID (4 + MANU_OFFSET) 
#define POS_SEQ_NO (5 + MANU_OFFSET) 
#define POS_MANU_INFO (6 + MANU_OFFSET)
extern uint8_t seq_no;
extern uint8_t manu_info[MANU_INFO_SIZE]; /* Variable for MANU_INFO */

/* TBD */
#define POS_SECTOR_NO (4 + MANU_OFFSET) 
#define DEFAULT_SECTOR_NO 0


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
#define get_occupied_flag() _get_bit(the_ad[FLAGS_OFFSET], POS_IS_OCCUPIED) 
#define get_redundancy_flag() _get_bit(the_ad[FLAGS_OFFSET], POS_REDUNDANCY) 
#define get_relay_flag() _get_bit(the_ad[FLAGS_OFFSET], POS_IS_RELAY) 
#define get_direction_flag() _get_bit(the_ad[FLAGS_OFFSET], POS_DIRECTION) 
#define get_ack_flag() _get_bit(the_ad[FLAGS_OFFSET], POS_IS_ACK) 

#define set_occupied_flag(bit) the_ad[FLAGS_OFFSET] = _set_bit(the_ad[FLAGS_OFFSET], POS_IS_OCCUPIED, bit) 
#define set_redundancy_flag(bit) the_ad[FLAGS_OFFSET] = _set_bit(the_ad[FLAGS_OFFSET], POS_REDUNDANCY, bit) 
#define set_relay_flag(bit) the_ad[FLAGS_OFFSET] = _set_bit(the_ad[FLAGS_OFFSET], POS_IS_RELAY, bit) 
#define set_direction_flag(bit) the_ad[FLAGS_OFFSET] = _set_bit(the_ad[FLAGS_OFFSET], POS_DIRECTION, bit) 
#define set_ack_flag(bit) the_ad[FLAGS_OFFSET] = _set_bit(the_ad[FLAGS_OFFSET], POS_IS_ACK, bit) 


/*
 * Handlers for "manufacturer info" structure
 */ 
#define get_sender_device_id() the_ad[POS_SDEV_ID]
#define get_sender_layer_id() the_ad[POS_SLAYER_ID]
#define get_target_device_id() the_ad[POS_TDEV_ID]
#define get_target_layer_id() the_ad[POS_TLAYER_ID]
#define get_sender_parking_id() the_ad[POS_PARKING_ID]
#define get_sender_seq_no() the_ad[POS_SEQ_NO]

#define set_sender_device_id(val) the_ad[POS_SDEV_ID] = val
#define set_sender_layer_id(val) the_ad[POS_SLAYER_ID] = val
#define set_target_device_id(val) the_ad[POS_TDEV_ID] = val
#define set_target_layer_id(val) the_ad[POS_TLAYER_ID] = val
#define set_sender_parking_id(val) the_ad[POS_PARKING_ID] = val

#define MAX_SEQ_NO 255
#define set_sender_seq_no(val) the_ad[POS_SEQ_NO] = val
#define incr_generic_seq_no(val) ((val + 1) % MAX_SEQ_NO)
#define incr_device_seq_no() set_sender_seq_no((seq_no = incr_generic_seq_no(seq_no)))

/* TBD */ 
#define get_sector_num() the_ad[POS_SECTOR_NO]
#define set_sector_num(val) the_ad[POS_SECTOR_NO] = val


/*
 * Initializers 
 */ 
#define initialize_ad(type) initialize_ad_for_##type##_device()

void initialize_ad_for_sensor_device(void);

void initialize_ad_for_relayer_device(void);


/*
 * Parsers (for received ads)
 */ 
#define get_recv_occupied_flag(buf) _get_bit(buf[FLAGS_OFFSET], POS_IS_OCCUPIED) 
#define get_recv_redundancy_flag(buf) _get_bit(buf[FLAGS_OFFSET], POS_REDUNDANCY) 
#define get_recv_relay_flag(buf) _get_bit(buf[FLAGS_OFFSET], POS_IS_RELAY) 
#define get_recv_direction_flag(buf) _get_bit(buf[FLAGS_OFFSET], POS_DIRECTION) 
#define get_recv_ack_flag(buf) _get_bit(buf[FLAGS_OFFSET], POS_IS_ACK) 

#define get_recv_sender_device_id(buf) buf[POS_SDEV_ID]
#define get_recv_sender_layer_id(buf) buf[POS_SLAYER_ID]
#define get_recv_target_device_id(buf) buf[POS_TDEV_ID]
#define get_recv_target_layer_id(buf) buf[POS_TLAYER_ID]
#define get_recv_sender_parking_id(buf) buf[POS_PARKING_ID]
#define get_recv_sender_seq_no(buf) buf[POS_SEQ_NO]

#define is_ad_from_a_system_device(buf) (!bcmp((buf + POS_MANU_INFO), manu_info, MANU_INFO_SIZE));

#define is_ad_for_this_device(buf) \
({ \
    /*
     * Check if ad:
     * 1) Is from a system device
     * 2) Has a target that matches this device's ID
     * 3) Has the same sector no. (TBD, not now)
     */ \
    bool for_this_device = \
	(true \
	 && is_ad_from_a_system_device(buf) \
	 && (get_recv_target_device_id(buf) == device_ID)); \
    \
    \
    for_this_device ; \
})

#define is_ad_ack_for_this_device(buf) \
({ \
    /*
     * Check if ad:
     * 1) Is for this device
     * 2) Is an acknowledgement
     */ \
    bool ack_for_this_device = \
	(true \
	 && is_ad_for_this_device(buf) \
	 && get_recv_ack_flag(buf)); \
    \
    \
    ack_for_this_device; \
})


