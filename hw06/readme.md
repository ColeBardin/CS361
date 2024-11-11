# CS361 HW06 - Bar

By cab572@drexel.edu

## Building

Included in the project is a makefile. It will build the source code into an executable `bin/bar`

There are a handful of make targets for this project:

`make` will only compile the project.

`make test1` will compile the project and run it with 10 eaters.

`make test2` will compile the project and run it with 20 eaters.

`make test3` will compile the project and run it with 100 eaters.

`make doc` will compile doxygen documentation.

`make clean` will remove all generated files.

## Running

The program requires 1 command line argument.

Usage: `./bin/bar <nPeople>`

`nPeople` must be an integer greater than 0.

## Short Answer

### Question 1

I used 2 semaphores. I used one to emulate the bar seats, it is initialized to the number of bar seats. This allowed me to use a thread safe exclusion method to regulate access up to a certain number of threads. In this case, it was 5 seats at the bar. Threads call `wait()` to acquire a seat and `signal()` when they are done. The other semaphore is used as a lock around the IO stream. It is a binary semaphore and only acquired before printing and released after.

### Question 2

All threads call `wait()` on the semaphore. If there are no empty seats, then they wait until one does open. When they execute passed the `wait()` call, a seat was released and they acquired it.

### Question 3

People know the bar is full if they are blocked by their `wait()` call on the seat semaphore.

### Question 4

I am certain everyone will eat because there is no possibility of a deadlock. While a thread can acquire a seat and then IO privilege, the reverse cannot happen. Therefore deadlock is not possible. Without the possibility of deadlock, all the threads will execute their function. As the threads return, there is less and less competition for the locks.

### Question 5

Nothing was challenging about this assignment. My solution came very quickly to me and was quick to implement.
