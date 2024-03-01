#ifndef SEG_DISPLAY_H
#define SEG_DISPLAY_H


void startDisplay(void);

void startDips(void);

void diploop(void);

//runs a command, given by prof
static void runCommand(char* command);

//resets the display
static void resetDisplay(int i2cFileDesc);

//shows a specific number using topValue and bottomValue as the two halves
static void showDigit(unsigned char topValue, unsigned char bottomValue, int i2cFileDesc);

//turns one digit of the 14-seg display on/off
static void flipDigit(char* filePath, int onOff);

//sleeps for desired ms
static void sleepForMs(long long delayInMs);

//main loop for displaying the digits
void digitLoop(void);

#endif