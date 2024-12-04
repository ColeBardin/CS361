/**
 @file
 @author Cole Bardin <cab572@drexel.edu>
 @date December 4th, 2024
 @section Description
 
 A task to search for a word in file contents.
 */
#ifndef _SEARCH_TASK_H_
#define _SEARCH_TASK_H_

#include <vector>
#include <filesystem>
#include <string>
#include <thread>
#include "task.h"
#include "channel.h"

/**
 This class searches for a word in a file. Instances are used as thread pool tasks
 */
class searchTask: public task{
private:
    std::filesystem::path myPath;/**< Path of the file to check*/
    std::string myTarget;/**< Word to search for in file*/
    channel<int>* ioLk;/**< Channel used to lock STDIO*/
public:
    /**
     Create a new task using the moves given.
     @param target is the string to look for
     @param file is the directory entry to search
     @param ioCh is the lock to use for printing
     */
    searchTask(std::string target, std::filesystem::directory_entry file, channel<int>* ioCh);
    /**
     Destory a task.
     */
    ~searchTask();
    /**
     Run the task (in a thread)
     @return NULL
     */
    std::queue<task*>* runTask();
private:
    /**
     Prints out the match found to STDOUT. Thead safe
     @param tid is the thread id that made the find
     @param file is the filename
     @param lineNumber is the line number
     @param line is the text line that contains the match
     */
    void announceFind(std::thread::id tid, std::string file, int lineNumber, std::string line);
};

#endif

