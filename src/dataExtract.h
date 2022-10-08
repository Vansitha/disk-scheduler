/*
 * FILE: dataExtract.h
 * AUTHOR: Vansitha Induja Ratnayake
 * UNIT: Operating Systems - COMP2006
 * PURPOSE: Contains the function declarations for dataExtract.c
 * REFERENCE: None
 * LAST MOD: 12/05/22
 * COMMENTS: None
 */

#ifndef DATAEXTRACT_H
#define DATAEXTRACT_H

void getInput(char *getFileName);
int getArraySize(char *fileName);
bool readFile(char *fileName, int **dataArrPtr, int *arrSize);

#endif