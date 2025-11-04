/** @brief Constants, structures, function prototypes for lsm9ds1
 *  @file lsm9ds1.h
 *  @since 2025-02-19
 */
#ifndef LSM9DS1_H
#define LSM9DS1_H

// Includes
#include <fcntl.h>
#include <i2c/smbus.h>
#include <linux/i2c-dev.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/ioctl.h>
#include <unistd.h>

// Constants
#ifndef DEV_PATH
#define DEV_PATH "/dev/i2c-1"
#endif // DEV_PATH

// Magnetometer 0x1c(0x1e) and Accelerometer/Gyroscope 0x6a(0x6b)
#define LSM9DS1_I2C_ADDR_MG 0x1C
#define LSM9DS1_I2C_ADDR_AG 0x6A
#define LSM9DS1_WHO_AM_I 0x0F
#define LSM9DS1_DEV_ID_MG 0x3D
#define LSM9DS1_DEV_ID_AG 0x68

// Control Registers
#ifndef CTRL_REG1
#define CTRL_REG1 0x20
#endif // CTRL_REG1 
// #ifndef CTRL_REG2
// #define CTRL_REG2 0x21
// #endif // CTRL_REG2

// Magnetometer Output Registers
#define STATUS_REG_M 0x27
#define OUT_X_L_M 0x28
#define OUT_X_H_M 0x29
#define OUT_Y_L_M 0x2A
#define OUT_Y_H_M 0x2B
#define OUT_Z_L_M 0x2C
#define OUT_Z_H_M 0x2D

//Gyroscope Output Registers
#define INT_GEN_SRC_G 0x14
#define OUT_TEMP_L 0x15
#define OUT_TEMP_H 0x16
#define STATUS_REG_G 0x17
#define OUT_X_L_G 0x18
#define OUT_X_H_G 0x19
#define OUT_Y_L_G 0x1A
#define OUT_Y_H_G 0x1B
#define OUT_Z_L_G 0x1C
#define OUT_Z_H_G 0x1D

// Accelerometer Output Registers
#define STATUS_REG_XL 0x27
#define OUT_X_L_XL 0x28
#define OUT_X_H_XL 0x29
#define OUT_Y_L_XL 0x2A
#define OUT_Y_H_XL 0x2B 
#define OUT_Z_L_XL 0x2C
#define OUT_Z_H_XL 0x2D

// Function Prototypes
/// @cond INTERNAL
void ShGetAG();
double ShGetMG();
/// @endcond

#endif // LSM9DS1_H
