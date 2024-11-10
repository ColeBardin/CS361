#include <iostream>
#include <thread>
#include <vector>
#include <mutex>
#include <random>
#include <chrono>
#include <time.h>
#include <functional>
#include <cstdlib>
#include "semaphore.h"

void eaterTask(int id);

std::mutex ioLock;
Semaphore seats = Semaphore(5);

int main(int argc, char **argv){
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

    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " woke up from their nap." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));

    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " got to the noodle bar." << std::endl;
    }
    seats.wait();

    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " started eating." << std::endl;
    }
    std::this_thread::sleep_for(std::chrono::milliseconds(getSleep()));

    {std::lock_guard<std::mutex> guard(ioLock);
        std::cout << "Person " << id << " finished eating and is heading home." << std::endl;
    }
    seats.signal();

    return;
}
