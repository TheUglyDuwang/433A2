//this will be used by the thread that reads the light sensor inputs
//current file path /app/lightReader.c
//using function from /hal/sensor.h
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> 

// Declaration of the function from hal/sensor.h
extern int getVoltage1Reading();

void *lightSamplingThread(void *arg) {
    while (true) {
        int voltage = getVoltage1Reading();
        // Process the voltage reading as needed
        
        usleep(1000); // Add a semicolon here
    }
    return NULL;
}

bool lightSampling(bool activate) {
    static pthread_t sampleLightThread; // Declare the thread variable as static to preserve its value between function calls

    if (activate) {
        pthread_create(&sampleLightThread, NULL, lightSamplingThread, NULL);
    } else {
        pthread_join(sampleLightThread, NULL); // Use sampleLightThread, not lightSampling
    }

    return true; // Return true when activated
}
