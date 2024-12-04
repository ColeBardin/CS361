/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date Orgininal: 2021, Revised: 2024
 @section Description
 
 A thread pool that searches for words in file contents. All thread synchronization is done with the channel class provided in class.
 */
#ifndef _THREAD_POOL_H_
#define _THREAD_POOL_H_

#include <thread>
#include <queue>
#include "task.h"
#include "channel.h"

/**
 This class manages threads and searches for words in files
 */
class threadPool{
private:
    int numberOfThreads;/**< threads are in our pool*/
    std::thread** myThreads;/**< Pointers to the threads*/
    bool done;/**< True when all work is done.*/
    channel<int>* queueLk;/**< A channel used to lock the queue.*/
    std::queue<task*>* toDO;/**< A list of tasks.*/
public:
    /**
     Create a new Thread Pool.
     */
    threadPool();
    /**
     Delete a thread pool from memory.
     */
    ~threadPool();
    /**
     Add a task to the todo list.
     @param t is the task to add.
     */
    void addWork(task* t);
    /**
     Start up the threads and run them.
     */
    void start();
    /**
     Stop the pool and exit all threads.
     */
    void stop();
private:
    /**
     Instructions for a worker thread to play tic-tac-toe
     @param statusID is the threads index into the array
     */
    void workerThread(int statusID);
};

#endif
