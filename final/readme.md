# CS361 Final Assessment Threading Puzzle - Jurassic Park Safari Ride

## Program Design

Since in homework 8, I only used channels as locks and I did not actually send useful data back and forth, I decided to use channels as intended.

There are car threads and people threads. People threads wander around the park for a bit, then they get in line for the safari ride. The line is a queue of integer channel pointers. People threads are 'in line' by putting their channel in the queue. Car threads try to pull channels from the queue, this simulates a car being available. After getting in line, people will try to receive from their unbuffered channel. The car that they get will send the car's ID number. They use this channel to negotiate that the car is available, when the person gets in, and then when the car has finished its ride. Then the person can get out and walk to the exit and go home.

There is also a dinosaur thread. The dinosaur thread tries to escape. If it does, it eats everyone waiting in line and walking to the exit. People in safari cars will steal them to save their lives.

## Building

There is a provided `makefile` that will build the program and create an executable to `bin/safari`.

There are a couple of included targets:

`make` will build the program.

`make run` will build the program and run it with 16 people and 3 cars.

`make doc` will build doxygen and latex documentation.

`make clean` will remove all build files and executables.

## Running

There are 2 required command line arguments:

`numPeople` is the number of park attendees.

`numCars` is the number of cars available at the park.

They both must be integers greater than 0. It is recommended to have more people than cars.
