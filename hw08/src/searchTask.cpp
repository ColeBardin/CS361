/**
 @file
 @author Mark Boady <mwb33@drexel.edu>
 @date Orgininal: 2021, Revised: 2024
 @section Description
 
 Implementation of search tasks.
 */
#include "searchTask.h"
#include <iostream>
#include <fstream>
#include <mutex>

searchTask::searchTask(std::string target, std::filesystem::directory_entry file, channel<int> * ioCh){
    myTarget = target;
    myPath = std::filesystem::path(file.path());
    ioLk = ioCh;
}

searchTask::~searchTask(){
    // Nothing to do
}

std::queue<task*>*  searchTask::runTask(){
    std::thread::id tid = std::this_thread::get_id();
    std::ifstream infile(myPath);
    std::string lineString;
    int line = 0;
    // Search each line
    while(std::getline(infile, lineString)){
        line++;
        // Check if this line contains the target string
        if(lineString.find(myTarget) != std::string::npos){
            // Print the find
            announceFind(tid, myPath.string(), line, lineString);
        }
    }
    // Return empty queue
    return NULL;
}

void searchTask::announceFind(std::thread::id tid, std::string file, int lineNumber, std::string line){
    // Acquire access and print out find
    ioLk->receive();
    std::cout << "−−−−−−−−−−" << std::endl;
    std::cout << "Thread " << tid << " found a match." << std::endl;
    std::cout << "File: \"" << file << "\"" << std::endl;
    std::cout << "Line " << lineNumber << ": " << line << std::endl;
    std::cout << "−−−−−−−−−−" << std::endl;
    ioLk->send(0);
}
