//this has the main functiuon that splits off into the other threads.
//Afterwards, it will handle printing to the screen and taking the exit input
#include "main.h"
#include "lightReader.h"
#include "blink.h"
#include "network.h"
#include "segDisplay.h"


void help(void) {
    printf("Accepted commands:\n");
    printf("help: Return a brief summary/list of supported commands.\n");
    printf("?: Same as help\n");
    printf("count: Return the total number of light samples take so far (may be huge, like > 10 billion).\n");
    printf("length: Return how many samples were captured during the previous second.\n");
    printf("dips: Return how many dips were detected during the previous second’s samples.\n");
    printf("history: Return all the data samples from the previous second.");
    printf("stop: Exit the program.\n");
    printf("<enter>: Runs the last command.\n");
}
void history(void) {
    int size = 0;
    double *samples = Sampler_getHistory(&size);
    if (samples == NULL) {
        printf("Error: Failed to retrieve sample history.\n");
        return;
    }
    printf("Data samples from the previous second:\n");
    for (int i = 0; i < size; i++) {
        printf("%.3f, ", samples[i]);
        if ((i + 1) % 10 == 0) {
            printf("\n");
        }
    }
    printf("\n");
    free(samples); // Free the allocated memory
}

int main(void) {
    char command[20];
    char previous_command[20] = "";
    while (1) {
        printf("Enter command: ");
        fgets(command, sizeof(command), stdin);
        command[strcspn(command, "\n")] = '\0'; // remove trailing newline
        
        // If the command is blank, repeat the previous command
        if (strcmp(command, "") == 0) {
            if (strcmp(previous_command, "") == 0) {
                printf("Unknown command: %s\n", command);
            } else {
                strcpy(command, previous_command);
            }
        }
        
        // Store the current command as the previous command
        strcpy(previous_command, command);
        
        if (strcmp(command, "help") == 0 || strcmp(command, "?") == 0) {
            help();
        } else if (strcmp(command, "count") == 0) {
            printf("Total samples taken: %llu\n", Sampler_getNumSamplesTaken());
        } else if (strcmp(command, "length") == 0) {
            printf("Samples taken in the last second: %d\n", Sampler_getHistorySize());
        } else if (strcmp(command, "dips") == 0) {
            printf("Samples taken in the last second: %d\n", Sampler_getDips());
        } else if (strcmp(command, "history") == 0) {
            history();
        } else if (strcmp(command, "stop") == 0) {
            Sampler_cleanup();
        } else if (strcmp(command, "") == 0) {
            printf("Unknown command: %s\n", command);
        }
    }
    return 0;
}
