# CS361 HW01

### By cab572

## Building

There is an included `Makefile` that will build the files in `src` and `include`. Make will output the binary to `bin`.

There are Make targets to comply with the specified cases in the `monty.pdf` provided by the assignment.

## Documentation

There is also a Doxygen config file named `doxyconf`. This will create documentation and output it to `doc`.

## Program Structure

Source files are located in `src`, header files in `inc`. Make will not create intermediate object files because this project is so simple.

This program works by spinning C++ threads to divide the workload in two. This probably does not provide much of a performance benefit, but it's extra credit. The thread function `runTests` will seed its own RNG engine based on the algorithm provided in lecture. 

The threads simulate the Monty Hall problem a given number of times and then provide the number of times a Switch would result in a Win. Then `main()` accumulates the results and prints out the program statistics.
