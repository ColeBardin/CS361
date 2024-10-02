/**
 @file find_target.cpp
 @author Cole Bardin <cab572@drexel.edu>
 @section Description
 
 Explain what you did in this file:
 I implemented findTarget by creating 4 threads to check the values outlined in the assignment PDF. Each thread checks in its range and will set a dest pointer if it finds a value.
 Then I implemented the smaller function from the assignment PDF. I created an expanding function around smaller, called smaller4, that compares 4 numbers instead of 2.
 After joining all the threads, the it computes the smallest value and prints it to COUT.
 */

//For I/O
#include <iostream>
//Get the prototypes
#include "find_target.h"
//You may include other libraries
#include <thread>

//Rewrite this function to solve the problem.
//You may not change the prototype, only implementation.
void findTarget(std::function<bool(int)> func){
    int max = 100000;
    int ret[4] = {max, max, max, max};
    // Create & join array of threads to handle each range
    std::thread threads[4] = {
        std::thread(checkFuncRange, func, 0, max - 2, &ret[0]),
        std::thread(checkFuncRange, func, 1, max - 1, &ret[1]),
        std::thread(checkFuncRange, func, -1 * max + 2, -2, &ret[2]),
        std::thread(checkFuncRange, func, -1 * max + 1, -1, &ret[3])
    };
    for(int i = 0; i < 4; i++) threads[i].join();
    
    // Compare results values from threads
    int smallest = smaller4(ret[0], ret[1], ret[2], ret[3]);
    if(smallest == max){
        std::cerr << "ERROR: value not found to make func true" << std::endl;
        return;
    }
    std::cout << "The smallest (abs val) answer that makes the function true is "
    << smallest << std::endl;
    return;
}

//Add as many other functions as you need.

void checkFuncRange(std::function<bool(int)> func, int start, int stop, int *ret){
    // Validate input parameters
    if(start > stop){
        std::cerr 
        << "ERROR: thread received start value greater than stop" 
        << std::endl;
        return; 
    }
    if(!ret){
        std::cerr 
        << "ERROR: thread received NULL ret pointer" 
        << std::endl;
        return;
    }

    // Find the smallest value that makes func return true, setting ret if found
    for(int i = start; i <= stop; i = i + 2){
        if(func(i) == true)
        {
            *ret = i;
            return;
        }
    }
    return;
}

int smaller(int x, int y){
    // Smaller implementation from the assignment PDF
    int abs_x = std::abs(x); 
    int abs_y = std::abs(y); 

    if(abs_x < abs_y) return x;
    else if(abs_y < abs_x) return y;
    else return abs_x;
}

int smaller4(int a, int b, int c, int d){
    int temp[2];

    // Find the smallest of all 4 numbers
    // First comparing a to b and c to d
    // Then comparing the results of the previous comparisons
    temp[0] = smaller(a, b);
    temp[1] = smaller(c, d);
    return smaller(temp[0], temp[1]);
}

