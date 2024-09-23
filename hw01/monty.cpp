/**
 @mainpage CS 361 - Homework 1
 @section Description
 Describe how the entire program works. What is it for?
*/

/**
 @file
 @author Cole Bardin <cab572@drexel.edu>
 @date September 23, 2024
 @section DESCRIPTION
 Write a description of what this file contains here.
*/
#include <iostream>
#include <random>
#include <time.h>

int run_tests(int tests);

int main(int argc, char **argv) {
    int tests;

    if(argc != 2){
        std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <ntests>" << std::endl;     
        return 1;
    }

    tests = std::atoi(argv[1]);
    if(!tests){
        std::cerr << "ERROR: failed to parse number of tests argument" << std::endl;
        std::cerr << "Usage: " << argv[0] << " <ntests>" << std::endl;     
        return 1;
    }

    std::cout << "Running " << tests << " tests..." << std::endl;
    srand(time(NULL));

    int switch_wins = run_tests(tests);

    float sw_ratio = 100.0 * switch_wins / tests;
    std::cout << "Switch wins:   " << switch_wins << " (" << sw_ratio << "%)" << std::endl;
    std::cout << "Switch losses: " << tests - switch_wins << " (" << 100 - sw_ratio << "%)" << std::endl;

    return 0;
}

int run_tests(int tests){
    int switch_wins;
    int i;
    int prize;
    int choice;

    switch_wins = 0;
    for(i = 0; i < tests; i++){
        prize = std::rand() / ((RAND_MAX + 1u)/ 3);   
        choice = std::rand() / ((RAND_MAX + 1u)/ 3);   
        if(prize != choice) switch_wins++;
    }

    return switch_wins;
}








