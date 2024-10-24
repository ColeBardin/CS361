/**
 @mainpage CS361 - Race - HW04
 @section Description

 Simulates a race game between a chosen number of threads. The game master thread choses a number, n, between 0 and 5 inclusive. Then it rolls n 6 sided dice. It adds the n values to a queue.

 The players race to get access to the queue. They pull the front value and move that many spaces forward. Once they pass 20 spaces, they are finished and they add themselves to the podium.

 Once all players finish, the game master returns and the podium is printed out.
*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date October 23, 2024
 @section Description
 The main file for the race program. It defines and implements the game master and player thread functions. It also starts race and prints out the results. 
 */

#include <iostream>
#include <thread>
#include <vector>
#include <cstdlib>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>
#include <queue>
#include <mutex>
#include "semaphore.h"

/**
 Runs the game master logic. Manage semaphore access to dice queue. Add numbers to dice queue.
 @param threadCount is the number of player threads that were spun
 */
void gameMaster(int threadCount);

/**
 Runs the player logic. Tries to get access to semaphore. Then pulls
 */
void gamePlayer();

std::vector<std::thread> players;
std::queue<int> diceRolls;
std::queue<std::thread::id> podium;
std::mutex diceLock;
std::mutex podiumLock;
std::mutex ioLock;
Semaphore sem = Semaphore(0);

/**
 Parses command line input for number of threads. Creates game master and player threads, then joins them. Once all threads return, it prints out the results of the race.
 @param argc is the number of arguments 
 @param argv is an array of C strings that are the arguments
 @return 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    // Parse command line arguments
    if(argc == 1){
        std::cerr << "ERROR: Missing required argument" << std::endl;
        std::cerr << "USAGE: " << argv[0] << " <threadCount>" << std::endl;
        return 1;
    }
    if(argc > 2){
        std::cerr << "ERROR: Too many arguments" << std::endl;
        std::cerr << "USAGE: " << argv[0] << " <threadCount>" << std::endl;
        return 1;
    }
    int threadCount = atoi(argv[1]);
    if(threadCount < 1){
        std::cerr << "ERROR: threadCount must be greater than 0" << std::endl;
        return 1;
    }

    // Spin player threads
    std::cout << "Starting Race with " << threadCount << " threads." << std::endl;
    for(int i = 0; i < threadCount; i++){
        players.push_back(std::thread(gamePlayer));
    }
    // Start game master
    std::thread controller(gameMaster, threadCount);

    // Join all threads
    for(auto &p : players) p.join();
    controller.join();

    // Output results of race
    int i = 1;
    while(podium.size() != 0){
        std::cout << i++ << ": " << podium.front() << std::endl;
        podium.pop();
    }

    return 0;
}

void gameMaster(int threadCount){
    int diceCount;
    int diceRoll;

    // Seed RNG
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);

    // Create 3 RNG engines bound to functions for the various distributions needed
    std::uniform_int_distribution<int> dis0_5(0,5);
    auto getDiceCount = std::bind(dis0_5, gen);
    std::uniform_int_distribution<int> dis1_6(1,6);
    auto rollD6 = std::bind(dis1_6, gen);
    std::uniform_int_distribution<int> dis0_2000(0,2000);
    auto getSleep = std::bind(dis0_2000, gen);

    // Repeat until all players are done
    while(podium.size() < threadCount){
        // Lock dice queue and add numbers
        {std::lock_guard<std::mutex> guard(diceLock);
            diceCount = getDiceCount();
            for(int i = 0; i < diceCount; i++){
                diceRoll = rollD6();
                diceRolls.push(diceRoll);
            }
        }
        // Alert players that there are rolls available
        sem.signal();
        std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    }
}

void gamePlayer(){
    int position = 0;
    int roll;
    std::thread::id tid = std::this_thread::get_id();

    // Create RNG engine bound to function for random sleep timing
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis0_2000(0,2000);
    auto getSleep = std::bind(dis0_2000, gen);

    // Loop until thread crosses finish line
    while(position < 20){
        sem.wait();
        if(diceRolls.size() != 0){
            // Once semaphore is acquired, lock dice queue and pull out move
            {std::lock_guard<std::mutex> guard(diceLock);
                if(position == 0){
                    {std::lock_guard<std::mutex> guard(ioLock);
                        std::cout << "Thread " << tid << " has left the gate." << std::endl;
                    }
                }
                roll = diceRolls.front();
                diceRolls.pop();
                position += roll;
                {std::lock_guard<std::mutex> guard(ioLock);
                    std::cout << "Thread " << tid << " moved forward " << roll << " spaces." << std::endl;
                }
            }
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    }

    // Announce being finished and add TID to podium queue
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Thread " << tid << " has crossed the finish line." << std::endl;
    }

    podiumLock.lock();
    podium.push(tid);
    podiumLock.unlock();
}

