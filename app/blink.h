//header file for blink.c
#ifndef BLINK_H
#define BLINK_H
#include <cstdio>

//main loop for blinking
bool mainBlinkloop(void);

//turns on the light, allows it to blink
bool enableLight(void);

//turns off light, stops it from blinking
bool disableLight(void);

//sets the looping value to false to stop the blinking
void stopLooping(void);

#endif