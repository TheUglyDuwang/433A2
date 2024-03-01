#include "./hal/digitDisplay.c"
#include "segDisplay.h"
#include <stdatomic.h>
#include "lightReader.h"

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

static _Atomic int numOfBlinks;
static pthread_t thread_id;
static pthread_t thread2;

void startDisplay(void){
    if (pthread_create(&thread_id, NULL, digitLoop, NULL) != 0) {
        perror("pthread_create");
        return;
    }
    startDips();
}

void startDips(void){
    if (pthread_create(&thread2, NULL, diploop, NULL) != 0) {
        perror("pthread_create");
        return;
    }
}

void diploop(void){
    setBlinkNum(Sampler_getDips());
    sleep(1);
}

static void setBlinkNum(int newNum){
    numOfBlinks = newNum;
}

//provided by prof
static void runCommand(char* command){
 // Execute the shell command (output into pipe)
    FILE *pipe = popen(command, "r");
 // Ignore output of the command; but consume it
 // so we don't get an error when closing the pipe.
 char buffer[1024];
    while (!feof(pipe) && !ferror(pipe)) {
        if (fgets(buffer, sizeof(buffer), pipe) == NULL)
            break;
 // printf("--> %s", buffer); // Uncomment for debugging
    }
 // Get the exit code from the pipe; non-zero is an error:
    int exitCode = WEXITSTATUS(pclose(pipe));
    if (exitCode != 0) {
        runCommand(command);//since this will only be used to initialize the i2c pins, it should only call an error if they were already exported
                            //which would unexport them, so they need to be exported again
    }
}

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

    pthread_create(&digitThread, NULL, digitLoop, NULL);
}

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
    runCommand("config-pin P9_18 i2c");
    runCommand("config-pin P9_17 i2c");
    numOfBlinks = 0;
    int i2cFileDesc =  initI2cBus(I2CDRV_LINUX_BUS1, I2C_DEVICE_ADDRESS);
    unsigned char upperCodes[10] = {0xe1, 0x00, 0xc3, 0x01, 0x62, 0x63, 0xa3, 0x01, 0xe3, 0x63}; //register 0x01
    unsigned char lowerCodes[10] = {0xd0, 0xc0, 0x98, 0xd8, 0xc8, 0x58, 0x58, 0xc0, 0xd8, 0xd8}; //register 0x00
    bool cont = true;
    unsigned char leftLowerDigit;
    unsigned char leftUpperDigit;
    unsigned char rightUpperDigit;
    unsigned char rightLowerDigit;
    resetDisplay(i2cFileDesc);
    while(cont){
        
        int left = numOfBlinks/10;
        int right = numOfBlinks%10;
        if(numOfBlinks == -1){//exit condition
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