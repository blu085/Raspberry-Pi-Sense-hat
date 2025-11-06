/** @brief Modified from https://github.com/davebm1/c-sense-hat
 *  @file lsm9ds1.c
 *  @since 2024-03-12
 *  C code to read colour from the
 *  Raspberry Pi Sense HAT add-on board (LSM9DS1 sensor)
 */

#include "lsm9ds1.h"

/*compile with gcc lsm9ds1.c -li2c, run with ./a.out
int main(void) 
{
    // Output 
    printf("magnetometer = %.2f\n", ShGetMG()); 
    printf("accelerometer/gyroscope = %.2f \n", ShGetAG());
    return (0);
}*/

// /** Brief
//  * @since 2024-02-16
//  * @param void
//  * @return 
//  */
// void ShGetMG()
// {
//     int fd = 0;
//     uint8_t status = 0;

//     /* open i2c comms */
//     if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
//         perror("Unable to open i2c device");
//         exit(1);
//     }

//     /* configure i2c slave */
//     if (ioctl(fd, I2C_SLAVE, LSM9DS1_I2C_ADDR_MG) < 0) {
//         perror("Unable to configure i2c slave device");
//         close(fd);
//         exit(1);
//     }

//     printf("\nThe device at %#x identifies as %#x\n",LSM9DS1_I2C_ADDR_MG,i2c_smbus_read_byte_data(fd, LSM9DS1_WHO_AM_I));

//     /* check we are who we should be */
//     if (i2c_smbus_read_byte_data(fd, LSM9DS1_WHO_AM_I) != LSM9DS1_DEV_ID_MG) {
//         printf("%s\n", "who_am_i error");
//         close(fd);
//         exit(1);
//     }

//     /* Power down the device */
//     i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);
//     close(fd);

//     return (0);
// }

// /** Brief
//  * @since 2024-02-16
//  * @param void
//  * @return 
//  */
// void ShGetAG()
// {
//     int fd = 0;
//     uint8_t status = 0;

//     /* open i2c comms */
//     if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
//         perror("Unable to open i2c device");
//         exit(1);
//     }

//     /* configure i2c slave */
//     if (ioctl(fd, I2C_SLAVE, LSM9DS1_I2C_ADDR_AG) < 0) {
//         perror("Unable to configure i2c slave device");
//         close(fd);
//         exit(1);
//     }

//     printf("\nThe device at %#x identifies as %#x\n",LSM9DS1_I2C_ADDR_AG,i2c_smbus_read_byte_data(fd, LSM9DS1_WHO_AM_I));

//     /* check we are who we should be */
//     if (i2c_smbus_read_byte_data(fd, LSM9DS1_WHO_AM_I) != LSM9DS1_DEV_ID_AG) {
//         printf("%s\n", "who_am_i error");
//         close(fd);
//         exit(1);
//     }

//     /* Power down the device */
//     i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);
//     close(fd);

//     return (0);
// }

