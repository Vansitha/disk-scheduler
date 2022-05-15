/*
 * FILE: algorithms.h
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Function declarations and constants for algorithm.c
 * REFERENCE: None
 * LAST MOD: 11/05/22
 * COMMENTS: None
 */

#ifndef ALGORITHMS_H
#define ALGORITHMS_H

// constants used in the algorithms
#define METADATA_MIN 3
#define REQ_START_POS 3
#define CURR_HEAD_POS 1
#define PRE_REQUEST 2
#define REQ_END 3
#define TOTAL_CYLINDERS_INDEX 0
#define END_INDEX 4

// scheduling algorithms
int fcfs(int *schedulingDataArr, int lenOfArr);
int sstf(int *schedulingDataArr, int lenOfArr);
int scan(int *schedulingDataArr, int lenOfArr);
int look(int *schedulingDataArr, int lenOfArr);
int cScan(int *schedulingDataArr, int lenOfArr);
int cLook(int *schedulingDataArr, int lenOfArr);

// algorithms required for the scheduling algorithms to function
void copyDataArr(int *schedulingDataArr, int lenOfArr, int **copiedArr);
void sortArr(int *dataArr, int len, int **sortedArr);
void extractReqData(int *schedulingDataArr, int lenOfArr, int **reqArr);
int getStartPos(int *dataArr, int len, int headPos);

// algorithms for servicing tracks in both directions
int serviceToInnerTracks(int *requestArr, int itterationStartPos);
int serviceFromInnerTracks(int *requestArr, int itterationEndIndex);
int serviceToOuterTracks(int *requestArr, int itterationStartPos, int lenOfArr, int startHead);
int serviceFromOuterTrack(int *requestArr, int itterationStartIndex, int lenOfArr, int startHead);

// output functions
void runAllAlgorithms(int *schedulingDataArr, int lenOfArr, char *filename);
void displayErr(char *filename);

#endif