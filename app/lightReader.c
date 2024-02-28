#include "lightReader.h"
#include "sensor.h"
#include <pthread.h>
#include <math.h> // for floor function

#define VOLTAGE_MAX 4095
#define REFERENCE_VOLTAGE 1.8
#define PACKET_SIZE 1500
#define PORT 12345

typedef struct sampler {
    int size;
    double samples[1000]; // array to store samples
} sampler;

static int isInit;
static int sampling;
static pthread_t thread_id;
static long long samplesTaken;
static sampler reader; // declare reader as a global variable
static sampler history;
static history[1000];
static long long totalVoltage;

static long long getTimeInMs(void){
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
    return milliSeconds;
}

void Sampler_moveCurrentDataToHistory(void){
    history.size = reader.size;
    copyArray(reader.samples, history.samples, history.size);
    reader.size = 0;
    return;
}

void *Sampler_thread(void* arg){
    int prevTime = getTimeInMs();
    int curTime = getTimeInMs();
    while(isInit){
        if( (curTime - prevTime) > 1000 ){
            Sampler_moveCurrentDataToHistory();
            prevTime = getTimeInMs();
        }
        int value = getVoltage1Reading();
        double voltage = floor(((value / (double)VOLTAGE_MAX) * REFERENCE_VOLTAGE) * 1000) / 1000;
        reader.samples[reader.size++] = voltage;
        samplesTaken++;
        curTime = getTimeInMs();
        usleep(1000); // sleep for 1 millisecond
    }
    return NULL;
}

void Sampler_init(void){
    isInit = 1;
    samplesTaken = 0;
    reader.size = 0;
    history.size = 0;
    totalVoltage = 0;
    if (pthread_create(&thread_id, NULL, Sampler_thread, NULL) != 0) {
        perror("pthread_create");
        return;
    }
}

void Sampler_cleanup(void){
    isInit = 0;
    if (pthread_join(thread_id, NULL) != 0) {
        perror("pthread_join");
        return;
    }
}

long long Sampler_getNumSamplesTaken(void){
    return samplesTaken;
}

double Sampler_getAverageReading(void){
    double avg = 0;
    for(int i = 0; i < history.size; i++){
        avg = avg + history.sample[i];
    }
    avg = avg / history.size;
    return floor( avg *1000)/ 1000;
}

int Sampler_getHistorySize(void){
    return history.size;
}

double* Sampler_getHistory(int *size){
    if(history.size = size){
        return history.sample;
    }
    else{
        perror("issue getting history");
        return 0.00;
    }
}

int Sampler_getDips( void ){
    double avg = Sampler_getAverageReading();
    int dips = 0;
    int canCountDip = 1;
    int size = Sampler_getHistorySize();

    for(int i = 0; i < size; i++){
        if ( (avg - ar[i] ) < 0.1 && canCountDip){
            dips++;
            canCountDip = 0;
        } else if ( (avg - ar[i] ) >= 0.07 && !canCountDip ) {
            canCountDip = 1;
        }
    }

    return dips;
}