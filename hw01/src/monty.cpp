/**
 @mainpage CS 361 - Homework 1
 @section Description
 Describe how the entire program works. What is it for?
*/

/**
 @file monty.cpp
 @author Cole Bardin <cab572@drexel.edu>
 @date September 23, 2024
 @section DESCRIPTION
 Write a description of what this file contains here.
*/
#include <iostream>
#include <random>
#include <thread>
#include <time.h>

#include "monty.h"

int main(int argc, char **argv) {
    if(argc != 2){
        std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <ntests>" << std::endl;     
        return 1;
    }

    int tests = std::atoi(argv[1]);
    if(!tests){
        std::cerr << "ERROR: Failed to parse ntests argument" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <ntests>" << std::endl;     
        return 1;
    }

    std::cout << "Monty Hall Problem Simulator" << std::endl;
    srand(time(NULL));

    int r1 = 0;
    int r2 = 0;

    std::thread t1(run_tests, tests / 2, &r1);
    std::thread t2(run_tests, (tests / 2) + (tests % 2 ? 1 : 0), &r2);

    t1.join();
    t2.join();
    int switch_wins = r1 + r2;

    float sw_ratio = 100.0 * switch_wins / tests;

    std::cout << "Switch would win " << sw_ratio << " percent of experiments." << std::endl;
    std::cout << "Stay would win   " << 100.0 - sw_ratio << " percent of experiments." << std::endl;

    return 0;
}

int run_tests(int tests, int *ret){
    int switch_wins;
    int prize;
    int choice;
    
    if(!ret) return -1;

    switch_wins = 0;
    while(tests-- > 0){
        prize = std::rand() / ((RAND_MAX + 1u)/ 3);   
        choice = std::rand() / ((RAND_MAX + 1u)/ 3);   
        if(prize != choice) switch_wins++;
    }

    *ret = switch_wins;
    return 0;
}

