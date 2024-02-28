#include "sampler.h"
#include "sensor.h"

#define VOLTAGE_MAX 4095
#define REFERENCE_VOLTAGE 1.8
#define HISTORY_SIZE 1000
#define PACKET_SIZE 1500

typedef struct {
    _Atomic double samples[HISTORY_SIZE];
    _Atomic int head;
    _Atomic int size;
    pthread_mutex_t mutex; // Mutex for thread safety
} history;

void Sampler_init(void){
    return;
}


void Sampler_cleanup(void){
    return;
}

void Sampler_moveCurrentDataToHistory(void){
    return;
}

int Sampler_getHistorySize(void){
    return 1;
}

double* Sampler_getHistory(int *size){
    return 1;
}

double Sampler_getAverageReading(void){
    return 1;
}

long long Sampler_getNumSamplesTaken(void){
    return 1;
}
