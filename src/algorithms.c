/*
 * FILE: algorithms.c
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Contains all the sechduling algorithms and other associated
 *          algorithms requred to run them
 * REFERENCE: THIS FILE CONTAINS CODE LOGIC OBTAINED FROM AN EXTERNAL SOURCE.
 *            REFERENCED AND INDICADED WHERE NECESSARY.
 *            sstf function - Code logic to implement this function was obtained
 *            from a source from the internet.
 *            (REFERENCE)
 *            Hussain, Sadab. "C-Program of SSTF (Short seek time first )Disk scheduling Algorithms in operating
 *            system (OS)." ECZ Easy coding zone. Accessed May 1st, 2022.
 *            https://www.easycodingzone.com/2021/07/c-program-of-sstf-short-seek-time-first.html
 * LAST MOD: 12/05/22
 * COMMENTS: Important details to note.
 *           Inner tracks = current head position to 0th track (Inner most track)
 *           Outer tracks = current head poistion to the max track (total cyclinders - 1)
 *           Current head position = 2nd value read from the input file
 *           Max cylinder  = 1st value from input file - 1 (starting from 0)
 *           Previous Request = 3rd value from the input file
 *           All the data in the input file is directly read to an array and a boundary
 *           is placed from the 2nd index when itterating to calculate the seek times.
 *           Each function has been modified from the previous version to support multi-threading
 */

#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <pthread.h>
#include "algorithms.h"

/*
 * NAME: fcfs
 * PURPOSE: Implementation of a first come first serve disk scheduling algorithm
 * IMPORTS: Array with all the request metadata from the
 *          file (schedulingDataArr) and length of the array (lenofArr)
 * EXPORTS:
 * ASSERTIONS:
 *      PRE: Array should be in the same format as in the input file.
 *      POST: None
 * COMENTS: Loops through the request array and services each track in the order
 *          of arrival
 */
void *fcfs(void *schedulingData)
{
    // initialize the shared data between the threads
    threadData *sharedData = (threadData *)schedulingData;

    // thread does not quit unit parent thread sends in a cancellation request
    while (1)
    {
        // get the mutex lock and wait for signal from parent thread
        pthread_mutex_lock(&sharedData->accessMutex);
        pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

        int *schedulingDataArr = sharedData->buffer1;
        int lenOfArr = sharedData->lenofArr;

        // algorithm starts here

        int currHead;
        int totalSeekTime;
        int i;
        int diff;

        /* intitally set using the data in the array */
        currHead = schedulingDataArr[CURR_HEAD_POS];
        totalSeekTime = 0;

        for (i = REQ_START_POS; i < lenOfArr; i++)
        {
            diff = abs(schedulingDataArr[i] - currHead);
            totalSeekTime += diff;
            currHead = schedulingDataArr[i];
        }

        // insert the seek time into the buffer
        sharedData->buffer2[0] = totalSeekTime;

        // algorithm ends here
        pthread_mutex_unlock(&sharedData->accessMutex);
        pthread_cond_signal(&sharedData->runCondition);
    }
}

/*
 * NAME: sstf
 * PURPOSE: Implementation of a shortest seek time first disk scheduling algorithm
 * IMPORTS: Array with all the request metadata from the
 *          file (schedulingDataArr) and length of the array (lenofArr)
 * EXPORTS: seek time
 * ASSERTIONS:
 *      PRE: Array should be in the same format as in the input file.
 *      POST: None
 * REFERENCE: Hussain, Sadab. "C-Program of SSTF (Short seek time first )Disk
 *            scheduling Algorithms in operating system (OS)."
 *            ECZ Easy coding zone. Accessed May 1st, 2022.
 *            https://www.easycodingzone.com/2021/07/c-program-of-sstf-short-seek-time-first.html
 */
void *sstf(void *schedulingData)
{
    // initialize the shared data between the threads
    threadData *sharedData = (threadData *)schedulingData;

    // thread does not quit unit parent thread sends in a cancellation request
    while (1)
    {
        // get the mutex lock and wait for signal from parent thread
        pthread_mutex_lock(&sharedData->accessMutex);
        pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

        // algorithm starts here
        int *schedulingDataArr = sharedData->buffer1;
        int lenOfArr = sharedData->lenofArr;

        int min;
        int diff;
        int index;
        int initial;
        int count;
        int numberOfRequests;
        int i;
        int totalSeekTime;
        int *schedulingDataArrCpy;

        copyDataArr(schedulingDataArr, lenOfArr, &schedulingDataArrCpy);

        // * START OF COD LOGIC OBTAINED FROM THE SOURCE REFERENCED ABOVE *
        count = 0;
        numberOfRequests = lenOfArr - REQ_START_POS;
        totalSeekTime = 0;
        initial = schedulingDataArrCpy[CURR_HEAD_POS];
        while (count != numberOfRequests)
        {
            // min can be set to any large value
            min = 1000000;
            for (i = REQ_START_POS; i < lenOfArr; i++)
            {
                // finds the difference between current request and the intial request
                diff = abs(schedulingDataArrCpy[i] - initial);

                if (min > diff)
                {
                    min = diff;
                    index = i;
                }
            }
            totalSeekTime += min;
            initial = schedulingDataArrCpy[index];
            // sets the current request to a large value. This indicates that it was already visited
            schedulingDataArrCpy[index] = 1000000;
            count++;
        }
        // * END OF CODE LOGIC OBTAINED FROM THE SOURCE REFERENCED ABOVE *

        // insert the seek time into the buffer
        sharedData->buffer2[1] = totalSeekTime;
        free(schedulingDataArrCpy);

        /*
        once execution is complete unlock mutex and signal parent thread to
        take over
        Tries to execute again but hits the wait condition at the top and wait for
        another data set and signal form the parent thread to continue
        */
        pthread_mutex_unlock(&sharedData->accessMutex);
        pthread_cond_signal(&sharedData->runCondition);
    }
}

void *scan(void *schedulingData)
{

    // initialize the shared data between the threads
    threadData *sharedData = (threadData *)schedulingData;

    // thread does not quit unit parent thread sends in a cancellation request
    while (1)
    {

        // get the mutex lock and wait for signal from parent thread
        pthread_mutex_lock(&sharedData->accessMutex);
        pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

        // algorithm starts here
        int *schedulingDataArr = sharedData->buffer1;
        int lenOfArr = sharedData->lenofArr;

        /*
        inner tracks and outter tracks distinguished from a pivot point which
        is the current head location
        */
        int *schedulingDataArrCpy;
        int *sortedDataArr;
        bool moveToInnerTracks;
        int currHead;
        int direction;
        int totalSeekTime;
        int initialHeadPos;
        int itterationStartIndex;
        int finalServiceRequest;

        copyDataArr(schedulingDataArr, lenOfArr, &schedulingDataArrCpy);
        sortArr(schedulingDataArrCpy, lenOfArr, &sortedDataArr);

        // check the direction which the read/write head moves
        direction = schedulingDataArrCpy[CURR_HEAD_POS] - schedulingDataArrCpy[PRE_REQUEST];
        moveToInnerTracks = false;
        /* if set to true goes towards track 0 which is considered the inner track if
        set to false goes towards the outter most track */
        if (direction < 0)
        {
            moveToInnerTracks = true;
        }

        totalSeekTime = 0;
        initialHeadPos = schedulingDataArrCpy[CURR_HEAD_POS];

        itterationStartIndex = getStartPos(schedulingDataArrCpy, lenOfArr, initialHeadPos);

        // if go towards inner track then go to the left in array
        if (moveToInnerTracks)
        {
            // call the left track service function
            totalSeekTime = serviceToInnerTracks(sortedDataArr, itterationStartIndex);

            // add the final request before reaching 0th track
            totalSeekTime += sortedDataArr[REQ_END];
            currHead = 0; // reached 0th track now will continue in the reverse direction from there

            // reached 0  now it will start servicing in reverse
            totalSeekTime += serviceToOuterTracks(sortedDataArr, itterationStartIndex, lenOfArr, currHead);
        }
        // this will start from servicing outter tracks first because the head moves in that direction
        else if (!moveToInnerTracks)
        {
            currHead = initialHeadPos;
            totalSeekTime = serviceToOuterTracks(sortedDataArr, itterationStartIndex, lenOfArr, currHead);

            finalServiceRequest = sortedDataArr[lenOfArr - 1];
            totalSeekTime += (sortedDataArr[TOTAL_CYLINDERS_INDEX] - finalServiceRequest);

            currHead = sortedDataArr[lenOfArr - 1];
            totalSeekTime += serviceFromOuterTrack(sortedDataArr, END_INDEX, lenOfArr, currHead);

            // add the final service request
            totalSeekTime += sortedDataArr[REQ_START_POS];
        }

        // insert the seek time into the buffer
        sharedData->buffer2[2] = totalSeekTime;
        free(schedulingDataArrCpy);

        /*
        once execution is complete unlock mutex and signal parent thread to
        take over
        Tries to execute again but hits the wait condition at the top and wait for
        another data set and signal form the parent thread to continue
        */
        pthread_mutex_unlock(&sharedData->accessMutex);
        pthread_cond_signal(&sharedData->runCondition);
    }
}

void *cScan(void *schedulingData)
{
    // initialize the shared data between the threads
    threadData *sharedData = (threadData *)schedulingData;

    // thread does not quit unit parent thread sends in a cancellation request
    while (1)
    {
        // get the mutex lock and wait for signal from parent thread
        pthread_mutex_lock(&sharedData->accessMutex);
        pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

        int *schedulingDataArr = sharedData->buffer1;
        int lenOfArr = sharedData->lenofArr;

        // algorithm starts here
        int *schedulingDataArrCpy;
        int *sortedDataArr;
        int totalSeekTime;
        int itterationStartIndex;
        int initialHeadPos;
        int itterationEndIndex;
        int maxTrack;
        int direction;
        int currHead;
        bool moveToInnerTracks;

        // intital set up is the same as the scan algorithm
        copyDataArr(schedulingDataArr, lenOfArr, &schedulingDataArrCpy);
        sortArr(schedulingDataArrCpy, lenOfArr, &sortedDataArr);

        direction = schedulingDataArrCpy[CURR_HEAD_POS] - schedulingDataArrCpy[PRE_REQUEST];
        moveToInnerTracks = false;
        if (direction < 0)
        {
            moveToInnerTracks = true;
        }

        initialHeadPos = schedulingDataArrCpy[CURR_HEAD_POS];
        itterationStartIndex = getStartPos(schedulingDataArrCpy, lenOfArr, initialHeadPos);

        totalSeekTime = 0;

        if (moveToInnerTracks)
        {
            // first service inner tracks
            totalSeekTime = serviceToInnerTracks(sortedDataArr, itterationStartIndex);

            // service final request before reaching 0th track
            totalSeekTime += sortedDataArr[REQ_START_POS];

            // now go to the outter most track without servicing anything
            maxTrack = schedulingDataArrCpy[TOTAL_CYLINDERS_INDEX] - 1;
            totalSeekTime += maxTrack;

            // we need to go backwards now
            itterationEndIndex = itterationStartIndex;
            totalSeekTime += serviceFromOuterTrack(sortedDataArr, itterationEndIndex, lenOfArr, maxTrack);
        }
        else if (!moveToInnerTracks)
        {
            // service outer tracks first
            currHead = sortedDataArr[CURR_HEAD_POS];
            totalSeekTime = serviceToOuterTracks(sortedDataArr, itterationStartIndex, lenOfArr, currHead);

            // outter tracks services now reach the end of the disk before going in reverse to the start of the disk
            // Total number of cylinders - 1 because it starts from 0
            totalSeekTime += abs(sortedDataArr[lenOfArr - 1] - (sortedDataArr[TOTAL_CYLINDERS_INDEX] - 1));

            // now return to the start of the disk
            // and service the other requests from 0 to end of requests
            totalSeekTime += ((sortedDataArr[TOTAL_CYLINDERS_INDEX] - 1) - 0);
            itterationEndIndex = getStartPos(sortedDataArr, lenOfArr, initialHeadPos) + 1;
            totalSeekTime += serviceFromInnerTracks(sortedDataArr, itterationEndIndex);
        }

        // insert the seek time into the buffer
        sharedData->buffer2[3] = totalSeekTime;
        free(schedulingDataArrCpy);

        /*
       once execution is complete unlock mutex and signal parent thread to
       take over
       Tries to execute again but hits the wait condition at the top and wait for
       another data set and signal form the parent thread to continue
       */
        pthread_mutex_unlock(&sharedData->accessMutex);
        pthread_cond_signal(&sharedData->runCondition);
    }
}

void *look(void *schedulingData)
{
    // initialize the shared data between the threads
    threadData *sharedData = (threadData *)schedulingData;
    // thread does not quit unit parent thread sends in a cancellation request
    while (1)
    {
        // get the mutex lock and wait for signal from parent thread
        pthread_mutex_lock(&sharedData->accessMutex);
        pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

        int *schedulingDataArr = sharedData->buffer1;
        int lenOfArr = sharedData->lenofArr;

        // algorithm starts here
        int *schedulingDataArrCpy;
        int *sortedDataArr;
        bool moveToInnerTracks;
        int currHead;
        int direction;
        int totalSeekTime;
        int initialHeadPos;
        int itterationStartIndex;

        copyDataArr(schedulingDataArr, lenOfArr, &schedulingDataArrCpy);
        sortArr(schedulingDataArrCpy, lenOfArr, &sortedDataArr);

        // check the direction which the read/write head moves
        direction = schedulingDataArrCpy[CURR_HEAD_POS] - schedulingDataArrCpy[PRE_REQUEST];
        /*
        if set to true goes towards track 0 which is considered the inner track if set
        to false goes towards the outter most track
        */
        moveToInnerTracks = false;
        if (direction < 0)
        {
            moveToInnerTracks = true;
        }

        totalSeekTime = 0;
        initialHeadPos = schedulingDataArrCpy[CURR_HEAD_POS];

        itterationStartIndex = getStartPos(schedulingDataArrCpy, lenOfArr, initialHeadPos);

        // if go towards inner track then go to the left in array
        if (moveToInnerTracks)
        {
            // service inner tracks first
            totalSeekTime = serviceToInnerTracks(sortedDataArr, itterationStartIndex);

            // now the current head is at the last request at the inner most track does not reach 0
            currHead = sortedDataArr[REQ_END];

            // now it will gotowards outter tracks
            totalSeekTime += serviceToOuterTracks(sortedDataArr, itterationStartIndex, lenOfArr, currHead);
        }
        // this will start from servicing outter tracks first because the head moves in that direction
        else if (!moveToInnerTracks)
        {
            currHead = initialHeadPos;
            totalSeekTime = serviceToOuterTracks(sortedDataArr, itterationStartIndex, lenOfArr, currHead);

            currHead = sortedDataArr[lenOfArr - 1];
            totalSeekTime += serviceFromOuterTrack(sortedDataArr, REQ_START_POS, lenOfArr, currHead);

            totalSeekTime += sortedDataArr[REQ_START_POS];
        }

        // insert the seek time into the buffer
        sharedData->buffer2[4] = totalSeekTime;
        free(schedulingDataArrCpy);

        /*
        once execution is complete unlock mutex and signal parent thread to
        take over
        Tries to execute again but hits the wait condition at the top and wait for
        another data set and signal form the parent thread to continue
        */
        pthread_mutex_unlock(&sharedData->accessMutex);
        pthread_cond_signal(&sharedData->runCondition);
    }
}

void *cLook(void *schedulingData)
{
    // initialize the shared data between the threads
    threadData *sharedData = (threadData *)schedulingData;

    // thread does not quit unit parent thread sends in a cancellation request
    while (1)
    {
        // get the mutex lock and wait for signal from parent thread
        pthread_mutex_lock(&sharedData->accessMutex);
        pthread_cond_wait(&sharedData->runCondition, &sharedData->accessMutex);

        int *schedulingDataArr = sharedData->buffer1;
        int lenOfArr = sharedData->lenofArr;

        // algorithm starts here
        int *schedulingDataArrCpy;
        int *sortedDataArr;
        bool moveToInnerTracks;
        int currHead;
        int direction;
        int totalSeekTime;
        int initialHeadPos;
        int itterationStartIndex;
        int itterationEndIndex;
        int value;

        totalSeekTime = 0;
        copyDataArr(schedulingDataArr, lenOfArr, &schedulingDataArrCpy);
        sortArr(schedulingDataArrCpy, lenOfArr, &sortedDataArr);

        // check the direction which the read/write head moves
        direction = schedulingDataArrCpy[CURR_HEAD_POS] - schedulingDataArrCpy[PRE_REQUEST];
        /*
        if set to true goes towards track 0 which is considered the inner track if set
        to false goes towards the outter most track
        */
        moveToInnerTracks = false;
        if (direction < 0)
        {
            moveToInnerTracks = true;
        }

        initialHeadPos = schedulingDataArrCpy[CURR_HEAD_POS];

        itterationStartIndex = getStartPos(schedulingDataArrCpy, lenOfArr, initialHeadPos);

        // if go towards inner track then go to the left in array
        if (moveToInnerTracks)
        {
            // call the left track service function
            totalSeekTime = serviceToInnerTracks(sortedDataArr, itterationStartIndex);

            currHead = sortedDataArr[lenOfArr - 1];
            /*
            stop at the final request in the outer track now it will travel to the request at the outter most
            track and start servicing from there onwards
            */
            // this is the movement to the other end of the disk and will start servicing from there
            value = abs(sortedDataArr[REQ_START_POS] - currHead);
            totalSeekTime += value;

            totalSeekTime += serviceFromOuterTrack(sortedDataArr, itterationStartIndex, lenOfArr, currHead);
        }
        // this will start from servicing outter tracks first because the head moves in that direction
        if (!moveToInnerTracks)
        {
            // service outter tracks first and then service inner tracks
            currHead = initialHeadPos;
            totalSeekTime = serviceToOuterTracks(sortedDataArr, itterationStartIndex, lenOfArr, currHead);

            // then move disk head towards the inner most request and start servicing from there
            currHead = sortedDataArr[REQ_START_POS];
            totalSeekTime += (sortedDataArr[lenOfArr - 1] - currHead);
            itterationEndIndex = itterationStartIndex + 1;

            totalSeekTime += serviceToOuterTracks(sortedDataArr, REQ_START_POS, itterationEndIndex, currHead);
        }

        // insert the seek time into the buffer
        sharedData->buffer2[5] = totalSeekTime;
        free(schedulingDataArrCpy);

        /*
        once execution is complete unlock mutex and signal parent thread to
        take over
        Tries to execute again but hits the wait condition at the top and wait for
        another data set and signal form the parent thread to continue
        */
        pthread_mutex_unlock(&sharedData->accessMutex);
        pthread_cond_signal(&sharedData->runCondition);
    }
}

/*
 * NAME: copyDataArr
 * PURPOSE: creates a copy of an array
 * IMPORTS: Array with all the request metadata from the
 *          file (schedulingDataArr) and length of the array (lenofArr)
 *          pointer to the new array in the calling function (copiedArr)
 * EXPORTS: None
 * ASSERTIONS:
 *      PRE: None
 *      POST: Creates a copy of the array provided
 * COMMENTS: None
 */
void copyDataArr(int *schedulingDataArr, int lenOfArr, int **coppiedArr)
{
    int *copyArr;
    int i;

    copyArr = (int *)malloc(sizeof(int *) * (lenOfArr - 1));

    for (i = 0; i < lenOfArr; i++)
    {
        copyArr[i] = schedulingDataArr[i];
    }

    *coppiedArr = copyArr;
}

/*
 * NAME: sortArr
 * PURPOSE: sorts the array of request data in ascending order
 * IMPORTS: Array with all the request metadata from the
 *          file (schedulingDataArr) and length of the array (lenofArr)
 *          pointer to the new array in the calling function (copiedArr)
 * EXPORTS: None
 * ASSERTIONS:
 *      PRE: None
 *      POST: None
 * COMMENTS: Bubble Sort algorithm implemented
 */
void sortArr(int *dataArr, int len, int **sortedArr)
{

    /* starts the sorting from 3rd cell in array */
    int i;
    int passes = 0;
    bool sorted = false;
    int temp = 0;

    while (!sorted)
    {
        sorted = true;
        for (i = REQ_START_POS; i < ((len - passes) - 1); i++)
        {
            if (dataArr[i] > dataArr[i + 1])
            {
                temp = dataArr[i];
                dataArr[i] = dataArr[i + 1];
                dataArr[i + 1] = temp;
                sorted = false;
            }
        }
        passes++;
    }

    *sortedArr = dataArr;
}

/*
 * NAME: serviceToInnerTracks
 * PURPOSE: Responsible for moving the read/write heads towards track 0
 *          from the current position (2nd value from input file)
 * IMPORTS:  Array with all the request metadata from the
 *          file (schedulingDataArr) and the index from where
 *          the itteration should start from (itterationStartIndex)
 * EXPORTS: Seek Time
 * ASSERTIONS:
 *      PRE: Requires a sorted array to get correct results. Calling function
 *           should handle the intial set up for the particular scheduling
 *           algorithm.
 *      POST: None
 * COMMENTS: None
 */
int serviceToInnerTracks(int *requestArr, int itterationStartIndex)
{
    int currHead;
    int nextRequest;
    int i;
    int diff;
    int seekTime;

    currHead = requestArr[CURR_HEAD_POS];
    seekTime = 0;

    for (i = itterationStartIndex; i >= REQ_END; i--)
    {
        nextRequest = requestArr[i];
        diff = abs(currHead - nextRequest);
        seekTime += diff;
        currHead = nextRequest;
    }

    return seekTime;
}

/*
 * NAME: serviceFromInnerTracks
 * PURPOSE: Responsible for moving the read/write head from the inner
 *          most track (0th track) towards the current head position
 *          indicated by the 2nd value in the input file.
 * IMPORTS:  Array with all the request metadata from the
 *          file (schedulingDataArr)
 *          The index from where the itteration should end (itterationEndIndex)
 * EXPORTS: Seek Time
 * ASSERTIONS:
 *      PRE: Requires a sorted array to get correct results. Calling function
 *           should handle the intial set up for the particular scheduling
 *           algorithm.
 *      POST: None
 * COMMENTS: None
 */
int serviceFromInnerTracks(int *requestArr, int itterationEndIndex)
{
    int currHead;
    int nextRequest;
    int i;
    int diff;
    int seekTime;

    currHead = 0;
    seekTime = 0;

    for (i = REQ_START_POS; i < itterationEndIndex; i++)
    {
        nextRequest = requestArr[i];
        diff = abs(currHead - nextRequest);
        seekTime += diff;
        currHead = nextRequest;
    }
    return seekTime;
}

/*
 * NAME: serviceToOuterTracks
 * PURPOSE: Responsible for moving the read/write head from the current
 *          head position to the outter most track (max cylinder - 1) as
 *          indicated in the input file.
 * IMPORTS: Array with all the request metadata from the file (schedulingDataArr)
 *          The index from where the itteration should start (itterationStartIndex)
 *          Length of the request array (lenofArr)
 *          The request which it needs to begin servicing from (startHead)
 * EXPORTS: Seek Time
 * ASSERTIONS:
 *      PRE: Requires a sorted array to get correct results. Calling function
 *           should handle the intial set up for the particular scheduling
 *           algorithm.
 *      POST: None
 * COMMENTS: None
 */
int serviceToOuterTracks(int *requestArr, int itterationStartIndex, int lenOfArr, int startHead)
{
    int i;
    int currHead;
    int diff;
    int seekTime;
    int nextRequest;

    currHead = startHead;
    seekTime = 0;

    // we need to start from the curr head position +1
    for (i = itterationStartIndex + 1; i < lenOfArr; i++)
    {
        nextRequest = requestArr[i];
        diff = abs(currHead - nextRequest);
        seekTime += diff;
        currHead = nextRequest;
    }

    return seekTime;
}

/*
 * NAME: serviceFromOuterTracks
 * PURPOSE: Responsible for moving the read/write from the outer most track
 *          (max cylinder - 1) to the current head position.
 * IMPORTS:  Array with all the request metadata from the
 *          file (schedulingDataArr) and the index from where
 *          The itteration should end (itterationEndIndex)
 *          Length of the request array (lenofArr)
 *          Max cylinder size - 1 (startHead)
 * EXPORTS: Seek Time
 * ASSERTIONS:
 *      PRE: Requires a sorted array to get correct results. Calling function
 *           should handle the intial set up for the particular scheduling
 *           algorithm.
 *      POST: None
 * COMMENTS: None
 */
int serviceFromOuterTrack(int *requestArr, int itterationEndIndex, int lenOfArr, int startHead)
{

    int i;
    int currHead;
    int diff;
    int seekTime;
    int nextRequest;

    currHead = startHead;
    seekTime = 0;

    // we need to start from the curr head position +1
    for (i = lenOfArr - 1; i > itterationEndIndex; i--)
    {
        nextRequest = requestArr[i];
        diff = abs(currHead - nextRequest);
        seekTime += diff;
        currHead = nextRequest;
    }

    return seekTime;
}

/*
 * NAME: getStartPos
 * PURPOSE: Determines the index which should be used to begin iterations.
 *          based on the current head position
 * IMPORTS: Request array (dataArr)
 *          Value of the current head position (initialHeadVal)
 * EXPORTS: starting index (startPos)
 * ASSERTIONS:
 *      PRE: Requires a sorted data array in ascending order
 *      POST: Retruns the index from where the scheduling algorithm should
 *            start servicing from
 * COMMENTS: Because the current head position is the second value from the input file,
 *           it will not be included in the sorted array. However, the array is only sorted
 *           from the fourth item onwards (request data start). To begin servicing the request,
 *           it is necessary to determine where the current head position lies in the sorted
 *           array.
 */
int getStartPos(int *dataArr, int len, int initialHeadVal)
{
    /* requires the sorted array */
    int i;
    int startPos;

    for (i = REQ_START_POS; i < len; i++)
    {
        if (dataArr[i] < initialHeadVal)
        {
            startPos = i;
        }
    }

    return startPos;
}

/*
 * NAME: displayErr
 * PURPOSE: Display error message if there is insufficient data in the input file
 * IMPORTS: Filename
 * EXPORTS: None
 * ASSERTIONS:
 *      PRE: None
 *      POST: None
 * COMMENTS: None
 */
void displayErr(char *filename)
{
    printf("\nInsufficent Data In File: %s", filename);
    printf("\n================\n");
}