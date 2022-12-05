/*
 * FILE: scheduler.c
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Contains the driver code for the scheduling simulator and setups all
 *          the threads to run the 6 scheduling algorithms concurrently
 * REFERENCE: None
 * LAST MOD: 12/05/22
 * COMMENTS: None
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include <pthread.h>
#include "dataExtract.h"
#include "algorithms.h"

int main()
{
    pthread_t A, B, C, D, E, F;
    char filename[11]; // max filename size assumed to be 10 -> 11 with null terminator
    int *schedulingDataArr;
    int arrSize;
    int *arrSizePtr;
    bool readSucess;
    bool run = true;
    arrSize = 0;
    arrSizePtr = &arrSize;
    threadData *sharedData = malloc(sizeof(*sharedData));

    // initialize mutex and condition variables before creating the threads
    pthread_mutex_init(&sharedData->accessMutex, NULL);
    pthread_cond_init(&sharedData->runCondition, NULL);

    // initialize all threads
    // NOTE: Needs refactoring
    // Possible fix: Store an array of function pointers and iterate over it to to create each thread and run the function.
    if (pthread_create(&A, NULL, &fcfs, sharedData) != 0)
    {
        perror("Failed to create thread 'A':\n");
    }
    if (pthread_create(&B, NULL, &sstf, sharedData) != 0)
    {
        perror("Failed to create thread 'B':\n");
    }
    if (pthread_create(&C, NULL, &scan, sharedData) != 0)
    {
        perror("Failed to create thread 'C':\n");
    }
    if (pthread_create(&D, NULL, &look, sharedData) != 0)
    {
        perror("Failed to create thread 'D':\n");
    }
    if (pthread_create(&E, NULL, &cScan, sharedData) != 0)
    {
        perror("Failed to create thread 'E':\n");
    }
    if (pthread_create(&F, NULL, &cLook, sharedData) != 0)
    {
        perror("Failed to create thread 'F':\n");
    }

    printf("Disk Scheduler Simulator\n");
    while (run)
    {

        getInput(filename);
        if (strcasecmp(filename, "QUIT"))
        {

            readSucess = readFile(filename, &schedulingDataArr, arrSizePtr);
            if (*arrSizePtr > METADATA_MIN && readSucess)
            {
                sharedData->lenofArr = *arrSizePtr;
                sharedData->buffer1 = schedulingDataArr;

                pthread_cond_signal(&sharedData->runCondition);

                // wait for the child threads to finish before printing the results to the screen.
                pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

                // print the results (consumer)
                printf("FCFS: %d\n", sharedData->buffer2[0]);
                printf("SSTF: %d\n", sharedData->buffer2[1]);
                printf("SCAN: %d\n", sharedData->buffer2[2]);
                printf("C-SCAN: %d\n", sharedData->buffer2[3]);
                printf("LOOK: %d\n", sharedData->buffer2[4]);
                printf("C-LOOK: %d\n", sharedData->buffer2[5]);
                printf("\n================\n");

                free(schedulingDataArr);
                sharedData->buffer1 = 0;
            }
            // goes in only if read is success but insufficent data in file
            else if (readSucess)
            {
                displayErr(filename);
                free(schedulingDataArr); // free's array currently created array
            }
        }
        else
        {
            // if quit send cancellation request to all threads.
            // if successful prints the thread id.
            printf("\n");
            if (pthread_cancel(A) == 0)
                printf("%ld has terminated\n", A);
            if (pthread_cancel(B) == 0)
                printf("%ld has terminated\n", B);
            if (pthread_cancel(C) == 0)
                printf("%ld has terminated\n", C);
            if (pthread_cancel(D) == 0)
                printf("%ld has terminated\n", D);
            if (pthread_cancel(E) == 0)
                printf("%ld has terminated\n", E);
            if (pthread_cancel(F) == 0)
                printf("%ld has terminated\n", F);

            free(sharedData);
            pthread_mutex_destroy(&sharedData->accessMutex);
            pthread_cond_destroy(&sharedData->runCondition);

            run = false;
        }
    }
    return 0;
}
