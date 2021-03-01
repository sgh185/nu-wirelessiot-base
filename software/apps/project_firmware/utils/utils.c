/*
 * Implementations of utilities
 */ 

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
