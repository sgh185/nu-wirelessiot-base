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


// Callback handler for advertisement reception
void ble_evt_adv_report(ble_evt_t const* p_ble_evt) {

  // extract the fields we care about
  ble_gap_evt_adv_report_t const* adv_report = &(p_ble_evt->evt.gap_evt.params.adv_report);
  uint8_t const* ble_addr = adv_report->peer_addr.addr; // array of 6 bytes of the address
  uint8_t* adv_buf = adv_report->data.p_data; // array of up to 31 bytes of advertisement payload data
  uint16_t adv_len = adv_report->data.len; // length of advertisement payload data


  /*
  * Filter for certain devices:
  */
  // print address, and adv_length
  // printf("\naddress:\n\t");
  // for (int i=0; i < 6; i++){
  //   printf("%x ", ble_addr[i]);
  // }
  // printf("\nlength:");
  // printf("\n\t%d", adv_len);
  // printf("\npayload:");

  /*
  * this loop prints all of the payload
  */
  // for (int i=0; i < adv_len; i++){
  //   printf("%x ", adv_buf[i]);
  // }

  /*
  * this loop parses through payload (in Length-Type-Value format) and prints
  */
  // for (int i=0; i < adv_len; i++){
  //   // getting length
  //   int length = adv_buf[i];
  //   printf("\n");
  //   printf("\n\tlength: %d\n", length);
  //   for (int j=0; j < length; j++){
  //     i++;
  //     // getting type
  //     if (j == 0){
  //       printf("\ttype: \t%x\n", adv_buf[i+j]);
  //       printf("\tvalue:\t");
  //     }
  //     // getting value
  //     else{
  //       printf("%x ", adv_buf[i+j-1]);
  //     }
  //   }
  // }

  /*
  * this loop selectively prints payload based off BLE address
  */
  bool flag = true;
  // my other advertising nordic
  // c0:98:e5:4e:xx:xx
  // AABB (note this needs to be converted to little endian)
  int find_address[6] = {0xc0, 0x98, 0xe5, 0x4e, 0xAA, 0xBB};
  for(int i=0; i < 6; i++){
    if (ble_addr[i] != find_address[5 - i]) flag = false;
  }

  // print data
  if (flag == true){
    printf("Received an advertisement!");

    printf("\naddress:\n\t");
    for (int i=0; i < 6; i++){
      printf("%x ", ble_addr[i]);
    }
    printf("\nlength:");
    printf("\n\t%d", adv_len);
    printf("\npayload:");

    for (int i=0; i < adv_len; i++){
      // getting length
      int length = adv_buf[i];
      printf("\n");
      printf("\n\tlength: %d\n", length);
      for (int j=0; j < length; j++){
        i++;
        // getting type
        if (j == 0){
          printf("\ttype: \t%x\n", adv_buf[i+j]);
          printf("\tvalue:\t");
        }
        // getting value
        else{
          printf("%x ", adv_buf[i+j-1]);
        }
      }
    }
    printf("\n");
    printf("\n");
  }

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
