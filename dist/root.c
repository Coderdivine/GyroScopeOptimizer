#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/ioctl.h>
#include <linux/i2c-dev.h>

// Define the I2C address of the device
#define I2C_ADDR 0x68

// Register addresses of the Gyroscope and Accelerometer
#define GYRO_XOUT_H 0x43
#define ACCEL_XOUT_H 0x3B

int file;

void init_i2c()
{
    char *filename = "/dev/i2c-1";
    if ((file = open(filename, O_RDWR)) < 0) {
        printf("Failed to open I2C bus\n");
        exit(1);
    }
    if (ioctl(file, I2C_SLAVE, I2C_ADDR) < 0) {
        printf("Failed to select I2C device\n");
        exit(1);
    }
}

int16_t read_word_2c(int addr)
{
    uint8_t high, low;
    high = low = 0;
    high = i2c_smbus_read_byte_data(file, addr);
    low = i2c_smbus_read_byte_data(file, addr+1);
    int16_t val = (high << 8) + low;
    if (val >= 0x8000)
        val = -(65536 - val);
    return val;
}

void ReadGyroScope()
{
    int16_t gyro_xout, gyro_yout, gyro_zout;
    // Read Gyroscope values
    gyro_xout = read_word_2c(GYRO_XOUT_H);
    gyro_yout = read_word_2c(GYRO_XOUT_H + 2);
    gyro_zout = read_word_2c(GYRO_XOUT_H + 4);
    printf("Gyroscope: x=%d y=%d z=%d\n", gyro_xout, gyro_yout, gyro_zout);
}

void ReadAccelerometer()
{
    int16_t accel_xout, accel_yout, accel_zout;
     // Read Accelerometer values
    accel_xout = read_word_2c(ACCEL_XOUT_H);
    accel_yout = read_word_2c(ACCEL_XOUT_H + 2);
    accel_zout = read_word_2c(ACCEL_XOUT_H + 4);
    printf("Accelerometer: x=%d y=%d z=%d\n", accel_xout, accel_yout, accel_zout);
}


int main()
{

    init_i2c();
    ReadGyroScope();
    ReadAccelerometer();
    close(file);
    return 0;
}
