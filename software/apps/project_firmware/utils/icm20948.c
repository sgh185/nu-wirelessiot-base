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

#include "icm20948.h"

// ICM address and Magnetometer address
static uint8_t MPU_ADDRESS = 0x69; // might be 0x69 or 0x68
static uint8_t MAG_ADDRESS = 0x0C; // magnetometer address

static const nrf_twi_mngr_t* i2c_manager = NULL;

static uint8_t i2c_reg_read(uint8_t i2c_addr, uint8_t reg_addr) {
  uint8_t rx_buf = 0;
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(i2c_addr, &rx_buf, 1, 0),
  };
  // printf("asdf\n");
  // printf("%p\n", i2c_manager);
  ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  // printf("sadfasdf\n");
  APP_ERROR_CHECK(error_code);
  // printf("asdfasdfasdfasdf\n");
  return rx_buf;
}

static void i2c_reg_write(uint8_t i2c_addr, uint8_t reg_addr, uint8_t data) {
  uint8_t buf[2] = {reg_addr, data};
  nrf_twi_mngr_transfer_t const write_transfer[] = {
    NRF_TWI_MNGR_WRITE(i2c_addr, buf, 2, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, write_transfer, 1, NULL);
  APP_ERROR_CHECK(error_code);
}

// initialization and configuration
void icm20948_init(const nrf_twi_mngr_t* i2c) {
  printf("initializing...\n");
  i2c_manager = i2c;

  // initialize a timer - the default frequency is 16MHz
  nrf_drv_timer_config_t timer_cfg = {
    .frequency          = NRF_TIMER_FREQ_1MHz,
    .mode               = NRF_TIMER_MODE_TIMER,
    .bit_width          = NRF_TIMER_BIT_WIDTH_32,
    .interrupt_priority = NRFX_TIMER_DEFAULT_CONFIG_IRQ_PRIORITY,
    .p_context          = NULL,
  };

  // reset mpu, delay 100 ms
  printf("reset MPU\n");
  i2c_reg_write(MPU_ADDRESS, ICM20948_PWR_MGMT_1, 0x80); // should this just be 0x8?
  nrf_delay_ms(100);

  // disable sleep mode
  printf("disable sleep\n");
  i2c_reg_write(MPU_ADDRESS, ICM20948_PWR_MGMT_1, 0x00);

  // enable bypass mode
  printf("enable bypass\n");
  i2c_reg_write(MPU_ADDRESS, ICM20948_USER_CTRL, 0x00);
  nrf_delay_ms(3);
  i2c_reg_write(MPU_ADDRESS, ICM20948_INT_PIN_CFG, 0x02);

  // configure accelerometer range?
  // i2c_reg_write(MPU_ADDRESS, ICM20948_ACCEL_CONFIG, 0x00);

  // reset magnetometer
  printf("reset magnetometer\n");
  i2c_reg_write(MAG_ADDRESS, AK09916_CNTL3, 0x01);
  // i2c_reg_write(MAG_ADDRESS, AK09916_CNTL2, 0x00); // power-down
  nrf_delay_ms(100);

  // configure magnetometer, enable continuous measurement mode (8 Hz)
  printf("configure magnetometer\n");
  i2c_reg_write(MAG_ADDRESS, AK09916_CNTL2, 0x02);
}

// read accelerometer
icm20948_measurement_t icm20948_read_accelerometer() {
    printf("hello\n");
  // read values
  int16_t x_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, ICM20948_ACCEL_XOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, ICM20948_ACCEL_XOUT_L);
  int16_t y_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, ICM20948_ACCEL_YOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, ICM20948_ACCEL_YOUT_L);
  int16_t z_val = (((uint16_t)i2c_reg_read(MPU_ADDRESS, ICM20948_ACCEL_ZOUT_H)) << 8) | i2c_reg_read(MPU_ADDRESS, ICM20948_ACCEL_ZOUT_L);
    printf("goodbye\n");
  // convert to g
  // coversion at +/- 2 g is 16384 LSB/g -> probably wrong
  icm20948_measurement_t measurement = {0};
  measurement.x_axis = ((float)x_val) / 16384;
  measurement.y_axis = ((float)y_val) / 16384;
  measurement.z_axis = ((float)z_val) / 16384;
  return measurement;
}

// check magnetometer to see if it's working
bool icm20948_check_magnetometer(){
  // read data
  // must read 1 byte from the WIA register
  uint8_t reg_addr = AK09916_WIA;
  uint8_t rx_buf[1] = {0};
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(MAG_ADDRESS, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(MAG_ADDRESS, rx_buf, 1, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  APP_ERROR_CHECK(error_code);

  /* printf("checking for value 9, got value: %d\n", rx_buf[0]); */

  if(rx_buf[0] == 0b1001){
    return true;
  }

  return false;
}

// read measurement data from magnetometer
icm20948_measurement_t icm20948_read_magnetometer() {

  // read data
  // must read 8 bytes starting at the first status register
  uint8_t reg_addr = AK09916_ST1;
  uint8_t rx_buf[8] = {0};
  nrf_twi_mngr_transfer_t const read_transfer[] = {
    NRF_TWI_MNGR_WRITE(MAG_ADDRESS, &reg_addr, 1, NRF_TWI_MNGR_NO_STOP),
    NRF_TWI_MNGR_READ(MAG_ADDRESS, rx_buf, 8, 0),
  };
  ret_code_t error_code = nrf_twi_mngr_perform(i2c_manager, NULL, read_transfer, 2, NULL);
  APP_ERROR_CHECK(error_code);

#if 0
  printf("\nraw\n");
  for (uint8_t i = 0; i < 8 ; i++) printf("%x ", rx_buf[i]);
  printf("\n"); 
#endif

  // determine values
  int16_t x_val = (((uint16_t)rx_buf[2]) << 8) | rx_buf[1];
  int16_t y_val = (((uint16_t)rx_buf[4]) << 8) | rx_buf[3];
  int16_t z_val = (((uint16_t)rx_buf[6]) << 8) | rx_buf[5];

  // convert to g
  // data is twos complement number that needs to be multiplied by 0.15 
  // ** NOTE --- Change to large multiplier to inflate small movements **
  icm20948_measurement_t measurement = {0};
  measurement.x_axis = ((float)x_val) * 1000.0; //* 0.15;
  measurement.y_axis = ((float)y_val) * 1000.0; //* 0.15;
  measurement.z_axis = ((float)z_val) * 1000.0; //* 0.15;

  return measurement;
}


#if !SIMULATE
#define THRESHOLD 0.100f
bool synthesize_data(void)
{
    /*
     * Fetch measurement from *accelerometer* 
     */ 
    DEBUG_PRINT("Fetching data ...\n");
    icm20948_measurement_t measurement = icm20948_read_accelerometer();


    /*
     * Debugging
     */ 
    DEBUG_PRINT("                      X-Axis\t    Y-Axis\t    Z-Axis\n");
    DEBUG_PRINT("                  ----------\t----------\t----------\n");
    DEBUG_PRINT("Acceleration (g): %10.3f\t%10.3f\t%10.3f\n", measurement.x_axis, measurement.y_axis, measurement.z_axis);
    DEBUG_PRINT("\n");


    /*
     * Synthesize a binary value based on the measurement --- is
     * the measurement less or greater than a defined threshold? 
     */ 
    bool flag = (measurement.y_axis > THRESHOLD);


    /*
     * Debugging
     */ 
    DEBUG_PRINT("Result: %d\n", flag);


    return flag;
}
#endif


void initialize_sensor(const nrf_twi_mngr_t *manager)
{
    /*
     * Initialize i2c master (two wire interface) 
     */
    ret_code_t error_code = NRF_SUCCESS;
    nrf_drv_twi_config_t i2c_config = NRF_DRV_TWI_DEFAULT_CONFIG;
    i2c_config.scl = SCL_PIN;
    i2c_config.sda = SDA_PIN;
    i2c_config.frequency = NRF_TWIM_FREQ_100K;
    error_code = nrf_twi_mngr_init(manager, &i2c_config);
    APP_ERROR_CHECK(error_code);


    /*
     * Initialize ICM20948 
     */ 
    icm20948_init(manager);
    DEBUG_PRINT("ICM20948 initialized\n");


    return;
}

