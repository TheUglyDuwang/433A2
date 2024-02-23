//file containing the function prototypes for led.c

#include <cstdio>

#ifndef LED_H
#define LED_H


//enables the LED so it can turn on and off
void turnOn(FILE* enable);

//disables the LED to stop the blinking
void turnOff(FILE* enable);

//sets the duty cycle of the LED
void setDutyCycle(FILE* cycle, char* newCycle);

//sets the cycle of the LED
void setCycle(FILE* cycle, char* newCycle);

#endif