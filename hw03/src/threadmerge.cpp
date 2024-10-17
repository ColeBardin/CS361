/**
 @file
 @author Cole Bardin <cab572@drexel.edu>
 @date 10/16/2024
 @section Description
 
 This file implemens the threaded merge sort. Threads are used to speed up the classic merge sort.
 If only 1 thread is requested or if there are less elements than threads, single threaded merge sort is used.
 Otherwise, threads are spun to break up the work. The array is broken into threadCount sections of size size/threadCount. Each thread sorts its section with traditional merge sort.
 After each section is sorted, the threads return for final merging.
 To increase performance optimizatins, new threads are spun to perform the final marging. Each thread merges 2 array sections, halving the number of unmerged sections. The threads halve in count and array sections double in size until the entire array is merged.
 */

#include "threadmerge.h"
#include "mergesort.h"
#include <iostream>
#include <cmath>
#include <thread>
#include <vector>

void tmergesort(int* A, int size, int threadCount){
    // Perform single threaded merge sort on condition
    if(threadCount == 1 || size <= threadCount){
        mergesort(A, size);
        return;
    }

    // Break array into threadCount sections
    int start, stop, middle, i;
    int threadSecSize = size / threadCount;

    // Spin and join threads to merge sort each section
    std::vector<std::thread> threads;
    for(i = 0; i < threadCount; i++){
        start = i * threadSecSize;
        stop = (i + 1) * threadSecSize - 1;
        threads.push_back(std::thread(msortSec, A, start, stop));
    }
    for(auto &t : threads) t.join();
    
    // Final merging
    while(threadCount >= 2){ 
        threads.erase(threads.begin(), threads.end());
        // Spin & join threads to merge every 2 sections
        threadCount /= 2;
        threadSecSize = size / threadCount;
        for(i = 0; i < threadCount; i++){
            start = i * threadSecSize;
            stop = (i + 1) * threadSecSize - 1;
            middle = start + floor((stop - start) / 2);
            threads.push_back(std::thread(merge, A, start, middle, stop));
        }
        for(auto &t : threads) t.join();
    }
}

