// MPU-9250 driver
//
// Read from MPU-9250 3-axis accelerometer/gyro/magnetometer over I2C
// Register documentation: http://www.invensense.com/wp-content/uploads/2017/11/RM-MPU-9250A-00-v1.6.pdf
//
// Based on https://github.com/sparkfun/SparkFun_MPU-9250-DMP_Arduino_Library/blob/master/src/util/inv_mpu.c

#include <stdbool.h>
#include <stdint.h>

#include "app_error.h"
#include "nrf.h"
#include "nrf_delay.h"
#include "nrf_gpio.h"
#include "nrf_log.h"
#include "nrf_log_ctrl.h"
#include "nrf_log_default_backends.h"
#include "nrf_pwr_mgmt.h"
#include "nrf_serial.h"
#include "nrf_twi_mngr.h"
#include "nrfx_gpiote.h"


#include "icm20948.h"

// put whichever pin you choose as your SCL and SDA pins here
#define SCL_PIN NRF_GPIO_PIN_MAP(0,28)
#define SDA_PIN NRF_GPIO_PIN_MAP(0,4)


// I2C manager
NRF_TWI_MNGR_DEF(twi_mngr_instance, 5, 0);

int main(void){
  ret_code_t error_code = NRF_SUCCESS;

  // RTT library
  error_code = NRF_LOG_INIT(NULL);
  APP_ERROR_CHECK(error_code);
  NRF_LOG_DEFAULT_BACKENDS_INIT();
  printf("Log initialized\n");

  // initialize i2c master (two wire interface)
  nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
  i2c_config.scl = SCL_PIN;
  i2c_config.sda = SDA_PIN;
  i2c_config.frequency = NRF_TWIM_FREQ_100K;
  error_code = nrf_twi_mngr_init(&twi_mngr_instance, &i2c_config);
  APP_ERROR_CHECK(error_code);

  // initialize ICM20948
  icm20948_init(&twi_mngr_instance);
  printf("ICM20948 initialized\n");

  // check magnetometer reading
  if (icm20948_check_magnetometer()){
    printf("magnetometercheck is true\n");
  }
  else{
    printf("magnetometer check is false\n");
  }

  // loop
  while(1){
    // read measurement
    icm20948_measurement_t acc_measurement = icm20948_read_accelerometer();
    icm20948_measurement_t mag_measurement = icm20948_read_magnetometer();

    // print
    printf("                      X-Axis\t    Y-Axis\t    Z-Axis\n");
    printf("                  ----------\t----------\t----------\n");
    printf("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", acc_measurement.x_axis, acc_measurement.y_axis, acc_measurement.z_axis);
    printf("Magnetism   (uT): %10.3f\t%10.3f\t%10.3f\n", mag_measurement.x_axis, mag_measurement.y_axis, mag_measurement.z_axis);
    printf("\n");

    // wait
    nrf_delay_ms(1000);

    // return the binary value of if a car is there or not -> do threshold
  }
}
