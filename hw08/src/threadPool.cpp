/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date Orgininal: 2021, Revised: 2024
 @section Description
 
 A thread pool that searches for words in file contents. All thread synchronization is done with the channel class provided in class.
 
 Inspired by:
 https://ncona.com/2019/05/using-thread-pools-in-cpp/
 */

#include "threadPool.h"
#include <iostream>

//Constructor
//Determine number of threads for this
//system
threadPool::threadPool(){
	numberOfThreads =
		std::thread::hardware_concurrency();
    // Default to 2 threads
    if(numberOfThreads < 2) numberOfThreads = 2;
    //Allocate Space for Threads
    myThreads = new std::thread*[numberOfThreads];
    //Queue to store tasks
    toDO = new std::queue<task*>();
    // Create queue lock channel
    queueLk = makeChannel<int>(1);
    queueLk->send(0);
}

// Close down the Pool
threadPool::~threadPool(){
	delete myThreads;
	delete toDO;
    queueLk->close();
    delete queueLk;
}

//Add A Task to the work list
void threadPool::addWork(task* t){
    queueLk->receive();
	toDO->push(t);
    queueLk->send(0);
}

//Start up all threads
void threadPool::start(){
	//Start up all the threads
	for(int i=0; i < numberOfThreads; i++){
		//Start the thread
		myThreads[i] = new std::thread(
			&threadPool::workerThread,
			this,
			i
			);
	}
}

//Shut Down all threads
void threadPool::stop(){
    int i;
	// Add poison tasks to queue
    queueLk->receive();
    for(i = 0; i < numberOfThreads; i++) toDO->push(NULL);
    queueLk->send(0);
    // Join all threads
	for(i = 0; i < numberOfThreads; i++) myThreads[i]->join();
}

//Get a task, resolve it
//update the task list
//wait for more tasks
void threadPool::workerThread(int statusID){
	// Pointer to hold a task
	task* t;
	// Break this loop when done
	while(true){
        // Acquire queue lock
        queueLk->receive();
        // If no work has been added
        if(toDO->empty()){
            // Unlock queue and yield
            queueLk->send(0);
            std::this_thread::yield();
            continue;
        }
        // Get a value off the queue
        t = toDO->front();
        toDO->pop();
        // Check for poison task
        if(!t){
            // Die
            queueLk->send(0);
            return;
        }
        // Release queue lock
        queueLk->send(0);
		//Do the task
        t->runTask();
	}	
}

