/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date November 2024
 @section Description
 
 Implementation of a Semaphore with atomics.
 */
#include "semaphore.h"
#include <thread>

//Store a 1 into the atomic
Semaphore::Semaphore(){
    counter.store(1);
}

//Store whatever value is given.
Semaphore::Semaphore(int k){
    counter.store(k);
}

//Wait until we can decrement.
void Semaphore::wait(){
    int old = counter.load();
    while(true){
        while(old==0){
            //Give Someone else a chance
            std::this_thread::yield();
            old = counter.load();
        }//old != 0 here
        int current = old - 1;
        bool a = counter.compare_exchange_strong(old,current);
        if(a){return;}
    }
}

//Increment counter by one
void Semaphore::signal(){
    int old = counter.load();
    while(true){
        int current = old + 1;
        bool a = counter.compare_exchange_strong(old,current);
        if(a){return;}
    }
}

