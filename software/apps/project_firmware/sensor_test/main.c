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
#include "nrf_drv_timer.h"
#include "nrf_twi_mngr.h"

#include "nrf_gpio.h"

#include "icm20948.h"

// put whichever pin you choose as your SCL and SDA pins here
#define SCL_PIN NRF_GPIO_PIN_MAP()
#define SDA_PIN NRF_GPIO_PIN_MAP()
