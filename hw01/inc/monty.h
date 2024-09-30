/**
 @file monty.h
 @author Cole Bardin <cab572@drexel.edu>
 @date September 23, 2024
 @section DESCRIPTION
 This file contains the function prototype used for the Monty Hall simulator thread function.
 */

/**
 @function runTests
 @brief simulates the Monty Hall problem a given number of times and calculates the number of switch wins. Will seed its own RNG that is local to each thread.
 @param tests is number of tests to run.
 @param ret is the destination pointer to store the number of switch wins.
 @return 0 on success, 1 otherwise.
 */
int runTests(int tests, int *ret);
