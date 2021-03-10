/*
 * Base device implementation
 */
#include "base_device.h"


/*
 * Device initialization
 */ 
void _initialize_sensor_device_state(void)
{
    /*
     * Zero out the device state
     */
    memset(
	device_state,
	0,
	sizeof(uint32_t) * DEVICE_ENTRIES 
    );


    /*
     * Initialize the device state according to the
     * data/state encoding for each sensor device
     */ 
    device_state[DEVICE_ENTRY] = device_ID;
    device_state[LAYER_ENTRY] = layer_ID;
    device_state[FLAGS_ENTRY] = flags;
    device_state[PARKING_ENTRY] = parking_ID;


    return;
}


void _initialize_relayer_device_state(void)
{
    /*
     * Zero out the device state
     */
    memset(
	device_state,
	0,
	sizeof(uint32_t) * DEVICE_ENTRIES 
    );


    /*
     * Initialize the device state according to the
     * data/state encoding for each relayer device
     */ 
    device_state[DEVICE_ENTRY] = device_ID;
    device_state[LAYER_ENTRY] = layer_ID;
    device_state[FLAGS_ENTRY] = flags;


    return;

}

void _initialize_central_device_state(void);


