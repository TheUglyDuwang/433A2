//this will be used by the thread that involves blinking the light

#ifndef BLINK_C
#define BLINK_C

#include "blink.h"
#include "led.h"
#include "potentiometer.h"
#include <stdatomic.h>
#include <pthread.h>


static _Atomic bool isLooping;
static pthread_t thread_id;

void lightThread(void){
    if (pthread_create(&thread_id, NULL, mainBlinkloop, NULL) != 0) {
        perror("pthread_create");
        return;
    }
}

//main loop for blinking
void mainBlinkloop(void){
    enableLight();
    isLooping = true;
    int newCycle = 0; //this will store the new cycle
    FILE* cycle; 


    while(isLooping){//isLooping can be changed via main thread when exiting
    
        newCycle = getVoltage0Reading();
        cycle = fopen("/dev/bone/pwm/0/b/period", "w");
        LED_H.setCycle(cycle, newCycle);
        fclose(cycle);

    }

    disableLight();

    
}

//turns on the light, allows it to blink
bool enableLight(void){
    FILE enableFile = fopen("/dev/bone/pwm/0/b/enable", "w");//file for light enabling
    int inputWritten = fprintf(enableFile, "1");
    if(inputWritten <= 0){return false;}
    fclose(enableFile);
    return true;
}

//turns off light, stops it from blinking
bool disableLight(void){
    FILE enableFile = fopen("/dev/bone/pwm/0/b/enable", "w");
    int inputWritten = fprintf(enableFile, "0");
    if(inputWritten <= 0){return false;}
    fclose(enableFile);
    return true;
}

void stopLooping(void){
    isLooping = false;
}

#endif