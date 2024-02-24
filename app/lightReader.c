#include "lightReader.h"

#define VOLTAGE_MAX 4095
#define REFERENCE_VOLTAGE 1.8
#define HISTORY_SIZE 1000
#define PACKET_SIZE 1500

int lastCommand = NULL;
// Define the CircularBuffer struct
typedef struct {
    double samples[HISTORY_SIZE];
    int head;
    int size;
    pthread_mutex_t mutex; // Mutex for thread safety
} CircularBuffer;

// Function to create a CircularBuffer
CircularBuffer *createCircularBuffer(){
    CircularBuffer *buffer = (CircularBuffer *)malloc(sizeof(CircularBuffer));
    if(buffer == NULL){ // Correct the comparison operator here
        return NULL;
    }
    buffer->head = 0;
    buffer->size = 0;
    if(pthread_mutex_init(&buffer->mutex, NULL) != 0){
        free(buffer); // Free memory if mutex initialization fails
        return NULL;
    }
    return buffer;
}

// Function to destroy a CircularBuffer
void destroyCircularBuffer(CircularBuffer *buffer){
    pthread_mutex_destroy(&buffer->mutex);
    free(buffer);
}

extern int getVoltage1Reading(); // Declaration of external function

// Function to print the history stored in the circular buffer
void printHistory(CircularBuffer *buffer){
    int size = buffer -> size;
    int head = buffer->head + 1;
    int index = 0;
    int remainder = 0;
    int lines = 0;

    double ar[size];

    for(int i = head ; i < size; i ++){
        ar[index] = buffer ->samples[i];
        index++;
    }

    for(int i = 0; i< head; i++ ){
        ar[index] = buffer->samples[i];
        index++;
    }

    if(size % 10 != 0){
        remainder = size % 10;
        size = size - remainder;
    }

    lines = size / 10;

    for(int i = 0; i < lines; i ++){ //prints outputs and then \n without having white space for the 10th number in the line
        index = i*10;
        for(int k = 0; k < 10; k++){
            printf("%f,", ar[index+k]);
            if(k != 9){
                printf(" ");
            }
        }
        printf("\n");
    }
    index = (lines+1)*10; // Added missing semicolon
    for(int i = 0; i<remainder; i++ ){ //off chance there are not exactly 10 outputs a line, it will print the last line to the nth input and \n
        printf("%f,", ar[index+i]);
        if(i !=(remainder-1)){
            printf(" ");
        }
    }
}

// Function to add a voltage reading to the circular buffer
void addToHistory(double voltage, int totalSamplesTaken, CircularBuffer *buffer) {
    pthread_mutex_lock(&buffer->mutex); // Lock mutex before accessing buffer
    int index = totalSamplesTaken % HISTORY_SIZE;
    buffer->samples[index] = voltage;
    buffer->head = index;
    buffer->size++;
    pthread_mutex_unlock(&buffer->mutex); // Unlock mutex after accessing buffer
}

// Function to be executed by the sampling thread
void *lightSamplingThread(void *arg) {
    int *totalSamplesTaken = (int *)arg; // Cast the argument back to int pointer
    CircularBuffer *buffer = createCircularBuffer();

    while (true) {
        int value = getVoltage1Reading();
        double voltage = floor(((value / (double)VOLTAGE_MAX) * REFERENCE_VOLTAGE) * 1000) / 1000;
        addToHistory(voltage, *totalSamplesTaken, buffer);
        (*totalSamplesTaken)++;
        usleep(1000);
    }
    return NULL;
}

// Function to start/stop sampling or print history based on command
int lightSamplingCommand(int command) {
    static pthread_t sampleLightThread; // Declare the thread variable as static to preserve its value between function calls
    static CircularBuffer *circularBuffer = NULL; // Initialize to NULL
    static int totalSamplesTaken = 0;

    switch(command) {
        case 1: // Start sampling
            if (circularBuffer == NULL) {
                circularBuffer = createCircularBuffer(); // Create circular buffer if not already created
                if (circularBuffer == NULL) {
                    return -1; // Return -1 to indicate failure
                }
            }
            pthread_create(&sampleLightThread, NULL, lightSamplingThread, (void *)&totalSamplesTaken);
            break;
        case 0: // Stop sampling
            if (circularBuffer != NULL) {
                pthread_join(sampleLightThread, NULL);
                destroyCircularBuffer(circularBuffer);
                circularBuffer = NULL; // Set to NULL after destroying to indicate buffer is no longer valid
            }
            break;
        case 3: // Print history
            if (circularBuffer != NULL) {
                printHistory(circularBuffer);
            }
            break;
        case 4:
            printf("count\n");
            printf("%d\n", totalSamplesTaken);
            break;
        case 5:
            printf("length\n");
            printf("%d\n", circularBuffer->size);
            break;
        case 6:
            printf("ligthSamplingCommand is like the main function of lightReader:\n");
            printf("stop -> destroys/frees up sampling (used for clean up)\n");
            printf("command 1 -> initializes the samlping (used at the start of the program)\n");
            printf("history -> prints the history for the last 1 second\n");
            printf("count -> prints the total count of samples\n");
            printf("length -> prints the number of samples in the last second\n");
            printf("help -> help function\n");
            printf("<enter> -> A blank input. Repeats last command. if no last command then error unkown command\n");
            printf("");
            break;
        default:
            if(lastCommand == NULL){
                printf("ERROR: unknown command\n");
            }else{
                lightSamplingCommand(lastCommand);
            }
            break;
        if(command != NULL){
            lastCommand = command;
        }
        command = lastCommand;
    }

    return command;
}
