# CS361 Homework 07

## 1. Name

Cole Bardin - cab572

## 2. Instructions

### Building

There is a provided makefile with a few targets. It outputs the executable to `bin/island`.

`make` will build the program.

`make run` will build the program and run `bin/island 7 9`

`make clean` will remove all compiled code

`make doc` will make documentation

### Running 

The usage is as follows:

`island <nAdults> <nChildren>`

There are two mandatory command line arguments:

`nAdults` is the number of adult threads.

`nChildren` is the number of child threads.

## 3. Short essay 1

I used mutexes with lock guards to protect the driver's and passenger seat. I used locks to ensure only one thread is the driver's or passenger seat at a time. With appropriate logic, this is the bare minimum required to protect the boat.

## 4. Short essay 2

All threads, except for the chosen one, are lazy and will only be the driver. Once they gain permission to leave, they will always get in the driver's seat. The chosen one will always get in the passenger seat at the island and the driver's seat at the mainland.

## 5. Short essay 3

The chosen one will always drive the boat back to the island. There, they will pick up the next lazy person.

## 6. Short essay 4

I am certain everyone will get off the island because the algorithm is very simple. Every lazy person waits to get permission to leave and drives the boat to the mainland. The chosen one gets a ride to the mainland. If there are people left on the island, the chosen one will drive the boat back to the island. After the last person is brought back, the chosen one can finish.

## 7. Short essay 5

Nothing was very challenging in my opinion. Deciding when to use locks or semaphores and making sure everything was protected was the most challenging.

