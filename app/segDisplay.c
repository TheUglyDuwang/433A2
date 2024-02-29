#include "./hal/digitDisplay.c"
#include "segDisplay.h"
#include <stdatomic.h>

#define I2CDRV_LINUX_BUS0 "/dev/i2c-0"
#define I2CDRV_LINUX_BUS1 "/dev/i2c-1"
#define I2CDRV_LINUX_BUS2 "/dev/i2c-2"

#define I2C_DEVICE_ADDRESS 0x20

#define REG_DIRA 0x02
#define REG_DIRB 0x03
#define REG_OUTA 0x00
#define REG_OUTB 0x01

#define LEFT_DIGIT_FILE_PATH "/sys/class/gpio/gpio61/value"
#define RIGHT_DIGIT_FILE_PATH "/sys/class/gpio/gpio44/value"

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

void digitThread(void){
    static pthread_t digitThread;
    static int buffer = 0;

    pthread_create(&digitThread, NULL, digitLoop, (void *)&buffer);
    pthread_mutex_init(&buffer->mutex, NULL);

}

_Atomic int numOfBlinks;

static void flipDigit(char* filePath, int onOff){
    FILE* digit = fopen(filePath, "w");
    fprintf(digit, "%d", onOff);
    fclose(digit);
}

static void sleepForMs(long long delayInMs){
    const long long NS_PER_MS = 1000 * 1000;
    const long long NS_PER_SECOND = 1000000000;
    long long delayNs = delayInMs * NS_PER_MS;
    int seconds = delayNs / NS_PER_SECOND;
    int nanoseconds = delayNs % NS_PER_SECOND;
    struct timespec reqDelay = {seconds, nanoseconds};
    nanosleep(&reqDelay, (struct timespec *) NULL);
}

void digitLoop(void){
    numOfBlinks = 0;
    int i2cFileDesc =  initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    unsigned char upperCodes[10] = {0xe1, 0x00, 0xc3, 0x01, 0x62, 0x63, 0xa3, 0x01, 0xe3, 0x63}; //register 0x01
    unsigned char lowerCodes[10] = {0xd0, 0xc0, 0x98, 0xd8, 0xc8, 0x58, 0x58, 0xc0, 0xd8, 0xd8}; //register 0x00
    bool cont = true;
    int buffRead = 0;
    unsigned char leftLowerDigit;
    unsigned char leftUpperDigit;
    unsigned char rightUpperDigit;
    unsigned char rightLowerDigit;
    resetDisplay(i2cFileDesc);
    while(cont){
        
        int left = numOfBlinks/10;
        int right = numOfBlinks%10;
        if(buffRead == -1){//exit condition
            cont = false;
        }else{
            leftUpperDigit = upperCodes[left];
            leftLowerDigit = lowerCodes[left];

            rightUpperDigit = upperCodes[right];
            rightLowerDigit = lowerCodes[right];

            flipDigit(RIGHT_DIGIT_FILE_PATH, 0);
            showDigit(leftUpperDigit, leftLowerDigit, i2cFileDesc);
            flipDigit(LEFT_DIGIT_FILE_PATH, 1);
            sleepForMs(30);

            flipDigit(LEFT_DIGIT_FILE_PATH, 0);
            showDigit(rightUpperDigit, rightLowerDigit, i2cFileDesc);
            flipDigit(RIGHT_DIGIT_FILE_PATH, 1);
            sleepForMs(30);

        }
    }

}