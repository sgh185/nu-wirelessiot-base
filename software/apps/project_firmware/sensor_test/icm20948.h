// ICM-20948 driver
//
// Read from v 3-axis magnetometer over I2C

#pragma once

#include "app_error.h"
#include "nrf_twi_mngr.h"

// Types

typedef struct {
	float x_axis;
	float y_axis;
	float z_axis;
} icm20948_measurement_t;


// Function prototypes

// Initialize and configure the ICM-20948
//
// i2c - pointer to already initialized and enabled twim instance
void icm20948_init(const nrf_twi_mngr_t* i2c);

// Read WIA and check that we are getting proper values
//
// return boolean
bool icm20948_check_magnetometer();

// Read all three axes on the magnetometer
//
// Return measurements as floating point values in uT
icm20948_measurement_t icm20948_read_magnetometer();

// Definitions (rewritten for icm20948)

typedef enum {
	ICM20948_WHO_AM_I             = 0x00,
  ICM20948_USER_CTRL            = 0x03,
  ICM20948_LP_CONFIG            = 0x05,
  ICM20948_PWR_MGMT_1           = 0x06,
  ICM20948_PWR_MGMT_2           = 0x07,
  ICM20948_INT_PIN_CFG          = 0x0F,
  ICM20948_INT_ENABLE           = 0x10,
  ICM20948_INT_ENABLE1          = 0x11,
  ICM20948_INT_ENABLE2          = 0x12,
  ICM20948_INT_ENABLE3          = 0x13,
  ICM20948_I2C_MST_STATUS       = 0x17,
  ICM20948_INT_STATUS           = 0x19,
  ICM20948_INT_STATUS_1         = 0x1A,
  ICM20948_INT_STATUS_2         = 0x1B,
  ICM20948_INT_STATUS_3         = 0x1C,
  ICM20948_DELAY_TIMEH          = 0x28,
  ICM20948_DELAY_TIMEL          = 0x29,
  ICM20948_ACCEL_XOUT_H         = 0x2D,
  ICM20948_ACCEL_XOUT_L         = 0x2E,
  ICM20948_ACCEL_YOUT_H         = 0x2F,
  ICM20948_ACCEL_YOUT_L         = 0x30,
  ICM20948_ACCEL_ZOUT_H         = 0x31,
  ICM20948_ACCEL_ZOUT_L         = 0x32,
  ICM20948_GYRO_XOUT_H          = 0x33,
  ICM20948_GYRO_XOUT_L          = 0x34,
  ICM20948_GYRO_YOUT_H          = 0x35,
  ICM20948_GYRO_YOUT_L          = 0x36,
  ICM20948_GYRO_ZOUT_H          = 0x37,
  ICM20948_GYRO_ZOUT_L          = 0x39,
  ICM20948_TEMP_OUT_H           = 0x39,
  ICM20948_TEMP_OUT_L           = 0x3A,
  ICM20948_EXT_SLV_SENS_DATA_00 = 0x3B,
  ICM20948_EXT_SLV_SENS_DATA_01 = 0x3C,
  ICM20948_EXT_SLV_SENS_DATA_02 = 0x3D,
  ICM20948_EXT_SLV_SENS_DATA_03 = 0x3E,
  ICM20948_EXT_SLV_SENS_DATA_04 = 0x3F,
  ICM20948_EXT_SLV_SENS_DATA_05 = 0x40,
  ICM20948_EXT_SLV_SENS_DATA_06 = 0x41,
  ICM20948_EXT_SLV_SENS_DATA_07 = 0x42,
  ICM20948_EXT_SLV_SENS_DATA_08 = 0x43,
  ICM20948_EXT_SLV_SENS_DATA_09 = 0x44,
  ICM20948_EXT_SLV_SENS_DATA_10 = 0x45,
  ICM20948_EXT_SLV_SENS_DATA_11 = 0x46,
  ICM20948_EXT_SLV_SENS_DATA_12 = 0x47,
  ICM20948_EXT_SLV_SENS_DATA_13 = 0x48,
  ICM20948_EXT_SLV_SENS_DATA_14 = 0x49,
  ICM20948_EXT_SLV_SENS_DATA_15 = 0x4A,
  ICM20948_EXT_SLV_SENS_DATA_16 = 0x4B,
  ICM20948_EXT_SLV_SENS_DATA_17 = 0x4C,
  ICM20948_EXT_SLV_SENS_DATA_18 = 0x4D,
  ICM20948_EXT_SLV_SENS_DATA_19 = 0x4E,
  ICM20948_EXT_SLV_SENS_DATA_20 = 0x4F,
  ICM20948_EXT_SLV_SENS_DATA_21 = 0x50,
  ICM20948_EXT_SLV_SENS_DATA_22 = 0x51,
  ICM20948_EXT_SLV_SENS_DATA_23 = 0x52,
  ICM20948_FIFO_EN_1            = 0x66,
  ICM20948_FIFO_EN_2            = 0x67,
  ICM20948_FIFO_RST             = 0x68,
  ICM20948_FIFO_MODE            = 0x69,
  ICM20948_FIFO_COUNTH          = 0x70,
  ICM20948_FIFO_COUNTL          = 0x71,
  ICM20948_FIFO_R_W             = 0x72,
  ICM20948_DATA_RDY_STATUS      = 0x74,
  ICM20948_FIFO_CFG             = 0x76,
  ICM20948_REG_BANK_SEL         = 0x7F,
} ICM20948_reg_t;

// Magnetometer registers
typedef enum {
  AK09916_WIA = 0x01,
  AK09916_ST1 = 0x10,
  // measurement data
  AK09916_HXL = 0x11,
	AK09916_HXH = 0x12,
	AK09916_HYL = 0x13,
	AK09916_HYH = 0x14,
	AK09916_HZL = 0x15,
  AK09916_HZH = 0x16,
  AK09916_TMPS = 0x17,
  //
  AK09916_ST2 = 0x18,
  AK09916_CNTL2 = 0x31,
  AK09916_CNTL3 = 0x32,
  AK09916_TS1 = 0x33,
  AK09916_TS2 = 0x34,
} ak8963_reg_t;
