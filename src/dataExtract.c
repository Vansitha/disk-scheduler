/*
 * FILE: dataExtract.c
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Handles input data from the user
 * REFERENCE: None
 * LAST MOD: 11/05/22
 * COMMENTS: None
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <string.h>
#include "dataExtract.h"

/*
 * NAME: readFile
 * PURPOSE: Reads the input file provided by the user
 * IMPORTS: input filename (filename),
 *          pointer to scheduling request array (dataArrPtr),
 *          pointer to the size of the array (arrSize)
 * EXPORTS: Returns a boolean to the calling function to notify the
 *          status of the read operation.
 * ASSERTIONS:
 *       PRE: None
 *       POST: Scheduling data array and size of the array will be initialized
 *             in the calling function.
 * REMARKS: None
 */
bool readFile(char *fileName, int **dataArrPtr, int *arrSize)

{
    bool readingComplete;
    int line;
    int readValue;
    int reqArrSize;
    int *dataArr;
    int count;
    bool readSucess = false;

    FILE *filePtr = fopen(fileName, "r");

    if (filePtr == NULL)
    {
        perror("\nUnable to open file");
        printf("\n================\n");
        readSucess = false;
    }
    else
    {
        readingComplete = false;

        // get the required size for the dynamic array
        reqArrSize = getArraySize(fileName);

        // create the malloc array
        dataArr = (int *)malloc(sizeof(int *) * (reqArrSize - 1));

        // read operation
        count = 0;
        while (!readingComplete)
        {
            line = fscanf(filePtr, "%d", &readValue);
            dataArr[count] = readValue;
            count++;

            if (line == EOF)
            {
                readingComplete = true;
                readSucess = true;
            }
        }

        if (ferror(filePtr))
        {
            perror("\nError Occurred while reading file");
            readSucess = false;
        }

        fclose(filePtr);
    }

    /*
    Initialize the pointers in the main function with the data extracted from the file
    through the pointer parameters.
    */
    *dataArrPtr = dataArr;
    *arrSize = reqArrSize;

    return readSucess;
}

/*
 * NAME: getArraySize
 * PURPOSE: Get the total number of meta data in the input file to
 *          create the scheduling request array
 * IMPORTS: filename
 * EXPORTS: Total number of meta data in the input file
 * ASSERTIONS:
 *      PRE: Assumes that there are no errors when reading and the file already
 *           exists the calling function should handle any errors before calling
 *           this function.
 *      POST: None
 * REMARKS: None
 */
int getArraySize(char *fileName)
{
    FILE *filePtr = fopen(fileName, "r");

    int line;
    int value;
    int size = 0;
    bool run = true;

    while (run)
    {
        line = fscanf(filePtr, "%d", &value);
        if (line != EOF)
        {
            size++;
        }
        else
        {
            run = false;
        }
    }
    fclose(filePtr);

    return size;
}

/*
 * NAME: getInput
 * PURPOSE: Get input file name as a string from the user
 * IMPORTS: Pointer to a character array in the calling function
 * EXPORTS: None
 * ASSERTIONS:
 *      PRE: Create a character array with the length of 11 in the calling
 *           function to store the input string
 *     POST: Filename will be copied to the specified character array in the calling function
 * REMARKS: Assumed that the file name only contains a maximum of 10 characters. Therefore
 *          no error checking is done.
 */
void getInput(char *getFileName)
{
    char inputFile[11];

    printf("Enter File Name: ");
    scanf("%s", inputFile);
    strcpy(getFileName, inputFile);
}