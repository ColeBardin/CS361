/**
 @file
 @author Cole Bardin <cab572@drexel.edu>
 @date 10/16/2024
 @section Description
 
 This source file implements a classic textbook Merge Sort Algorithm.
 I copied the algorithms line for line directly from the assignment PDF 
 */

#include <iostream>
#include <cmath>
#include <stdlib.h>
#include <string.h>
#include "mergesort.h"

void mergesort(int* A, int size){
    msortSec(A, 0, size - 1);
}

void msortSec(int* A, int start, int stop){
    if(start >= stop) return;
    int middle = start + floor((stop - start) / 2);
    msortSec(A, start, middle);
    msortSec(A, middle + 1, stop);
    merge(A, start, middle, stop);
}

void merge(int* A, int start, int middle, int stop){
    int *Aux = (int *)malloc(sizeof(int) * (stop - start + 1));
    if(Aux == NULL){
        std::cerr << "ERROR: Failed to malloc copy array" << std::endl;
        return;
    }
    memcpy(Aux, &A[start], sizeof(int) * (stop - start + 1));    
    int Aux_middle = middle - start;
    int Aux_stop = stop - start;
    int i = 0;
    int j = Aux_middle + 1;

    for(int k = start; k < stop + 1; k++){
        if(i > Aux_middle) A[k] = Aux[j++];
        else if(j > Aux_stop) A[k] = Aux[i++];
        else if(Aux[j] > Aux[i]) A[k] = Aux[i++];
        else A[k] = Aux[j++];
    }
    free(Aux);
}

