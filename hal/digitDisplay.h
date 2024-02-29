#ifndef DIGIT_DISPLAY_H
#define DIGIT_DISPLAY_H

//initializes the i2c bus
int initI2cBus(char* bus, int address);

//writes a value to the i2c register to display a value
void writeI2cReg(int i2cFileDesc, unsigned char regAddr, unsigned char value);

#endif