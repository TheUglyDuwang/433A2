//header file for lightReader.c

#include <stdio.h>
#include <pthread.h>
#include <unistd.h>
#include <math.h>
#include <stdlib.h> 


// ligthSamplingCommand is like the main function of lightReader:
//     command 1 -> initializes the samlping (used at the start of the program)
//     command 0 -> destroys/frees up sampling (used for clean up)
//     command 3 -> prints the history for the last 1 second
//     command 4 -> prints the total count of samples
//     command 5 -> prints the number of samples in the last second
//     command 6 -> help function
//     command 7 -> dip //not implemented

int lightSamplingCommand(int command);