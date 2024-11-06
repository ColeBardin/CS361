# CS361 - HW05 Mandelbrot

## Description

This project solves the Mandelbrot set for a given region and outputs a bitmap file. 

## Building

The included make file will build the files in `src`. It will output the executable to `bin/mandelbrot`.

There are a handful of make targets included:

`make mandelbrot` builds the program

`make clean` removes temporary and output files

`make doc` builds doxygen documentation

`make e0.bmp`, `make e1.bmp`, `make e2.bmp`, and `make e3.bmp` build the program and run a test set.

## Running

Usage: `./bin/mandelbrot <x1> <x2> <y1> <y2> <filename.bmp>'

x1 is the lower bound of x values

x2 is the uppwer bound of x values

y1 is the lower bound of y values

y2 is the uppwer bound of y values

filename.bmp is the bitmap file that the program will output
