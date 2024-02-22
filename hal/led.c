//implementation of code used to turn the LED on and off

#include "led.h"

//enables the LED so it can turn on and off
void turnOn(FILE* enable){//can probably turn the argument into a static in this file
    int result = fprintf(enable, "1");
    if(result <= 0){
        printf("ERROR WRITING TO FILE\n");
        exit(-1);
    }
    return;
}

//disables the LED to stop the blinking
void turnOff(FILE* enable){
    int result = fprintf(enable, "0");
    if(result <= 0){
        printf("ERROR WRITING TO FILE\n");
        exit(-1);
    }
    return;
}

//sets the duty cycle of the LED
void setDutyCycle(FILE* cycle, char* newCycle){
        int result = fprintf(cycle, newCycle);
    if(result <= 0){
        printf("ERROR WRITING TO FILE\n");
        exit(-1);
    }
    return;
}