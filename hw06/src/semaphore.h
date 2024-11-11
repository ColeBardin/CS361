/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date November 2024
 @section Description
 
 A semaphore built from test and set commands.
 */
#include <atomic>

#ifndef _SEMAPHORE_H_
#define _SEMAPHORE_H_

/**
 A semaphore built on busy waits.
 */
class Semaphore{
private:
    std::atomic<int> counter;/**< Atomic Counter*/
public:
    /**
     Default counter to 1.
     */
    Semaphore();
    /**
     Set counter to a different starting value.
     @param k is the new value to start at.
     */
    Semaphore(int k);
    /**
     Wait until decrement is possible.
     */
    void wait();
    /**
     Increase count by 1 and wake up waiting thread.
     */
    void signal();
};

#endif

