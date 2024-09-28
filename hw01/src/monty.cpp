/**
 @mainpage CS 361 - Homework 1
 @section Description
 This project simulates the Monty Hall problem. 
 It outputs the results odds of winning if the user stays or switches.
 There is a required command line argument to set the number of trails to run.
 This program will divide the simulations between two threads and output the overall stats.
 */

/**
 @file monty.cpp
 @author Cole Bardin <cab572@drexel.edu>
 @date September 23, 2024
 @section DESCRIPTION
 This is the main file of the program. It handles command line inputs and seeds the RNG.
 Then it will spin threads to do the simulation. Then collect the results and display the stats.
 */
#include <iostream>
#include <random>
#include <thread>
#include <time.h>

#include "monty.h"

int main(int argc, char **argv) {
    // Validate number of and values provided as command line arguments
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
    // Seed random number generator
    srand(time(NULL));

    // Spin & join threads, each with half of the workload
    int ret0 = 0;
    int ret1 = 0;
    std::thread thread0(runTests, tests / 2, &ret0);
    std::thread thread1(runTests, (tests / 2) + (tests % 2 ? 1 : 0), &ret1);
    thread0.join();
    thread1.join();

    // Perform analysis and print out stats
    int switchWins = ret0 + ret1;
    float switchWinRatio = 100.0 * switchWins / tests;
    std::cout << "Switch would win " << switchWinRatio << " percent of experiments." << std::endl;
    std::cout << "Stay would win   " << 100.0 - switchWinRatio << " percent of experiments." << std::endl;

    return 0;
}

int runTests(int tests, int *ret){
    int switchWins;
    int prize;
    int choice;
    
    if(!ret) return -1;

    switchWins = 0;
    // Simulate Monty Hall problem and accumulate number of switch wins
    while(tests-- > 0){
        prize = std::rand() / ((RAND_MAX + 1u)/ 3);   
        choice = std::rand() / ((RAND_MAX + 1u)/ 3);   
        if(prize != choice) switchWins++;
    }

    *ret = switchWins;
    return 0;
}

