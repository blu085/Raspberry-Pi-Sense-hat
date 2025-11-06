/** @brief sensors readings
 *  @file sensors.c
 *  @since 2025-03-26
 */
//include
#include"sensors.h"

/** main fuction
 * @version ceng153
 * @author Huy Bui
 * @since 2025-04-02
 * @param Void
 * @return int
 */
int main(void)
{
    initscr();
    mvprintw(1,1,"Unit: %LX",ShGetSerial());
    time_t now;
    for(;;)
    {
        now=time(NULL);
        mvprintw(2,1,"%s",ctime(&now));
        ShGetAG();
        mvprintw(3,1,"x:%.3f|y:%.3f\n", xl[0], xl[1]);
        mvprintw(4,1,"Temp (from lps25h) = %.2f°C\n", ShGetTemperature());
        mvprintw(5,1,"Pressure (from lps25h) = %.0f hPa\n", ShGetPressure());
        mvprintw(6,1,"Humidity (from hts221) = %.0f%% rH\n", ShGetHumidity());
        refresh();
        usleep(1000000);
    }
    endwin();
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

/** @Brief getting data from the accelerometer
 *  @since 2024-02-16
 *  @param void
 *  @return double
 */
double ShGetTemperature() //From pressure sensor
{
    int fd = 0;
    uint8_t status = 0;

    uint8_t temp_out_l = 0;
    uint8_t temp_out_h = 0;
    int16_t temp_out = 0;
    double t_c = 0.0;

    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, LPS25H_I2C_ADDR) < 0) {
        perror("Unable to configure i2c slave device");
        close(fd);
        exit(1);
    }

    /* check we are who we should be */
    if (i2c_smbus_read_byte_data(fd, LPS25H_WHO_AM_I) != LPS25H_DEV_ID) {
        printf("%s\n", "who_am_i error");
        close(fd);
        exit(1);
    }

    /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    /* Turn on the pressure sensor analog front end in single shot mode  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);

    /* Run one-shot measurement (temperature and pressure), the set bit will be reset by the
     * sensor itself after execution (self-clearing bit)
     */
    i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);

    /* Wait until the measurement is complete */
    do {
        usleep(25 * 1000); /* 25 milliseconds */
        status = i2c_smbus_read_byte_data(fd, CTRL_REG2);
    } while (status != 0);

    /* Read the temperature measurement (2 bytes to read) */
    temp_out_l = i2c_smbus_read_byte_data(fd, TEMP_OUT_L);
    temp_out_h = i2c_smbus_read_byte_data(fd, TEMP_OUT_H);

    /* make 16 and 24 bit values (using bit shift) */
    temp_out = temp_out_h << 8 | temp_out_l;

    /* calculate output values */
    t_c = 42.5 + (temp_out / 480.0);

    /* Power down the device */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    close(fd);

    /*output */
    return t_c;      //"Temp (from lps25h) = %.2f°C\n"
}

/** Brief
 * @since 2024-02-16
 * @param void
 * @return double
 */
double ShGetPressure() //From pressure sensor
{
    int fd = 0;
    uint8_t status = 0;

    uint8_t press_out_xl = 0;
    uint8_t press_out_l = 0;
    uint8_t press_out_h = 0;
    int32_t press_out = 0;
    double pressure = 0.0;

    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, LPS25H_I2C_ADDR) < 0) {
        perror("Unable to configure i2c slave device");
        close(fd);
        exit(1);
    }

    /* check we are who we should be */
    if (i2c_smbus_read_byte_data(fd, LPS25H_WHO_AM_I) != LPS25H_DEV_ID) {
        printf("%s\n", "who_am_i error");
        close(fd);
        exit(1);
    }

    /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    /* Turn on the pressure sensor analog front end in single shot mode  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);

    /* Run one-shot measurement (temperature and pressure), the set bit will be reset by the
     * sensor itself after execution (self-clearing bit)
     */
    i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);

    /* Wait until the measurement is complete */
    do {
        usleep(25 * 1000); /* 25 milliseconds */
        status = i2c_smbus_read_byte_data(fd, CTRL_REG2);
    } while (status != 0);

    /* Read the pressure measurement (3 bytes to read) */
    press_out_xl = i2c_smbus_read_byte_data(fd, PRESS_OUT_XL);
    press_out_l = i2c_smbus_read_byte_data(fd, PRESS_OUT_L);
    press_out_h = i2c_smbus_read_byte_data(fd, PRESS_OUT_H);

    /* make 16 and 24 bit values (using bit shift) */
    press_out = press_out_h << 16 | press_out_l << 8 | press_out_xl;

    /* calculate output values */
    pressure = press_out / 4096.0;

    /* Power down the device */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    close(fd);

    /*output */
    return pressure; //"Pressure (from lps25h) = %.0f hPa\n"
}

/** Brief
 * @since 2024-02-16
 * @param void
 * @return double
 */
double ShGetHumidity()
{
    int fd = 0;
    uint8_t status = 0;

    /* open i2c comms */
    if ((fd = open(DEV_PATH, O_RDWR)) < 0) {
        perror("Unable to open i2c device");
        exit(1);
    }

    /* configure i2c slave */
    if (ioctl(fd, I2C_SLAVE, HTS221_I2C_ADDR) < 0) {
        perror("Unable to configure i2c slave device");
        close(fd);
        exit(1);
    }

    /* check we are who we should be */
    if (i2c_smbus_read_byte_data(fd, HTS221_WHO_AM_I) != HTS221_DEV_ID) {
        printf("%s\n", "who_am_i error");
        close(fd);
        exit(1);
    }

    /* Power down the device (clean start) */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);

    /* Turn on the humidity sensor analog front end in single shot mode  */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x84);

    /* Run one-shot measurement (temperature and humidity). The set bit will be reset by the
     * sensor itself after execution (self-clearing bit) */
    i2c_smbus_write_byte_data(fd, CTRL_REG2, 0x01);

    /* Wait until the measurement is completed */
    do {
        usleep(25*1000); /* 25 milliseconds */
        status = i2c_smbus_read_byte_data(fd, CTRL_REG2);
    } while (status != 0);

    /* Read calibration relative humidity LSB (ADC) data
     * (humidity calibration x-data for two points)
     */
    uint8_t h0_out_l = i2c_smbus_read_byte_data(fd, H0_T0_OUT_L);
    uint8_t h0_out_h = i2c_smbus_read_byte_data(fd, H0_T0_OUT_H);
    uint8_t h1_out_l = i2c_smbus_read_byte_data(fd, H1_T0_OUT_L);
    uint8_t h1_out_h = i2c_smbus_read_byte_data(fd, H1_T0_OUT_H);

    /* Read relative humidity (% rH) data
     * (humidity calibration y-data for two points)
     */
    uint8_t h0_rh_x2 = i2c_smbus_read_byte_data(fd, H0_rH_x2);
    uint8_t h1_rh_x2 = i2c_smbus_read_byte_data(fd, H1_rH_x2);

    /* make 16 bit values (bit shift)
     * (humidity calibration x-values)
     */
    int16_t H0_T0_OUT = h0_out_h << 8 | h0_out_l;
    int16_t H1_T0_OUT = h1_out_h << 8 | h1_out_l;

    /* Humidity calibration values
     * (humidity calibration y-values)
     */
    double H0_rH = h0_rh_x2 / 2.0;
    double H1_rH = h1_rh_x2 / 2.0;

    /* Solve the linear equasions 'y = mx + c' to give the
     * calibration straight line graphs for humidity
     */
    double h_gradient_m = (H1_rH - H0_rH) / (H1_T0_OUT - H0_T0_OUT);
    double h_intercept_c = H1_rH - (h_gradient_m * H1_T0_OUT);

    /* Read the ambient humidity measurement (2 bytes to read) */
    uint8_t h_t_out_l = i2c_smbus_read_byte_data(fd, H_T_OUT_L);
    uint8_t h_t_out_h = i2c_smbus_read_byte_data(fd, H_T_OUT_H);

    /* make 16 bit value */
    int16_t H_T_OUT = h_t_out_h << 8 | h_t_out_l;

    /* Calculate ambient humidity */
    double H_rH = (h_gradient_m * H_T_OUT) + h_intercept_c;
    
    /* Power down the device */
    i2c_smbus_write_byte_data(fd, CTRL_REG1, 0x00);
    close(fd);

    /* Output */
    return H_rH; //"Humidity (from hts221) = %.0f%% rH\n"
}