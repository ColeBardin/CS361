/**
 @mainpage CS361 - Race - HW04
 @section Description

*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date October 23, 2024
 @section Description
 
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

void gameMaster(int threadCount);
void gamePlayer();

std::vector<std::thread> players;
std::queue<int> diceRolls;
std::queue<std::thread::id> podium;
std::mutex diceLock;
std::mutex podiumLock;
std::mutex ioLock;

/**
 Run all tests then run timings.
 @param argc is the number of arguments 
 @param argv is an array of C strings that are the arguments
 @return 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
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
        std::cerr << "ERROR: threadCount must be greater than 1" << std::endl;
        return 1;
    }

    std::cout << "Starting Race with " << threadCount << " threads." << std::endl;
    for(int i = 0; i < threadCount; i++){
        players.push_back(std::thread(gamePlayer));
    }

    std::thread controller(gameMaster, threadCount);

    for(auto &p : players) p.join();
    controller.join();

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

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);

    std::uniform_int_distribution<int> dis0_5(0,5);
    auto getDiceCount = std::bind(dis0_5, gen);

    std::uniform_int_distribution<int> dis1_6(1,6);
    auto rollD6 = std::bind(dis1_6, gen);

    std::uniform_int_distribution<int> dis0_2000(0,2000);
    auto getSleep = std::bind(dis0_2000, gen);

    while(podium.size() < threadCount){
        diceCount = getDiceCount();
        diceLock.lock();
        for(int i = 0; i < diceCount; i++){
            diceRoll = rollD6();
            diceRolls.push(diceRoll);
        }
        diceLock.unlock();
        std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    }
}

void gamePlayer(){
    int position = 0;
    int roll;
    std::thread::id tid = std::this_thread::get_id();

    auto currentTime = std::chrono::high_resolution_clock::now();
    auto clock = currentTime.time_since_epoch();
    int ticks = clock.count();
    std::default_random_engine gen(ticks);
    std::uniform_int_distribution<int> dis0_2000(0,2000);
    auto getSleep = std::bind(dis0_2000, gen);

    while(position < 20){
        if(diceRolls.size() != 0){
            if(position == 0){
                ioLock.lock();
                std::cout << "Thread " << tid << " has left the gate." << std::endl;
                ioLock.unlock();
            }

            diceLock.lock();             
            roll = diceRolls.front();
            diceRolls.pop();
            position += roll;
            ioLock.lock();
            std::cout << "Thread " << tid << " moved forward " << roll << " spaces." << std::endl;
            ioLock.unlock();
            diceLock.unlock();
        }
        std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));
    }

    ioLock.lock();
    std::cout << "Thread " << tid << " has crossed the finish line." << std::endl;
    ioLock.unlock();

    podiumLock.lock();
    podium.push(tid);
    podiumLock.unlock();
}

