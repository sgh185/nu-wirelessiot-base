// BLE Scanning app
//
// Receives BLE advertisements

#include <stdbool.h>
#include <stdint.h>
#include <stdio.h>

#include "simple_ble.h"

#include "nrf52840dk.h"

// BLE configuration
// This is mostly irrelevant since we are scanning only
static simple_ble_config_t ble_config = {
        // BLE address is c0:98:e5:4e:00:02
        .platform_id       = 0x4E,    // used as 4th octet in device BLE address
        .device_id         = 0x0002,  // used as the 5th and 6th octet in the device BLE address, you will need to change this for each device you have
        .adv_name          = "CS397/497", // irrelevant in this example
        .adv_interval      = MSEC_TO_UNITS(1000, UNIT_0_625_MS), // send a packet once per second (minimum is 20 ms)
        .min_conn_interval = MSEC_TO_UNITS(500, UNIT_1_25_MS), // irrelevant if advertising only
        .max_conn_interval = MSEC_TO_UNITS(1000, UNIT_1_25_MS), // irrelevant if advertising only
};
simple_ble_app_t* simple_ble_app;



#define MAX_ADDR_BYTES 6
#define STRUCTURE_TYPE_SIZE 1

// Callback handler for advertisement reception
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) 
{
    // extract the fields we care about
    ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
    uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
    uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
    uint16_t adv_len = adv_report->data.len; // length of advertisement payload data

    printf("Received an advertisement! Parsing ...\n");


    /*
     * Output the advertisement payload length
     */ 
    printf(
	"\tadv_len: \t%u\n",
	adv_len
    );


    /*
     * Print out the advertisement address (6 bytes)
     */ 
    int i;

    printf("\tble_addr: \t");
    for (i = 0 ; i < MAX_ADDR_BYTES ; i++) printf("%x ", ble_addr[i]);
    printf("\n");


    /*
     * Print out the advertisement buffer in full
     */ 
    printf("\tadv_buf: \t"); 
    for (i = 0 ; i < adv_len ; i++) printf("%x ", adv_buf[i]);
    printf("\n");


    /*
     * Now print out each advertisement structure from 
     * within the advertisement payload. Print each
     * structure as [structure_length : type : data]
     */ 
    i = 0; int num_structure = 0;
    while(i < adv_len) 
    {
	printf("\n\t\tStructure %d\n", num_structure);
    

	/*
	 * Fetch the real length of the structure's data,
	 * output, and increment the buffer index
	 */ 
	uint8_t structure_length = adv_buf[i] - STRUCTURE_TYPE_SIZE;
	printf("\t\t\tstructure_length: %x\n", structure_length); 
	i++;


	/*
	 * Fetch the type, output, and increment buffer index 
	 */ 
	uint8_t structure_type = adv_buf[i];
	printf("\t\t\tstructure_type: %x\n", structure_type); 
	i++;


	/*
	 * Iterate through the data and print it out 
	 */ 
	printf("\t\t\tstructure_data: ");
	int data_idx;
	for (data_idx = 0 ; data_idx < structure_length ; data_idx++)
	   printf("%x ", adv_buf[data_idx + i]); 
	printf("\n");


	/*
	 * Set the buffer index, increment the structure no. 
	 */ 
	i += data_idx;
	num_structure++;
    }    


    return;
}


int main(void) {

  // Setup BLE
  // Note: simple BLE is our own library. You can find it in `nrf5x-base/lib/simple_ble/`
  simple_ble_app = simple_ble_init(&ble_config);
  advertising_stop();

  // Start scanning
  scanning_start();

  // go into low power mode
  while(1) {
    power_manage();
  }
}



