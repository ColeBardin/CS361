/**
 @mainpage CS361 - HW07 
 @section Description
 This program simulates many independent threads trying to use the same resource. It emulates children and adults stuck on an island with 1 boat. All people must get back to the mainland but there are conditions on how the boat can be used:
 - there are 2 seats, driver and passenger
 - 2 adults cannot be in the boat at once
 - only 1 child and 1 adult or 2 children can be in the boat at a time
 - a passenger is not necessary, 1 child or 1 adult can drive the boat alone
 - no one person can drive the boat more than 4 times in a row

 The threads must share access to the boat and work together to get everyone to the mainland.
*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date November 18, 2024
 @section Description
 This is the main file of the program. It contains the main function as well as the thread functions for lazy people and the chosen one. It also contains a helper function to row the boat.
 */

#include <iostream>
#include <thread>
#include <mutex>
#include <vector>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>
#include <cstdlib>
#include "semaphore.h"

std::mutex ioLock; // Protects the IO stream
std::mutex driverLock; // Protects the driver's seat
std::mutex passengerLock; // Protects the passenger seat
Semaphore boatAtMainland(0); // Used for lazy people to tell chosen one boat is at mainland
Semaphore passengerReady(0); // Used for chosen one to tell lazy people they're ready
Semaphore permissionToLeave(0);

std::vector<std::thread> adults;
std::vector<std::thread> children;

// Program statistics
int peopleLeft;
int boatsToMainland;
int boatsToIsland;
int boatsWith2Children;
int boatsWith1Child1Adult;
int boatsWith1Person;
int adultDrivers;
int childDrivers;

/**
 Task function to be lazy. Only will drive boat from island to mainland.
 All lazy people get in line to go to the mainland. They drive the chosen one and then return.
 @param i is the Child or Adult index
 @param adult tells the function if it is a child or an adult
 */
void beLazy(int i, bool adult);

/**
 Task function for the chosen child. They are solely responsible for driving the boat back to the island every time. They get to rest on the way to the mainland while the lazy person drives.
 @param i is the Child index
 */
void doEverything(int i);

/**
 Simulates rowing the boat to or from the island.
 Makes announcement for direction and delays for 1 to 4 seconds.
 */
void rowBoat();

/**
 Main function. Handles command line inputs. Spins and joins child and adult threads. Then prints out statistics.
 @param argc is the number of command line arguments
 @param argv is an array of command line arguments
 @returns 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    int i;
    int nAdults, nChildren;

    // Handle command line inputs
    if(argc != 3){
        std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
        std::cout << "Usage: " << argv[0] << " <nAdults> <nChildren>" << std::endl;
        return 1;
    }
    nAdults = atoi(argv[1]);
    if(nAdults < 1){
        std::cerr << "ERROR: nAdults must be greater 0" << std::endl;
        return 1;
    }
    nChildren = atoi(argv[2]);
    if(nChildren < 1){
        std::cerr << "ERROR: nChildren must be greater 0" << std::endl;
        return 1;
    }

    // Init stats
    peopleLeft = nAdults + nChildren - 1;
    boatsToMainland = 0;
    boatsToIsland = 0;
    boatsWith2Children = 0;
    boatsWith1Child1Adult = 0;
    boatsWith1Person = 0;
    adultDrivers = 0 ;
    childDrivers = 0;

    // Start adults, they're lazy
    for(i = 0; i < nAdults; i++) adults.push_back(std::thread(beLazy, i, true));
    // Start the chosen child to do all the work
    children.push_back(std::thread(doEverything, 0));
    // Start remaining children, they're lazy
    for(i = 1; i < nChildren; i++) children.push_back(std::thread(beLazy, i, false));
    // Join all threads
    for(auto &a : adults) a.join();
    for(auto &c : children) c.join();

    // Print out final stats
    std::cout << "Summary of Events" << std::endl;
    std::cout << "Boat traveled to the mainland: " << boatsToMainland << std::endl;
    std::cout << "Boat traveled to the island: " << boatsToIsland << std::endl;
    std::cout << "Boats with 2 children: " << boatsWith2Children << std::endl;
    std::cout << "Boats with 1 child and 1 adult: " << boatsWith1Child1Adult << std::endl;
    std::cout << "Boats with only 1 person (child or adult): " << boatsWith1Person << std::endl;
    std::cout << "Times adults were the driver: " << adultDrivers << std::endl;
    std::cout << "Times children were the driver: " << childDrivers << std::endl;

    return 0;
}

void beLazy(int i, bool adult){
    // Wait in line to be next person
    permissionToLeave.wait();
    // Wait for nobody to be in the drvier's seat
    {std::lock_guard<std::mutex> guard(driverLock);
        {std::lock_guard<std::mutex> guard(ioLock);
            std::cout << (adult ? "Adult " : "Child ") << i << " got into the driver's seat of the boat." << std::endl;
        }
        // Wait for child to get in passenger seat
        passengerReady.wait();
        // Drive boat to mainland 
        rowBoat();
        if(adult){
            boatsWith1Child1Adult++;
            adultDrivers++;
        }else{
            boatsWith2Children++;
            childDrivers++;
        }
        peopleLeft--;
        // Let chosen one know boat has arrived at mainland
        boatAtMainland.signal();
    }
}

void doEverything(int i){
    for(;;){
        // Get the next person to go home
        permissionToLeave.signal();
        // Get in passenger seat
        {std::lock_guard<std::mutex> guard(passengerLock);
            {std::lock_guard<std::mutex> guard(ioLock);
                std::cout << "Child " << i << " got into the passenger seat of the boat." << std::endl;
            }
            // Let driver know it's time to go
            passengerReady.signal();
            // Wait for boat to arrive at mainland
            boatAtMainland.wait();
        }
        {std::lock_guard<std::mutex> guard(driverLock);
            // Check if more people at island
            if(peopleLeft > 0){
                {std::lock_guard<std::mutex> guard(ioLock);
                    std::cout << "Child " << i << " got into the driver's seat of the boat." << std::endl;
                }
                // Drive back to island
                rowBoat();
                boatsWith1Person++;
                childDrivers++;
            }else return; // Otherwise, it's over
        } 
    }
}

void rowBoat(){
    // Seed RNG
    static auto currentTime = std::chrono::high_resolution_clock::now();
    static auto clock = currentTime.time_since_epoch();
    static int ticks = clock.count();
    static std::default_random_engine gen(ticks);
    static std::uniform_int_distribution<int> dis1000_4000(1000,4000);
    static auto getSleep = std::bind(dis1000_4000, gen);
    static bool boatAtIsland = true;

    {std::lock_guard<std::mutex> guard(ioLock);
        if(boatAtIsland){
            std::cout << "Boat is traveling from island to mainland." << std::endl;
            boatsToMainland++;
        }else{
            std::cout << "Boat is traveling from mainland to island." << std::endl;
            boatsToIsland++;
        }
    }
    // Delay to emulate travel time
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    boatAtIsland = !boatAtIsland;
}

