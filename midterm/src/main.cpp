/**
 @mainpage CS361 - Midterm Thread Puzzle
 @section Description

 Simulates a party with drinkers taking servings from kegs. A pledge must refill the keg once it runs out. The number of drinkers, servings per keg, and number of kegs are all supplied by command line inputs. Once all the kegs have been emptied, the pledge declares the party to be over.
*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date October 23, 2024
 @section Description
 The main file for the race program. It defines and implements the pledge and drinker thread functions. It also starts the party and announces the stats. 
 */

#include <iostream>
#include <thread>
#include <vector>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>
#include <mutex>
#include "semaphore.h"

/**
 Waits for semaphore alerts and attempts to refill keg. Once kegs have run out, declare the party as over.
 @param kegs is the number of kegs available
 @param kegSize is the number of servings per keg
 */
void pledgeFunc(int kegs, int kegSize);

/**
 Attempts to drink from the keg then sleeps for random amount of time. If they took the last drink, signal the pledge to refill it. Leave once the party is over.
 @param number is the thread index which is more human readable than TIDs
 */
void drinkerFunc(int number);

bool goHome = false;
int keg = 0;
std::vector<std::thread> drinkers;
std::mutex kegLock;
std::mutex ioLock;
Semaphore sem = Semaphore(1);

/**
 Parses command line input for number of drinkers, keg serving size, and number of kegs. Creates pledge and drinker threads, then joins them.
 @param argc is the number of arguments 
 @param argv is an array of C strings that are the arguments
 @return 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    // Parse command line arguments
    if(argc < 4){
        std::cerr << "ERROR: Missing required arguments" << std::endl;
        std::cerr << "USAGE: " << argv[0] << " <numDrinkers> <kegSize> <numKegs>" << std::endl;
        return 1;
    }
    if(argc > 4){
        std::cerr << "ERROR: Too many arguments" << std::endl;
        std::cerr << "USAGE: " << argv[0] << " <numDrinkers> <kegSize> <numKegs>" << std::endl;
        return 1;
    }
    int M = atoi(argv[1]);
    if(M < 1){
        std::cerr << "ERROR: numDrinkers must be greater than 0" << std::endl;
        return 1;
    }
    int N = atoi(argv[2]);
    if(N < 1){
        std::cerr << "ERROR: kegSize must be greater than 0" << std::endl;
        return 1;
    }
    int kegs = atoi(argv[3]);
    if(kegs < 1){
        std::cerr << "ERROR: numKegs must be greater than 0" << std::endl;
        return 1;
    };

    std::cout << "Party has started. " << M << " attendees." << std::endl;
    std::cout << "Pledge has " << kegs << " kegs that each have " << N << " servings" << std::endl << std::endl;

    // Spin pledge and drinker threads
    std::thread pledge(pledgeFunc, kegs, N);
    for(int i = 0; i < M; i++){
        drinkers.push_back(std::thread(drinkerFunc, i));
    }

    // Join all threads
    for(auto &d : drinkers) d.join();
    pledge.join();

    return 0;
}

void pledgeFunc(int kegs, int kegSize){
    while(1){
        // Wait for drinker to alert that keg is empty
        sem.wait();
        {std::lock_guard<std::mutex> guard(kegLock);
            // Verify keg is empty
            if(keg == 0){
                // Keg is empty and none are remaining, party is over
                if(kegs == 0) break; 
                // Refill keg
                keg = kegSize;
                kegs--;
                {std::lock_guard<std::mutex> guard(ioLock);
                    std::cout << "PLEDGE: Swapped keg. Still have " << kegs << std::endl;
                }
            }
        }
    }
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "PLEDGE: Party's over. Go home" << std::endl;
    }
    goHome = true;
}

void drinkerFunc(int number){
    // Create RNG engine bound to function for random sleep timing
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis0_2000(0,2000);
    auto getSleep = std::bind(dis0_2000, gen);

    // Drink until party is over
    while(!goHome){
        {std::lock_guard<std::mutex> guard(kegLock);
            // If there are servings left
            if(keg >= 1){
                // Take drink
                keg--;
                {std::lock_guard<std::mutex> guard(ioLock);
                    std::cout << number << ": Drank. Drinks left: " << keg << std::endl;
                }
                // Alert pledge if that was the last drink
                if(keg == 0){
                    {std::lock_guard<std::mutex> guard(ioLock);
                        std::cout << number << ": Finished the keg. Alering pledge" << std::endl;
                    }
                    sem.signal();
                }
            // Race condition
            }else if(goHome){
                {std::lock_guard<std::mutex> guard(ioLock);
                    std::cout << number << ": Failed to get the last drink" << std::endl;
                }
            }
            // Party is going but keg is empty, release lock for pledge
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    }
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << number << ": Going home :(" << std::endl;
    }
}

