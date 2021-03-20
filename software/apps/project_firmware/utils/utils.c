/*
 * Implementations of utilities
 */ 
#include "utils.h"

void scanning_stop(void)
{
    /*
     * Stop the scan
     */ 
    uint32_t err_code = sd_ble_gap_scan_stop();


    /*
     * It's possible this function is called even
     * though scanning has already stopped --- make
     * sure the corresponding error code is not 
     * considered
     */ 
    if (err_code != NRF_ERROR_INVALID_STATE) APP_ERROR_CHECK(err_code);


    return;
}


#if SIMULATE
bool synthesize_data(void)
{
    return ((bool) rand() & 1);
}

#else
bool synthesize_data(void)
{
    return ((bool) rand() & 1);
}

#endif


void print_buffer(
    uint8_t *adv_buf, 
    uint16_t adv_len
)
{
    /*
     * Print out @adv_buf
     */ 
    DEBUG_PRINT("print_buffer:\n");
    for (uint16_t i = 0 ; i < adv_len ; i++) DEBUG_PRINT("%x ", adv_buf[i]);
    DEBUG_PRINT("\n");


    return;
}
