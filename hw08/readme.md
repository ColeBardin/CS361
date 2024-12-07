# CS361 HW08 - Channels & File Search

## 1

Cole Bardin - cab572

## 2: Instructions

### Building

Provided is a `makefile` that will build the program to `bin/search`.

`make` will build the program.

`make doc` will build doxygen and latex documentation.

`make clean` will remove all build and object files.

### Running

The usage is: `bin/search targetWord [startingDirectory]`

`targetWord` is the word to search for in the files. It is required

`startingDirectory` is an optional starting directory. If not provided, the program will use `.` as the starting directory.

## 3: Short Essay 1

Besides modifying the thread pool class and search task class, I did not make any other data structures. I try to simplify my code and not make unnecessary data structures whenever possible. I could have recreated the lock guard with a channel, but since I only used 2 channels, I didn't find it necessary.

## 4: Short Essay 2

The thread pool gets its tasks from the producer thread. When the task queue is empty, the worker threads yield, allowing the producer to have a higher chance of getting CPU time. The producer iterates through the directory and adds tasks to the queue while the workers pop off tasks from the queue.

## 5: Short Essay 3

Once the producer has added all the files in the form of tasks, the producer will add N poison pills to the queue, where N is the number of threads. Since the queueu is first come first serve, the real tasks will be handled first. But once a worker receives a poison pill, they will return. This makes it so no worker consumes two poison pills. Then once the producer joins all the workers, the program knows the thread pool has fully stopped.

## 6: Short Essay 4

I used two channels to create thread safety. They were both initialized as a buffered channel with 1 entry and a message already in the channel. This allowed them to be used as locks. To acquire the lock, threads call receive(), and to release it they can send() anything. One lock, which is created in the producer thread body, is passed to all of the search task instances. This is used to protect the IO. The second lock is owned by the thread pool instance and is used to manage access to the queue. All thread pool functions that access the queue must first acquire the lock.

## 7: Short Essay 5

Thinking about my previous assignments, I prefer using locks and semaphores. I have done a lot of FreeRTOS programming and I think that the best concurrent/parallel code is written in a way where threads cannot lock each other up. That is partially because I have written code for automotive purposes where I need several threads collecting data while other threads verify the data. But I personally find that having a single writer, multiple reader architecture is typically safer and is less prone to mess ups. But that was because the code I was writing had predefined threads that all do different tasks. Overall, this class gave me many new tools and perspectives on concurrent programming in C++.

