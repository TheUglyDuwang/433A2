//header file for lightReader.c

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h> 


void Sampler_init(void);
void Sampler_cleanup(void);

void Sampler_moveCurrentDataToHistory(void);

int Sampler_getHistorySize(void);

double* Sampler_getHistory(int *size);

double Sampler_getAverageReading(void);

long long Sampler_getNumSamplesTaken(void);


