//this will be used by the thread that involves blinking the light

#ifndef BLINK_C
#define BLINK_C

#include "blink.h"
#include "led.h"

//main loop for blinking
bool mainBlinkloop(bool isLooping){
    enableLight();

    int newCycle = 0; //this will store the new cycle
    FILE* cycle = fopen("/dev/bone/pwm/1/b/cycle", "w");


    while(isLooping){//isLooping can be changed via main thread when exiting
    
    //get potentiometer here, get into newCycle

        setCycle(newCycle, cycle);

    }

    disableLight();

    return true;
}

//uses the led file in the HAL folder to adjust the cycle
void setCycle(int cycle, FILE* file){
    char cycleChar[sizeof(cycle)*8+1];
    itoa(cycle, cycleChar, DECIMAL);//converting in

    int inputWritten = fprintf(file, cycleChar);//sets the cycle to the char-ized version of cycle
    if(inputWritten <= 0){
        printf("SOMETHING IS WRONG\n");
    }
}

//turns on the light, allows it to blink
bool enableLight(void){
    FILE enableFile = fopen("/dev/bone/pwm/1/b/enable", "w");//file for light enabling
    int inputWritten = fprintf(enableFile, "1");
    if(inputWritten <= 0){return false;}
    fclose(enableFile);
    return true;
}

//turns off light, stops it from blinking
bool disableLight(void){
    FILE enableFile = fopen("/dev/bone/pwm/1/b/enable", "w");
    int inputWritten = fprintf(enableFile, "0");
    if(inputWritten <= 0){return false;}
    fclose(enableFile);
    return true;
}

#endif