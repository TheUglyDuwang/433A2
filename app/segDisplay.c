#include "./hal/digitDisplay.c"

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x02
#define REG_DIRB 0x03
#define REG_OUTA 0x00
#define REG_OUTB 0x01

static void resetDisplay(int i2cFileDesc){
    writeI2cReg(i2cFileDesc, REG_DIRA, 0x00);
    writeI2cReg(i2cFileDesc, REG_DIRB, 0x00);
    writeI2cReg(i2cFileDesc, REG_OUTA, 0x00);
    writeI2cReg(i2cFileDesc, REG_OUTB, 0x00);
}

static void showDigit(unsigned char topValue, unsigned char bottomValue, int i2cFileDesc){
    writeI2cReg(i2cFileDesc, REG_OUTA, topValue);
    writeI2cReg(i2cFileDesc, REG_OUTB, bottomValue);
}

void digitLoop(bool cont){
    int i2cFileDesc =  initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    unsigned char upperCodes[10] = {0xd0, 0xc0, 0x98, 0x18, 0x48, 0x58, 0xd8, 0x10, 0xd8, 0x58};
    unsigned char lowerCodes[10] = {0xe1, 0x00, 0xc3, 0xe3, 0xa2, 0x23, 0x23, 0xa0, 0xe3, 0xe3};
    resetDisplay(i2cFileDesc);
    while(cont){

        //int left = read from buffer divided by 10?
        //int right = read from buffer mod 10?
        //unsigned char leftUpperDigit = upperCodes[left];
        //unsigned char leftLowerDigit = lowerCodes[left];

        //unsigned char rightUpperDigit = upperCodes[right];
        //unsigned char rightLowerDigit = lowerCodes[right];

        //turn on left digit
        //showDigit(leftUpperDigit, leftLowerDigit, i2cFileDesc);
        //turn off left digit
        //turn on right digit
        //showDigit(rightUpperDigit, rightLowerDigit, i2cFileDesc);
        //turn off right digit
    }

}