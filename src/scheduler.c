/*
 * FILE: scheduler.c
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Contains the driver code for the scheduling simulator
 * REFERENCE: None
 * LAST MOD: 11/05/22
 * COMMENTS: None
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dataExtract.h"
#include "algorithms.h"

int main()
{
    char filename[11]; // max filename size assumed to be 10 -> 11 with null terminator
    int *schedulingDataArr;
    int arrSize;
    int *arrSizePtr;
    bool readSucess;
    bool run = true;
    arrSize = 0;
    arrSizePtr = &arrSize;

    printf("Disk Scheduler Simulator\n");
    while (run)
    {
        getInput(filename);
        if (strcasecmp(filename, "QUIT"))
        {
            // reading input file
            readSucess = readFile(filename, &schedulingDataArr, arrSizePtr);
            if (*arrSizePtr > METADATA_MIN && readSucess)
            {
                // runs all disk scheduling algorithms
                runAllAlgorithms(schedulingDataArr, *arrSizePtr, filename);
                free(schedulingDataArr);
            }
            // goes in only if read is successful but there is insufficent data in the file
            else if (readSucess)
            {
                displayErr(filename);
                free(schedulingDataArr);
            }
        }
        else
        {
            run = false;
        }
    }
    return 0;
}