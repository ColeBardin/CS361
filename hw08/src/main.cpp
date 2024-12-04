/**
 @file
 @author Cole Bardin <cab572@drexel.edu>
 @date December 4th, 2024
 @section Description
 
 Main file of program. This program uses a thread pool to search file contents for a target word. The producer task spins a thread pool and finds files for the pool to check.
 */
#include <iostream>
#include <filesystem>
#include <fstream>
#include <thread>
#include <string>
#include <array>

#include "threadPool.h"
#include "searchTask.h"

/**
 Producer thread body. Creates thread pool. Finds files to search. Stops thread pool.
 @param target is the word to search for
 @param dir is the starting directory
 */
void producerTask(std::string target, std::string dir);

/**
 Determines if a file is worth searching. The file must be a regular file and have the correct extension.
 @param file is the directory entry to check
 @return true if the file meets the requirements, false otherwise
 */
bool fileIsWorthy(std::filesystem::directory_entry file);

/**
 Main function. Checks command line arguments and spins producer thread
 @param argc is the number of command line arguments
 @param argv is an array of string pointers of the command line arguments
 @return 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    // Verify the correct number of arguments were passed
    if(argc < 2 || argc > 3){
        std::cerr << "ERROR: Incorrect number of arguments." << std::endl;
        std::cout << "USAGE: " << argv[0] << " <target_string> [directory]" << std::endl;
        return 1;
    }
    // Handle command line arguments
    std::string dir;
    std::string target;
    target = std::string(argv[1]);
    if(argc == 2) dir = "."; 
    else dir = std::string(argv[2]);
    // Make sure the given starting directory path exists and is a directory
    std::filesystem::path start_dir(dir);
    if(!std::filesystem::exists(start_dir)){
        std::cerr << "ERROR: Given directory '" << dir << "' does not exist." << std::endl;
        return 1;
    }
    if(!std::filesystem::is_directory(start_dir)){
        std::cerr << "ERROR: Given directory '" << dir << "' is not a directory." << std::endl;
        return 1;
    }
    // Create and run producer thread
    std::thread producer(producerTask, target, dir);
    producer.join();

    return 0;
}

void producerTask(std::string target, std::string dir){
    // Init channel used for IO locking
    channel<int> * ioCh = makeChannel<int>(1);
    ioCh->send(0);
    int threads = std::thread::hardware_concurrency();
    std::cout << "!−−−− Search Started −−−−!" << std::endl;
    std::cout << "Target Folder: " << dir << std::endl;
    std::cout << "Target Text: " << target << std::endl;
    std::cout << "Using a Pool of " << threads << " threads to search." << std::endl;
    // Create thread pool
    threadPool* pool = new threadPool();
    // Start threads while searching for files
    pool->start();
    // Recursively iterate through starting directory
    for(const std::filesystem::directory_entry& file : std::filesystem::recursive_directory_iterator(dir)){
        // add file tasks to pool
        if(fileIsWorthy(file)) pool->addWork(new searchTask(target, file, ioCh));
    }
    // Stop the thread pool and wait for all threads to finish
    pool->stop();
    std::cout << "!−−−− Search Complete −−−−!" << std::endl;
    // Close io lock channel
    ioCh->close();
    delete ioCh;
    return;
}

bool fileIsWorthy(std::filesystem::directory_entry file){
    // Ignore directories or other files
    if(!file.is_regular_file()) return false;

    std::string fileStr = file.path().string();
    std::array<std::string, 9> extensions = 
        {".cc", ".c", ".cpp", ".h", ".hpp", ".pl", ".sh", ".py", ".txt"};
    // Check to see if file has one of the above extensions
    for(auto &ext : extensions){
        if(std::equal(ext.rbegin(), ext.rend(), fileStr.rbegin())) return true;
    }
    // Did not find a match
    return false;
}

