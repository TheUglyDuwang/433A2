//this will be used by the thread that reads the light sensor inputs
//current file path /app/lightReader.c
//using function from /hal/sensor.h
#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <stdbool.h> 
#include <math.h>

#define VOLTAGE_MAX 4095
#define REFERENCE_VOLTAGE 1.8
#define HISTORY_SIZE 1000
#define PACKET_SIZE 1500


// Declaration of the function from hal/sensor.h
extern int getVoltage1Reading();

void *addToHistory ( double voltage , int totalSamplesTaken){
    
}

void *lightSamplingThread(void *arg) {
    int totalSamplesTaken = 0;
    while (true) {
        int value = getVoltage1Reading();
        double voltage = floor( ((value/VOLTAGE_MAX) * REFERENCE_VOLTAGE) * 1000 ) / 1000;
        addToHistory(voltage, totalSamplesTaken);
        totalSamplesTaken++;
        // Process the voltage reading as needed
        
        usleep(1000); 
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

    return activate; // Return true when activated
}
