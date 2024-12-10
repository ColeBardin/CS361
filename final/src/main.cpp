/**
 @mainpage CS361 Final - Jurassic Park Safari Ride 
 @section Description

 This program simulates a Jurassic Park Safari ride and demonstrates inter-thread communicaiton. 
 There are people threads, car threads, and a dinosaur thread. 
 People wander around the park and get in line for the ride. When a car is available they ride and then go home.
 The dinosaur tries to escape. If it does, it eats all people not in cars.
*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date December 7th, 2024
 @section Description
 
 Main file of the program. It contains global variables for the threads. It also has the thread functions for people, cars, and the dinosaur.
 */

#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <queue>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>
#include <cstdlib>
#include "channel.h"

bool end; /* Signal to car threads and dinosaur thread that all people threads are done */
bool dinosaurEscaped; /* Variable to say the state of the dinosaur's escape plan */
std::mutex ioLock; /* Protects the IO stream*/
std::mutex lineLock; /* Protects the line queue */
std::queue<channel<int> *> line; /* Line for the safari ride */

/*
 @brief thread body for people. They wander around the park, then get in line, then ride, then go home.
 @param id is the person's number
 */
void peopleThread(int id);

/*
 @brief car thread body. Waits for people to get in line. Then rides them around. Will return when all people are gone
 @param id is the car's number
 */
void carThread(int id);

/**
 @brief dinosaur thread body. Has a 1 in 10 chance of escaping every 1-4 seconds.
 */
void dinosaurThread();

/**
 Main function. Handles command line inputs. 
 @param argc is the number of command line arguments
 @param argv is an array of command line arguments
 @returns 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    int i;
    // handle command line arguments
    if(argc != 3){
        std::cerr << "ERROR: Incorrect number of arguments" << std::endl;
        std::cout << "USAGE: " << argv[0] << " <numPeople> <numCars>" << std::endl;
        return 1;
    }
    int numPeople = atoi(argv[1]);
    if(numPeople < 1){
        std::cerr << "ERROR: Failed to parse numCars, value must be greater than 1" << std::endl;
        return 1;
    }
    int numCars = atoi(argv[2]);
    if(numCars < 1){
        std::cerr << "ERROR: Failed to parse numCars, value must be greater than 1" << std::endl;
        return 1;
    }

    // Tell the user they chose a boring setup
    if(numCars >= numPeople) std::cout << "Boring! Try with more people than cars next time..." << std::endl << std::endl;

    // Announce program stats
    std::cout << "Starting Jurassic Park Safari Ride" << std::endl;
    std::cout << "Number of safari cars:    " << numCars << std::endl;
    std::cout << "Number of park attendees: " << numPeople << std::endl;
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    
    // Set defaut state variables
    end = false;
    dinosaurEscaped = false;

    // Spin threads
    std::vector<std::thread> people;
    for(i = 0; i < numPeople; i++) people.push_back(std::thread(peopleThread, i));
    std::vector<std::thread> cars;
    for(i = 0; i < numCars; i++) cars.push_back(std::thread(carThread, i));
    std::thread dinosaur(dinosaurThread);

    // Wait for people to be done
    for(auto &p : people) p.join();
    // Signal end of program
    end = true;
    // Wait for car threads and dinosaur thread to exit
    for(auto &c : cars) c.join();
    dinosaur.join();
    std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
    std::cout << "Program End" << std::endl;

    return 0;
}

void peopleThread(int id){
    // Create RNG engine bound to function for random sleep timing
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis500_2000(500,2000);
    auto getSleep = std::bind(dis500_2000, gen);

    channel<int> *ticket = makeChannel<int>(0);
    int car;

    // Walk around park
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " is wandering around the park" << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));

    // Get in line
    {std::lock_guard<std::mutex> guard(lineLock);
        line.push(ticket);
        {std::lock_guard<std::mutex> guard(ioLock);
            std::cout << "Person " << id << " got in line" << std::endl;
        }
    }
    // Wait for an open car thread
    car = ticket->receive();
    // Dinosaur eats people in line and trying to get in cars
    if(dinosaurEscaped) goto die;
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " got in a car " << car << std::endl;
    }
    // Alert car that we've gotten in
    ticket->send(id);
    // Wait for ride to be over then get out
    ticket->receive();
    // Dinosaur can't catch cars, so steal it
    if(dinosaurEscaped){
        {std::lock_guard<std::mutex> guard(ioLock);
            std::cout << "Person " << id << " stole the car to escape from the dinosaur!" << std::endl;
        }
        return;
    }
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " got out of car " << car << std::endl;
    }
    // Walk to the exit
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    // Dinosaur can catch them on the way out
    if(dinosaurEscaped) goto die;
    // Go home safely
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " went home" << std::endl;
    }
    return;
die:
    // Print out death message
    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " was eaten by the dinosaur!" << std::endl;
    }
    return;

}

void carThread(int id){
    // Create RNG engine bound to function for random sleep timing
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis1000_2000(1000,2000);
    auto getSleep = std::bind(dis1000_2000, gen);

    channel<int> *c;
    int person;

    // Repeat until the program is over
    while(!end){
        // Get next person in line
        {std::lock_guard<std::mutex> guard(lineLock);
            // Check if anyone is in line
            if(line.empty()) c = NULL;
            else {
                // Get channel for next person
                c = line.front();
                line.pop();
            }
        }
        if(c){
            // Alert person that they are out of line
            c->send(id);
            // If dinosaur escaped, people can't get in car
            if(dinosaurEscaped) continue;
            // Wait for them to get in car
            person = c->receive();
            // Drive around safari
            {std::lock_guard<std::mutex> guard(ioLock);
                std::cout << "Car " << id << " is driving person " << person << " around" << std::endl;
            }
            std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
            {std::lock_guard<std::mutex> guard(ioLock);
                std::cout << "Car " << id << " is returned with person " << person << std::endl;
            }
            // Alert person that the ride has ended
            c->send(id);
        }
        std::this_thread::yield();
    }
    return;
}

void dinosaurThread(){
    // Create RNG engine bound to function for random sleep timing
    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis1000_4000(1000,4000);
    auto getSleep = std::bind(dis1000_4000, gen);
    std::uniform_int_distribution<int> dis0_9(0,9);
    auto getOut = std::bind(dis0_9, gen);

    for(;;){
        std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
        // Try to escape
        if(end) return;
        if(getOut() == 0){
            {std::lock_guard<std::mutex> guard(ioLock);
                std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
                std::cout << "ROAR!!!!! Dinosaur escaped" << std::endl;
                std::cout << "~~~~~~~~~~~~~~~~~~~~~~~~" << std::endl;
            }
            dinosaurEscaped = true;
            return;
        }
    }
}
