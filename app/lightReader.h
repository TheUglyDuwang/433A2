#ifndef LIGHT_READER_H
#define LIGHT_READER_H


// Initializes the sampler
void Sampler_init(void);

// Cleans up the sampler
void Sampler_cleanup(void);

// Moves current data to history
void Sampler_moveCurrentDataToHistory(void);

// Gets the size of the history
int Sampler_getHistorySize(void);

// Gets the history of samples and returns the size
double* Sampler_getHistory(int *size);

// Calculates and returns the average reading from the history
double Sampler_getAverageReading(void);

// Returns the total number of samples taken
long long Sampler_getNumSamplesTaken(void);

// Returns the number of dips detected in the history
int Sampler_getDips(void);

// Outputs the first line of data to the terminal
void terminal_OutputLine1(void);

// Outputs the second line of data to the terminal
void terminal_OutputLine2(void);

#endif /* LIGHT_READER_H */
