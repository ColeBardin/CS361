/**
 @mainpage CS361 - HW06 
 @section Description
 This program demonstrates sempahore usage by simulating a noodle bar with only 5 seats with n number of hungry patrons. Only 5 people can be eating at a time, the others must wait. The seats are not first come first serve. They can be taken by anyone in the waiting room.
*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date October 23, 2024
 @section Description
 This is the main file of the program. It contains the main function as well as the thread task function to simulate a hungry patron. 
 */

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>
#include <cstdlib>
#include "semaphore.h"

/**
 Simulates a hungry patron that wants to eat at the noodle bar. They wake up, walk to the noodle bar, wait for a seat, eat, then leave.
 @param id is their number
 */
void eaterTask(int id);

Semaphore seats = Semaphore(5);
Semaphore ioLock = Semaphore(1);

/**
 Handles command line arguments and spins eater threads. Returns once all eater threads return.
 @param argc is the number of command line arguments
 @param argv is an array of command line arguments
 @returns 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    // Parse command line arguments
    if(argc != 2){
        std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
        std::cout << "Usage: " << argv[0] << " <numPeople>" << std::endl;
        return 1;
    };
    int n = atoi(argv[1]);
    if(n <= 0){
        std::cerr << "ERROR: failed to pase numPeople argument" << std::endl;
        std::cout << "Usage: " << argv[0] << " <numPeople>" << std::endl;
        return 1;
    }

    std::vector<std::thread> eaters;

    // Spin and join eater threads
    for(int i = 0; i < n; i++) eaters.push_back(std::thread(eaterTask, i));
    for(auto &t : eaters) t.join();

    return 0;
}

void eaterTask(int id){
    // Seed RNG
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis0_3000(0,3000);
    auto getSleep = std::bind(dis0_3000, gen);

    // Wake up
    ioLock.wait();
    std::cout << "Person " << id << " woke up from their nap." << std::endl;
    ioLock.signal();

    // Walk to bar
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    ioLock.wait();
    std::cout << "Person " << id << " got to the noodle bar." << std::endl;
    ioLock.signal();

    // Wait for a seat
    seats.wait();

    // Start eating
    ioLock.wait();
    std::cout << "Person " << id << " started eating." << std::endl;
    ioLock.signal();
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));

    // Go home
    ioLock.wait();
    std::cout << "Person " << id << " finished eating and is heading home." << std::endl;
    ioLock.signal();
    seats.signal();

    return;
}

