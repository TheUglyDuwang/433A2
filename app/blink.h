//header file for blink.c

#include <cstdio>
#ifndef BLINK_H
#define BLINK_H

//main loop for blinking
bool mainBlinkloop();

//uses the led file in the HAL folder to adjust the cycle
void setCycle(int cycle, FILE* file);

//turns on the light, allows it to blink
bool enableLight(void);

//turns off light, stops it from blinking
bool disableLight(void);

#endif