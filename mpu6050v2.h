/*
 * mpu6050v2.h
 *
 *  Created on: Feb 23, 2025
 *      Author: agana
 */

#ifndef MPU6050V2_H_
#define MPU6050V2_H_

#include <stdint.h> // Include standard integer types

// MPU6050 Device Address
#define DEV_ADDRESS  0x68  // 7-bit I2C address (binary: 1101000)

//============================================
// Gyroscope Configuration (Register 27)
//============================================

// FS_SEL[bit4:bit3]
#define GYRO_FS_SEL_250    0b00000000  // ±250°/s
#define GYRO_FS_SEL_500    0b00001000  // ±500°/s
#define GYRO_FS_SEL_1000   0b00010000  // ±1000°/s
#define GYRO_FS_SEL_2000   0b00011000  // ±2000°/s

//============================================
// Accelerometer Configuration (Register 28)
//============================================

// AFS_SEL[bit4:bit3]
#define AFS_SEL_2G    0b00000000  // ±2g
#define AFS_SEL_4G    0b00001000  // ±4g
#define AFS_SEL_8G    0b00010000  // ±8g
#define AFS_SEL_16G   0b00011000  // ±16g

//============================================
// Power Management (Register 107)
//============================================
#define EXIT_SLEEP  0x00  // Wake up from sleep mode

//============================================
// Register Addresses
//============================================
#define REG_CONFIG_GYRO   27
#define REG_CONFIG_ACC    28
#define REG_PWR_MGMT      107

// Accelerometer Data Registers
#define REG_DATA_X_ACC    59
#define REG_DATA_Y_ACC    61
#define REG_DATA_Z_ACC    63

// Gyroscope Data Registers
#define REG_DATA_X_GYRO   67
#define REG_DATA_Y_GYRO   69
#define REG_DATA_Z_GYRO   71




//============================================
// Function Prototypes
//============================================

void mpu6050_init(void);
void mpu6050_accel(float *ax, float *ay, float *az);
void mpu6050_gyro(float *gx, float *gy, float *gz);
void mpu6050_temp(void);
void Trigger_Software_Reset(void); // Reset Device

#endif /* MPU6050V2_H_ */
