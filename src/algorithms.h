/*
 * FILE: algorithms.h
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Function declarations and constants for algorithm.c
 * REFERENCE: None
 * LAST MOD: 12/05/22
 * COMMENTS: None
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H
#include <pthread.h>

// constants used in the algorithms
#define METADATA_MIN 3
#define CURRENT_SECTOR_LOC 1
#define REQ_START_POS 3
#define QUEUE_SIZE 4
#define CURR_HEAD_POS 1
#define PRE_REQUEST 2
#define REQ_END 3
#define TOTAL_CYLINDERS_INDEX 0
#define END_INDEX 4
#define OUT_BUFFER_SIZE 6

// struct for the shared data between the threads
typedef struct threadData
{
    pthread_mutex_t accessMutex;
    pthread_cond_t runCondition;
    int lenofArr;
    int *buffer1;
    int buffer2[OUT_BUFFER_SIZE];

} threadData;

// scheduling algorithms
void *fcfs(void *schedulingData);
void *sstf(void *schedulingData);
void *scan(void *schedulingData);
void *look(void *schedulingData);
void *cScan(void *schedulingData);
void *cLook(void *schedulingData);

// algorithms required for the scheduling algorithms to function
void copyDataArr(int *schedulingDataArr, int lenOfArr, int **coppiedArr);
void sortArr(int *dataArr, int len, int **sortedArr);
void extractReqData(int *schedulingDataArr, int lenOfArr, int **reqArr);
int getStartPos(int *dataArr, int len, int headPos);

// algorithms for servicing tracks in both directions
int serviceToInnerTracks(int *requestArr, int itterationStartPos);
int serviceFromInnerTracks(int *requestArr, int itterationEndIndex);
int serviceToOuterTracks(int *requestArr, int itterationStartPos, int lenOfArr, int startHead);
int serviceFromOuterTrack(int *requestArr, int itterationStartIndex, int lenOfArr, int startHead);

void displayErr(char *filename);

#endif