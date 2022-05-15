## Disk Scheduling Simulator V1.0

# Program Overview

- Implementation of 6 disk scheduling algorithms. 

- Program takes in an input file containing service request and calculates 
  the total seek time for each scheduling algorithm

- Input file format: 
  <total cylinders> <current read/write head position> <previous disk request> <request data>
  (Note that each value needs to be seprated by a single space)

- Once computation is complete the program waits for another input file.

- Users can provide the "QUIT" command to exit the program (case insensitive).

# Directory/File Overview
.
|-- src
    |-- algorithms.c -> Contains all disk scheduling algorithms.
    |-- algorithms.h -> Contains function declarations and constants for algorithms.c.
    |-- dataExtract.c -> Contains code for File IO and user input.
    |-- dataExtract.h -> Contains the function declarations for dataExtract.c
    |-- scheduler.c -> Contains the driver code for the scheduling simulator.
|-- Makefile -> Build program and create "scheduler" executable
|-- README.txt -> About program
|-- input.txt -> Sample test data 1 provided in the assignment specification.
|-- input2.txt -> Sample test data 2 
|-- input3.txt -> Sample test data 3 with insufficient data.

C libraries utilized:
  <stdio.h>, <stdlib.h>, <stdbool.h>, <string.h>

# How to run the Program

Step 1: 
cd into the "program1" directory.

Step 2: 
run `make` command
This will build the program and create a folder called "bin" containing all the object files and 
an executable called "scheduler" in the current directory.

Step 3:
Run executable to start the program.
command: `./scheduler`

Step 4 (Optional)
Run `make clean` to delete the bin folder containing the object files and "scheduler" 
executable from the directory.

(Note: Developed using C11 Standard)
