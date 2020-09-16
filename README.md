# 3 State Single Threaded Processor
## Programming Assignment Number 1
### Description
In this assignment you will implement a basic single threaded 3 state processor with the following process states: ready, blocked, and processing.
I have added 2 additional states for your (the programmer's) convenience: notArrived and done.

You place a process on the "processor" by marking the processor as unavailable - processorAvailable = false;
Then that process's process time is incremented

When a process is placed on the processor it should continue to process until it is done or blocked by an IOEvent

When a process is complete or blocked the first ready process in the vector by index should be placed on the processor
(This is not accurate to how a processor would work, but it simplifies things for the purpose of this assignment)

##### Processor Flow
1. Set processes to ready (when they arrive and become unblocked)
2. Check for the *first* process that is ready
3. "Process" (processorAvailable = false) for 1 time unit (while loop iteration)
4. repeat 3 until done or blocked (but make sure that time isn't standing still for other processes)
5. repeat steps 1-4 until all processes are complete

### Compiling
type "make" and press enter
just *make* sure your program can compile on the campus machines

### Running
./program

##### command line arguments
./program [file] [sleepduration]
**file** - pass a file name from the command line
**sleepDuration** - how long the program sleeps each loop iteration

### Files
`process.h`
`process.cpp`
`main.cpp`

process defines the process struct and provides functions that will be helpful to you

The only file you should *need* to change is main.cpp.
Absolutely do **not** change the output format in any way shape or form.

The program should exit once all the processes are complete

### Output
The output function is provided to you, and it is already written in `main.cpp` where it should be executed, again, do *not* change either of these. I will be using diff to attempt to auto-grade where possible, and I use a diff tool when I manually check output so don't mess with it.

The print function outputs the state of every process in the proces vector, it is executed each time interval, so you should see each process continuously process until blocked or complete, then another process starts, until they are all complete.

### Input
The input file follows this format:
arrivalTime requiredProcessTime (IOEventStart IOEventLength)*

example:
```
1 7 3 2
5 6
10 13 4 1 8 1
```

3 processes:
The First - an IO event starts after it has processed for 3 time units and lasts 2 time units
The Second - no IO events 
The Third - 2 IO events starting at 4 tu and 8 tu and each last for 1 tu

I recommend you create your own test files.

### Code Quality
Your code does not need to be perfect, it does not need to be extremely space efficient or fast. Your code does need to be readable, commented where necessary, formatted consistently, and able to compile on at least 1 campus machine.

### Grading
Compiles - 10pts
Code Quality - 10pts
Correct Process Order - 15pts
Processes "Process" - 15pts
Processes Block Properly - 15pts
All Processes Complete - 15pts
Test Inputs - 20pts 
