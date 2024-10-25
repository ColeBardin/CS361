# CS361 HW04

## 1

Cole Bardin - cab572

## 2

Provided is a Makefile. To build the program run `make` in the root of the folder structure.

This will output the binary to `bin/race`.

The program takes 1 required command line argument, the number of player threads to spin. If the value provided is less than 1 or NaN, the program will exit.

There is an additional target to build and test the program. It can be invoked by calling `make run`. This will build the program and then run the program with 2, 4, and 10 threads.

Doxygen output can be created with `make doc`.

All generated files can be removed with `make clean`.

## 3

I used a semaphore to control access to the queue of dice rolls. The player threads wait for the semaphore to have a value. They are alerted by the game master thread. The game master thread increments the semaphore by the number of new numbers added to the queue. This allows all the values to be consumed while the game master is sleeping if interleaved properly. I used a semaphore because it can automatically wake up waiting threads when the game master signals.

### 4

I used a few locks. One lock was for the IO stream. This prevents interleaving of print statements. Another lock I used was for the dice queue. This stops threads from reading/popping from the queue while the game master tries to add new values. The last lock I used was for my podium queue. Threads finish the race and then add their TIDs to a queue. To prevent multiple threads from writing at once, I used a lock.

### 5

I am confident my program can never deadlock. When there are no values in the queue, the semaphore will have all the player threads halt, allowing the game master thread to add more values. Also, I only used the lock guards and never nested locks. This makes it so a thread can't acquire a lock and then be held up by another lock before releasing the first one. Putting all these together with a simple program concept allows me to be confident that it will not deadlock.

### 6

A starvation situation could happen if the queue is empty, but the game master is not allowed any CPU time to add more values because the player threads take up all the CPU time. I made sure this could not happen by using the semaphore in class. Only the game master thread can signal the semaphore and only the players can wait on it. There is nothing preventing the game master thread from placing new items in the queue except for the queue lock. However, the player threads will eventually use up the entire queue and then be waiting for the semaphore to be signaled. This guarantees that the game master thread will be able to acquire the queue lock and then add more values.

### 7

Nothing was exceptionally challening. But I did spend a good amount of time thinking about how to most effectively use the semaphore. I did not want the game master thread to only signal the semaphore once per loop. That would prevent the player threads from taking out more values from the queue while the game master is sleeping. So, I instead have the game master call signal for every value it adds to the queue.
