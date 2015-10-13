/*
 * adis_regs.h
 *
 *  Created on: 2 lis 2013
 *      Author: konrad
 */

#ifndef ADIS_REGS_H_
#define ADIS_REGS_H_

#define ADIS16400_READ_REG(a)    a<<8
#define ADIS16400_WRITE_REG(a) ((a) | 0x80)<<8
#define ADIS16400_WRITE_UPPER_HALF 1

#define ADIS16400_FLASH_CNT  0x00 /* Flash memory write count */
#define ADIS16400_SUPPLY_OUT 0x02 /* Power supply measurement */
#define ADIS16400_XGYRO_OUT 0x04 /* X-axis gyroscope output */
#define ADIS16400_YGYRO_OUT 0x06 /* Y-axis gyroscope output */
#define ADIS16400_ZGYRO_OUT 0x08 /* Z-axis gyroscope output */
#define ADIS16400_XACCL_OUT 0x0A /* X-axis accelerometer output */
#define ADIS16400_YACCL_OUT 0x0C /* Y-axis accelerometer output */
#define ADIS16400_ZACCL_OUT 0x0E /* Z-axis accelerometer output */
#define ADIS16400_XMAGN_OUT 0x10 /* X-axis magnetometer measurement */
#define ADIS16400_YMAGN_OUT 0x12 /* Y-axis magnetometer measurement */
#define ADIS16400_ZMAGN_OUT 0x14 /* Z-axis magnetometer measurement */
#define ADIS16400_TEMP_OUT  0x16 /* Temperature output */
#define ADIS16400_AUX_ADC   0x18 /* Auxiliary ADC measurement */

#define ADIS16350_XTEMP_OUT 0x10 /* X-axis gyroscope temperature measurement */
#define ADIS16350_YTEMP_OUT 0x12 /* Y-axis gyroscope temperature measurement */
#define ADIS16350_ZTEMP_OUT 0x14 /* Z-axis gyroscope temperature measurement */

#define ADIS16300_PITCH_OUT 0x12 /* X axis inclinometer output measurement */
#define ADIS16300_ROLL_OUT  0x14 /* Y axis inclinometer output measurement */
#define ADIS16300_AUX_ADC   0x16 /* Auxiliary ADC measurement */

/* Calibration parameters */
#define ADIS16400_XGYRO_OFF 0x1A /* X-axis gyroscope bias offset factor */
#define ADIS16400_YGYRO_OFF 0x1C /* Y-axis gyroscope bias offset factor */
#define ADIS16400_ZGYRO_OFF 0x1E /* Z-axis gyroscope bias offset factor */
#define ADIS16400_XACCL_OFF 0x20 /* X-axis acceleration bias offset factor */
#define ADIS16400_YACCL_OFF 0x22 /* Y-axis acceleration bias offset factor */
#define ADIS16400_ZACCL_OFF 0x24 /* Z-axis acceleration bias offset factor */
#define ADIS16400_XMAGN_HIF 0x26 /* X-axis magnetometer, hard-iron factor */
#define ADIS16400_YMAGN_HIF 0x28 /* Y-axis magnetometer, hard-iron factor */
#define ADIS16400_ZMAGN_HIF 0x2A /* Z-axis magnetometer, hard-iron factor */
#define ADIS16400_XMAGN_SIF 0x2C /* X-axis magnetometer, soft-iron factor */
#define ADIS16400_YMAGN_SIF 0x2E /* Y-axis magnetometer, soft-iron factor */
#define ADIS16400_ZMAGN_SIF 0x30 /* Z-axis magnetometer, soft-iron factor */

#define ADIS16400_GPIO_CTRL 0x32 /* Auxiliary digital input/output control */
#define ADIS16400_MSC_CTRL  0x34 /* Miscellaneous control */
#define ADIS16400_SMPL_PRD  0x36 /* Internal sample period (rate) control */
#define ADIS16400_SENS_AVG  0x38 /* Dynamic range and digital filter control */
#define ADIS16400_SLP_CNT   0x3A /* Sleep mode control */
#define ADIS16400_DIAG_STAT 0x3C /* System status */

/* Alarm functions */
#define ADIS16400_GLOB_CMD  0x3E /* System command */
#define ADIS16400_ALM_MAG1  0x40 /* Alarm 1 amplitude threshold */
#define ADIS16400_ALM_MAG2  0x42 /* Alarm 2 amplitude threshold */
#define ADIS16400_ALM_SMPL1 0x44 /* Alarm 1 sample size */
#define ADIS16400_ALM_SMPL2 0x46 /* Alarm 2 sample size */
#define ADIS16400_ALM_CTRL  0x48 /* Alarm control */
#define ADIS16400_AUX_DAC   0x4A /* Auxiliary DAC data */

#define ADIS16400_PRODUCT_ID 0x56 /* Product identifier */

#define ADIS16400_ERROR_ACTIVE                  (1<<14)
#define ADIS16400_NEW_DATA                      (1<<14)

/* MSC_CTRL */
#define ADIS16400_MSC_CTRL_MEM_TEST             (1<<11)
#define ADIS16400_MSC_CTRL_INT_SELF_TEST        (1<<10)
#define ADIS16400_MSC_CTRL_NEG_SELF_TEST        (1<<9)
#define ADIS16400_MSC_CTRL_POS_SELF_TEST        (1<<8)
#define ADIS16400_MSC_CTRL_GYRO_BIAS            (1<<7)
#define ADIS16400_MSC_CTRL_ACCL_ALIGN           (1<<6)
#define ADIS16400_MSC_CTRL_DATA_RDY_EN          (1<<2)
#define ADIS16400_MSC_CTRL_DATA_RDY_POL_HIGH    (1<<1)
#define ADIS16400_MSC_CTRL_DATA_RDY_DIO2        (1<<0)

/* SMPL_PRD */
#define ADIS16400_SMPL_PRD_TIME_BASE    (1<<7)
#define ADIS16400_SMPL_PRD_DIV_MASK     0x7F

/* DIAG_STAT */
#define ADIS16400_DIAG_STAT_ZACCL_FAIL  (1<<15)
#define ADIS16400_DIAG_STAT_YACCL_FAIL  (1<<14)
#define ADIS16400_DIAG_STAT_XACCL_FAIL  (1<<13)
#define ADIS16400_DIAG_STAT_XGYRO_FAIL  (1<<12)
#define ADIS16400_DIAG_STAT_YGYRO_FAIL  (1<<11)
#define ADIS16400_DIAG_STAT_ZGYRO_FAIL  (1<<10)
#define ADIS16400_DIAG_STAT_ALARM2      (1<<9)
#define ADIS16400_DIAG_STAT_ALARM1      (1<<8)
#define ADIS16400_DIAG_STAT_FLASH_CHK   (1<<6)
#define ADIS16400_DIAG_STAT_SELF_TEST   (1<<5)
#define ADIS16400_DIAG_STAT_OVERFLOW    (1<<4)
#define ADIS16400_DIAG_STAT_SPI_FAIL    (1<<3)
#define ADIS16400_DIAG_STAT_FLASH_UPT   (1<<2)
#define ADIS16400_DIAG_STAT_POWER_HIGH  (1<<1)
#define ADIS16400_DIAG_STAT_POWER_LOW   (1<<0)

/* GLOB_CMD */
#define ADIS16400_GLOB_CMD_SW_RESET     (1<<7)
#define ADIS16400_GLOB_CMD_P_AUTO_NULL  (1<<4)
#define ADIS16400_GLOB_CMD_FLASH_UPD    (1<<3)
#define ADIS16400_GLOB_CMD_DAC_LATCH    (1<<2)
#define ADIS16400_GLOB_CMD_FAC_CALIB    (1<<1)
#define ADIS16400_GLOB_CMD_AUTO_NULL    (1<<0)

#endif /* ADIS_REGS_H_ */
