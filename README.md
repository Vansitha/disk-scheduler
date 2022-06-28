## Disk Scheduling Simulator V1.0

# Program Overview

- Implementation of 6 disk scheduling algorithms. 

- Program takes in an input file containing service request and calculates 
  the total seek time for each scheduling algorithm

- Input file format: 
  "total cylinders" "current read/write head position" "previous disk request" [request data]
  (Note that each value needs to be seprated by a single space)

- Once computation is complete the program waits for another input file.

- Users can provide the "QUIT" command to exit the program (case insensitive).

# How to run the Program

Step 1: 
run `make` command
This will build the program and create a folder called "bin" containing all the object files and 
an executable called "scheduler" in the current directory.

Step 2:
Run executable to start the program.
command: `./scheduler`

Step 3 (Optional)
Run `make clean` to delete the bin folder containing the object files and "scheduler" 
executable from the directory.

Developed using C11 Standard
