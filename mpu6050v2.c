#include <mpu6050v2.h>
#include <main.h>
#include <stdio.h>
#include "string.h"
#include "stm32h7xx_hal.h"

extern I2C_HandleTypeDef hi2c2;

// Global variables to hold acceleration and gyroscope data
float acc_x_g, acc_y_g, acc_z_g;
float gyro_x_dps, gyro_y_dps, gyro_z_dps;

void mpu6050_init() {
    uint8_t data; // Buffer for I2C writes
    HAL_StatusTypeDef status;

    // Check if MPU6050 is responding
    if (HAL_I2C_IsDeviceReady(&hi2c2, DEV_ADDRESS << 1, 1, 100) == HAL_OK) {
        printf("MPU6050 is ready!\n");
    } else {
        printf("MPU6050 not responding!\n");
        return; // Exit if device isn’t ready
    }

    // Exit Sleep Mode (Register 107 or 0x6B) FIRST
    data = EXIT_SLEEP; // Use the macro for consistency
    status = HAL_I2C_Mem_Write(&hi2c2, DEV_ADDRESS << 1, REG_PWR_MGMT, 1, &data, 1, 100);
    if (status == HAL_OK) {
        printf("Successfully exited sleep mode\n");
    } else {
        printf("Failed to exit sleep mode\n");
        return; // Exit if wake-up fails
    }

    // Small delay to allow MPU6050 to stabilize after waking up
    HAL_Delay(10);

    // Gyro Configuration (Register 27)
    data = GYRO_FS_SEL_500; // ±500°/s
    status = HAL_I2C_Mem_Write(&hi2c2, DEV_ADDRESS << 1, REG_CONFIG_GYRO, 1, &data, 1, 100);
    if (status == HAL_OK) {
        printf("Set FS_SEL ±500°/s successfully\n");
    } else {
        printf("Failed to set FS_SEL ±500°/s\n");
    }

    // Accelerometer Configuration (Register 28)
    data = AFS_SEL_4G; // ±4g
    status = HAL_I2C_Mem_Write(&hi2c2, DEV_ADDRESS << 1, REG_CONFIG_ACC, 1, &data, 1, 100);
    if (status == HAL_OK) {
        printf("Set AFS_SEL ±4g successfully\n");
    } else {
        printf("Failed to set AFS_SEL ±4g\n");

    }
}



void Trigger_Software_Reset() {

    printf("I2C read failed!\n");
       HAL_Delay(1000);
       printf("Device Reset in 3 \n");
       HAL_Delay(1000);
       printf("Device Reset in 2\n");
       HAL_Delay(1000);
       printf("Device Reset in 1\n");
       HAL_Delay(1000);
       printf("Device Reset Successful \n");
       NVIC_SystemReset();  // Perform a full system reset

}




void mpu6050_accel(float *ax, float *ay, float *az) {
    uint8_t data_x_accel[2], data_y_accel[2], data_z_accel[2];
    int16_t acc_x, acc_y, acc_z;
    float sensitivity = 8192.0;  // ±4g → 8192 LSB/g

    if (HAL_I2C_Mem_Read(&hi2c2, DEV_ADDRESS << 1, REG_DATA_X_ACC , 1, data_x_accel, 2, 100) != HAL_OK) {
        Trigger_Software_Reset();
        return;
    }
    if (HAL_I2C_Mem_Read(&hi2c2, DEV_ADDRESS << 1, REG_DATA_Y_ACC , 1, data_y_accel, 2, 100) != HAL_OK) {
        Trigger_Software_Reset();
        return;
    }
    if (HAL_I2C_Mem_Read(&hi2c2, DEV_ADDRESS << 1, REG_DATA_Z_ACC , 1, data_z_accel, 2, 100) != HAL_OK) {
        Trigger_Software_Reset();
        return;
    }

    // Convert raw data to signed 16-bit integers
    acc_x = ((int16_t)data_x_accel[0] << 8) | data_x_accel[1];
    acc_y = ((int16_t)data_y_accel[0] << 8) | data_y_accel[1];
    acc_z = ((int16_t)data_z_accel[0] << 8) | data_z_accel[1];

    // Convert to g-units
    *ax = (float)acc_x / sensitivity;
    *ay = (float)acc_y / sensitivity;
    *az = (float)acc_z / sensitivity;
}

void mpu6050_gyro(float *gx, float *gy, float *gz) {
    uint8_t data_x_gyro[2], data_y_gyro[2], data_z_gyro[2];
    int16_t gyro_x, gyro_y, gyro_z;
    float sensitivity = 65.5;  // ±500°/s → 65.5 LSB/°/s

    if (HAL_I2C_Mem_Read(&hi2c2, DEV_ADDRESS << 1, REG_DATA_X_GYRO , 1, data_x_gyro, 2, 100) != HAL_OK) {
        Trigger_Software_Reset();
        return;
    }
    if (HAL_I2C_Mem_Read(&hi2c2, DEV_ADDRESS << 1, REG_DATA_Y_GYRO , 1, data_y_gyro, 2, 100) != HAL_OK) {
        Trigger_Software_Reset();
        return;
    }
    if (HAL_I2C_Mem_Read(&hi2c2, DEV_ADDRESS << 1, REG_DATA_Z_GYRO , 1, data_z_gyro, 2, 100) != HAL_OK) {
        Trigger_Software_Reset();
        return;
    }

    // Convert raw data to signed 16-bit integers
    gyro_x = ((int16_t)data_x_gyro[0] << 8) | data_x_gyro[1];
    gyro_y = ((int16_t)data_y_gyro[0] << 8) | data_y_gyro[1];
    gyro_z = ((int16_t)data_z_gyro[0] << 8) | data_z_gyro[1];

    // Convert to degrees per second
    *gx = (float)gyro_x / sensitivity;
    *gy = (float)gyro_y / sensitivity;
    *gz = (float)gyro_z / sensitivity;
}
