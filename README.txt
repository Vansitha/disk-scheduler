## Disk Scheduling Simulator V2.0 (With Thread Support)

# Program Overview

- Implementation of 6 disk scheduling algorithms that run in simultaenously to compute 
  the seek times for each algorithm.

- Input file format: 
  <total cylinders> <current read/write head position> <previous disk request> <request data>
  (Note that each value needs to be seprated by a single space)

- Once computation is complete the program waits for another input file.

- Users can provide the "QUIT" command to exit the program (case insensitive).
  This will cancel all threads and print out the thread ID's of each thread that was created.

# Directory/File Overview
.
|-- src
    |-- algorithms.c -> Contains all disk scheduling algorithms.
    |-- algorithms.h -> Contains function declarations, constants for algorithms.c and 
                        struct for the shared data
    |-- dataExtract.c -> Contains code for File IO and user input.
    |-- dataExtract.h -> Contains the function declarations for dataExtract.c
    |-- simulator.c -> Contains the driver code and creates threads to run the algorithms.
|-- Makefile -> Build program and create "scheduler" executable
|-- README.txt -> About program
|-- input.txt -> Sample test data 1 provided in the assignment specification.
|-- input2.txt -> Sample test data 2 
|-- input3.txt -> Sample test data 3 with insufficient data.

C libraries utilized:
  <stdio.h>, <stdlib.h>, <pthread.h>, <stdbool.h>, <string.h>

# How to run the Program

Step 1: 
cd into the "program2" directory.

Step 2: 
run `make` command
This will build the program and create a folder called "bin" containing all the object files and 
an executable called "simulator" in the current directory.

Step 3:
Run executable to start the program.
command: `./simulator`

Step 4 (Optional)
Run `make clean` to delete the bin folder containing the object files and "simulator" 
executable from the directory.

(Note: Developed using C11 Standard)