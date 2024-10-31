# CS361 Midterm Thread Puzzle

This program solves Question 4 of the midterm. See documentation and read the response in the PDF for more information.

## Building & Running 

There is a makefile. The program can be built with 'make'

It will generate a binary to `./bin/threadpuzzle`

The program can be ran with:
`./bin/threadpuzzle <numDrinkers> <kegSize> <numKegs>`

Where `numDrinkers` is the number of attendees drinking from the keg
      `kegSize` is the number of drink servings that fit in a keg
      `numKegs` is the number of kegs that are available for the party

To generate doxygen documentation, run `make doc`.

To clean the build files, run `make clean`.

To execute a demo, run `make test`.
