/**
 @file find_target.h
 @author Cole Bardin <cab572@drexel.edu>
 @section Description
 
 Explain what you did in this file:
 I added 3 function prototypes to aid my solution. 
 checkFuncRange is the thread function that checks which value makes func return true.
 smaller finds the smalles value according to the definition in the assignment PDF.
 smaller4 is a wrapper around smaller that compares 4 numbers instead of 2.
 */
#ifndef _FIND_TARGET_H_
#define _FIND_TARGET_H_

//You need this for general functional
//programming
#include <functional>

/**
 Takes any function that returns bool and takes int as input.
 prints std::cout the smallest (absolute value) number that returns true.
 @param func is the function to test
 */
void findTarget(std::function<bool(int)> func);

//You may add as many additional functions
//or includes to this file as you want.
//You MAY NOT change the prototype for findTarget

/**
 Searches every other number in range [start, stop] to see what makes a funciton true. If so, it will return 1 and set the ret pointer. 
 start must be less than stop. If start == stop, then it will only check 1 value.
 @param func is the function to test
 @param start is the first value to check
 @param stop is the last value to check
 @param ret is a destination pointer to store smallest number
 */
void checkFuncRange(std::function<bool(int)> func, int start, int stop, int *ret);

/**
 Compares 2 numbers and returns the smallest absolute value of the two or the absolute value of x.
 @param x is a number to compare
 @param y is a number to compare
 @return the signed value of the number with the smallest absolute value. If the numbers are equal, it returns the absolute value of x.
 */
int smaller(int x, int y);

/**
 Compares 4 signed integers and returns the smallest absolute value. Compares a to b and c to d first, then compares the smaller of the first comparisons.
 @param a is a number to compare
 @param b is a number to compare
 @param c is a number to compare
 @param d is a number to compare
 @return smallest absolue value of the 4 signed integers
 */
int smaller4(int a, int b, int c, int d);

#endif
