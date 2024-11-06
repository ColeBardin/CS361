/**
 @mainpage CS361 - HW05 
 @section Description
 Threaded Mandelbrot set calculator. Uses 10 threads to generate an image of the Mandelbrot set for a given x and y range.
*/

/**
 @file
 @author Cole Bardin <cab572 @drexel.edu>
 @date October 23, 2024
 @section Description
 Contains main function of program. Main function will spin the boss thread who will then spin worker threads to perform calculations and create the image. 
 */

#include <iostream>
#include <fstream>
#include <thread>
#include <vector>
#include <complex>
#include <cstdlib>
#include <cstring>
#include "bmp.h"

int width = -1;
int height = -1;
long double _x1 = 0;
long double _x2 = 0;
long double _y1 = 0;
long double _y2 = 0;

/**
 Creates bitmap image object, spins and divides work to worker threads. Then saves the file after image is done.
 @param filename is the filename for the bitmap file
 */
void bossTask(char *filename);

/**
 Computes Mandelbrot set for given pixel indices. Then colors the image.
 @param image is the bitMap object to fill out
 @param pixelStart is the first pixel to calculate and paint
 @param pixelStop is the last pixel to calculate and paint
 */
void workerTask(bitMap *image, int pixelStart, int pixelStop);

/**
 Converts the pixel index value to the corresponding complex number in the image
 @param pixel is the pixel index to calculate
 @return the complex number object that corresponds to that pixel
 */
std::complex<long double> pixelToValue(int pixel);

/**
 Determines if a complex number is in the set. Calculates up to Zt to determine if the complex number diverges.
 @param c is a complex number to test
 @param t is the number of elements in the sequence to calculate up to.
 @return i such that Zi > 4 or t if no value before Zt > 4
 */
int z(std::complex<long double> &c, int t);

/**
 Handles input arguments. Then spins boss thread to carry out rest of program.
 @param argc is the number of command line arguments
 @param argv is an array of command line argument strings
 @return 0 on success, 1 otherwise
 */
int main(int argc, char **argv){
    // Handle input arguments
    if(argc != 6){
        std::cerr << "ERROR: Incorrect arguments" << std::endl;
        std::cout << "USAGE: " << argv[0] << " x1 x2 y1 y2 filename.bmp" << std::endl;
        return 1;
    }
    _x1 = atof(argv[1]);
    _x2 = atof(argv[2]);
    _y1 = atof(argv[3]);
    _y2 = atof(argv[4]);
    if(_x1 >= _x2){
        std::cerr << "ERROR: _x1 must be smaller than _x2" << std::endl;
        return 1;
    }
    if(_y1 >= _y2){
        std::cerr << "ERROR: _y1 must be smaller than _y2" << std::endl;
        return 1;
    }
    // Verify that filename ends with ".bmp"
    char *filename = argv[5];
    int filenameLength = strlen(filename);
    if(filenameLength <= 4 || strcmp(&filename[filenameLength - 4], ".bmp")){
        std::cerr << "ERROR: '" << filename << "' is not a valid filename. bmp extension is required" << std::endl;
        std::cout << "USAGE: " << argv[0] << " x1 x2 y1 y2 filename.bmp" << std::endl;
        return 1;
    }

    std::thread boss(bossTask, filename);
    boss.join();

    return 0;
}

void bossTask(char *filename){
    int nthreads = 10; 
    int dpi = 150;
    width = 1500;
    height = (_y2 - _y1) * width / (_x2 - _x1);
    std::vector<std::thread> workers;

    // Create image. Size based on input parameters
    bitMap* image = new bitMap(width, height);
    int pixels = image->numPixels();
    int pixelsPerThread = pixels / nthreads;

    // Divide image into rows and spin threads to do work
    for(int i = 0; i < nthreads; i++){
        int start = i * pixelsPerThread;
        int stop = (i + 1) * pixelsPerThread - 1;
        if(stop > pixels) stop = pixels - 1;

        workers.push_back(std::thread(workerTask, image, start, stop));
    }

    // Wait for workers to finish and save image
    for(auto &w : workers) w.join();
    image->saveImage(filename);
}

void workerTask(bitMap *image, int pixelStart, int pixelStop){
    int c = 0;
    int threshold = 100;
    int color;
    // Calculate if each pixel diverges, then color image accordingly
    for(int i = pixelStart; i < pixelStop; i++){
        std::complex<long double> c = pixelToValue(i);
        int iterations = z(c, threshold);
        color = 255 * iterations / threshold;
        pixel p(color * 0.7, 0, color);
        image->setColor(i, p);
    }
}

std::complex<long double> pixelToValue(int pixel){
    if(width == -1 || height == -1) return 0;
    // Calculate corresponding row and column for pixel index
    int row = pixel / width;
    int col = pixel % width;

    // Scale row and column to x and y bounds set by user
    long double real = (((_x2 - _x1) * col ) / width) + _x1;
    long double comp = (((_y2 - _y1) * row ) / height) + _y1;

    return std::complex<long double>(real, comp);
}

int z(std::complex<long double> &c, int t){
    // Modified z function from lecture
    // takes in complex number object instead of Real and Imag components
    std::complex<long double> current(0,0);
    for(int i = 1; i < t; i++){
        current = current * current + c;
        if(std::abs(current) > 4) return i;
    }
    return t;
}

