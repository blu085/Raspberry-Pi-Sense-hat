/** @brief moving bubble
 *  @file test.c
 *  @since 2025-02-19
 *  C code Demonstration of Astro Pi Sense Hat.
 *  Raspberry Pi Sense HAT add-on board serial, nav pad, and led2472g
 */
#include "lsm9ds1.h"
#include "led2472g.h"


double xl[2] = {0.0, 0.0};

/** @Brief creating and changing 4 pixels acording to the accelerometer data
 *  @author Huy Bui
 *  @since 2025-02-05
 *  @param void
 *  @return void
 */

 void UpdateLevel(double xa, double ya, struct fb_t *fb)
 {
     int x,y;
 
     ShWipeScreen(BLACK,fb);
     usleep(300000);
     y = (int)(xa * -30.0 +4);
     x = (int)(ya * -30.0 +4);
     if(x<0)
     {
         x=0;
     }
     else if (x>6)
     {
         x=6;
     }
     if(y<0)
     {
         y=0;
     }
     else if (y>6)
     {
         y=6;
     }
     ShLightPixel(x  ,y  ,HY,fb);
     ShLightPixel(x+1,y  ,HY,fb);
     ShLightPixel(x  ,y+1,HY,fb);
     ShLightPixel(x+1,y+1,HY,fb);
     usleep(300000);
 
 }

/** Main function for reading the sensor data */
int main(void)
{
    for (;;)
    {
        ShGetAG();
        printf("x:%.3f|y:%.3f\n", xl[0], xl[1]);

        struct fb_t *fb;
		fb=ShInit(fb);
        usleep(300);
        UpdateLevel(-xl[0], -xl[1], fb);

    }
    return 0;
}

/** @Brief getting data from the accelerometer
 *  @since 2024-02-16
 *  @param void
 *  @return 
 */
void ShGetAG()
{
    int fd = 0;
    uint8_t status = 0;

    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, LSM9DS1_I2C_ADDR_AG) < 0) {
        perror("Unable to configure i2c slave device");
        close(fd);
        exit(1);
    }

    // printf("\nThe device at %#x identifies as %#x\n",LSM9DS1_I2C_ADDR_AG,i2c_smbus_read_byte_data(fd, LSM9DS1_WHO_AM_I));
    /* check we are who we should be */
    if (i2c_smbus_read_byte_data(fd, LSM9DS1_WHO_AM_I) != LSM9DS1_DEV_ID_AG) {
        printf("%s\n", "who_am_i error");
        close(fd);
        exit(1);
    }
    /* Power down the device */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);


    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);


    // i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);


    /* Wait until the measurement is complete */

    // do {
    //     usleep(25 * 1000); /* 25 milliseconds */
    //     status = i2c_smbus_read_byte_data(fd, CTRL_REG1);
    // } while (status != 0);
    usleep(25000);


    uint8_t out_x_l_xl = i2c_smbus_read_byte_data(fd, OUT_X_L_XL);
    uint8_t out_x_h_xl = i2c_smbus_read_byte_data(fd, OUT_X_H_XL);
    uint8_t out_y_l_xl = i2c_smbus_read_byte_data(fd, OUT_Y_L_XL);
    uint8_t out_y_h_xl = i2c_smbus_read_byte_data(fd, OUT_Y_H_XL);
    // uint8_t out_z_l_xl = i2c_smbus_read_byte_data(fd, OUT_Z_L_XL);
    // uint8_t out_z_h_xl = i2c_smbus_read_byte_data(fd, OUT_Z_H_XL);

    int16_t out_x = out_x_h_xl << 8 | out_x_l_xl;
    int16_t out_y = out_y_h_xl << 8 | out_y_l_xl;
    // int16_t out_z = out_z_h_xl << 8 | out_z_l_xl;

    xl[0] = (out_x * 2) / 200000.0;  // Scale to g
    xl[1] = (out_y * 2) / 200000.0;  // Scale to g

    

    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    close(fd);
}




