#include "lightReader.h"
#include "sensor.h"
#include "periodTimer.h"


#define VOLTAGE_MAX 4095
#define REFERENCE_VOLTAGE 1.8
#define PACKET_SIZE 1500
#define PORT 12345

typedef struct sampler {
    int size;
    double samples[1000]; // array to store samples
    double min;
    double max;
} sampler;

static int isInit;
static int sampling;
static pthread_t thread_id;
static long long samplesTaken;
static sampler reader; // declare reader as a global variable
static sampler history;

void copyArray(double source[], double destination[], int size) {
    for (int i = 0; i < size; i++) {
        destination[i] = source[i];
    }
}

void Sampler_moveCurrentDataToHistory(void) {
    history.size = reader.size;
    copyArray(reader.samples, history.samples, history.size);
    history.min = reader.min;
    history.max = reader.max;
    reader.min = 1000;
    reader.max = 0;
    reader.size = 0;
    return;
}

static long long getTimeInMs(void) {
    struct timespec spec;
    clock_gettime(CLOCK_REALTIME, &spec);
    long long seconds = spec.tv_sec;
    long long nanoSeconds = spec.tv_nsec;
    long long milliSeconds = seconds * 1000 + nanoSeconds / 1000000;
    return milliSeconds;
}

long long Sampler_getNumSamplesTaken(void) {
    return samplesTaken;
}

double Sampler_getAverageReading(void) {
    double avg = 0;
    for (int i = 0; i < history.size; i++) {
        avg = avg + history.samples[i];
    }
    avg = avg / history.size;
    return floor(avg * 1000) / 1000;
}

int Sampler_getHistorySize(void) {
    return history.size;
}

double* Sampler_getHistory(int *size) {
    *size = history.size;
    return history.samples;
}

int Sampler_getDips(void) {
    double avg = Sampler_getAverageReading();
    int dips = 0;
    int canCountDip = 1;
    int size = Sampler_getHistorySize();

    for (int i = 0; i < size; i++) {
        if ((avg - history.samples[i]) < 0.1 && canCountDip) {
            dips++;
            canCountDip = 0;
        } else if ((avg - history.samples[i]) >= 0.07 && !canCountDip) {
            canCountDip = 1;
        }
    }

    return dips;
}

void terminal_OutputLine1(void) {
    printf("Smpl ms[%f, %f] %f / %d\n", history.min, history.max, Sampler_getAverageReading(), history.size);
    return;
}

void terminal_OutputLine2(void) {
    if (history.size < 20) {
        for (int i = 0; i < history.size; i++) {
            printf("%d : %f\n", i + 1, history.samples[i]);
        }
    } else {
        int count = floor(history.size);
        for (int i = 0; i < history.size; i = i + count) {
            printf("%d : %f\n", i + 1, history.samples[i]);
        }
    }
    return;
}

void *Sampler_thread(void* arg) {
    int prevTime = getTimeInMs();
    int curTime = getTimeInMs();
    int prevTimePerSample;
    int curTimePerSample;
    int timeBetweenSamples;
    reader.min = 1000;
    while (isInit) {
        prevTimePerSample = getTimeInMs();
        if ((curTime - prevTime) > 1000) {
            Sampler_moveCurrentDataToHistory();
            prevTime = getTimeInMs();
            terminal_OutputLine1();
            terminal_OutputLine2();
        }
        int value = getVoltage1Reading();
        double voltage = floor(((value / (double)VOLTAGE_MAX) * REFERENCE_VOLTAGE) * 1000) / 1000;
        reader.samples[reader.size++] = voltage;
        samplesTaken++;
        curTime = getTimeInMs();
        curTimePerSample = getTimeInMs();
        timeBetweenSamples = curTimePerSample - prevTimePerSample;
        if (timeBetweenSamples < reader.min) {
            reader.min = timeBetweenSamples;
        }
        if (timeBetweenSamples > reader.max) {
            reader.max = timeBetweenSamples;
        }
        usleep(1000); // sleep for 1 millisecond
    }
    return NULL;
}

void Sampler_init(void) {
    isInit = 1;
    samplesTaken = 0;
    reader.size = 0;
    history.size = 0;
    if (pthread_create(&thread_id, NULL, Sampler_thread, NULL) != 0) {
        perror("pthread_create");
        return;
    }
}

void Sampler_cleanup(void) {
    isInit = 0;
    if (pthread_join(thread_id, NULL) != 0) {
        perror("pthread_join");
        return;
    }
    // Add any necessary cleanup here, such as freeing dynamically allocated memory
}
