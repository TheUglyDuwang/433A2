#include "digitDisplay.h"

#ifndef DIGIT_DISPLAY_C
#define DIGIT_DISPLAY_C


int initI2cBus(char* bus, int address){
    int i2cFileDesc = open(bus, O_RDWR);

    int result = ioctl(i2cFileDesc, I2C_SLAVE, address);
    if(result < 0){
        exit(-1);
    }

    return i2cFileDesc;
}

void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value){
    unsigned char buff[2];
    buff[0] = regAddr;
    buff[1] = value;
    int res = write(i2cFileDesc, buff, 2);
    if(res != 2){
        exit(-1);
    }
}

#endif
