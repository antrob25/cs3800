# The Writing Philosophers
## Programming Assignment Number 3
### Description
An extremely large group of philosophers have decided to take up poetry. 
The philosophers do not all meet on the same days, so the number that would write poetry on any given day is variable.
The philosophers discussed the creation of poems and determined the most efficient way to write poems was with 2 hands.
However, there are only so many sheets of paper available, and naturally 2 philosophers cannot write on the same piece of paper at the same time.

So, the philosophers have written a program to simulate the process of writing poems, given the number of philosophers available, but it can only run sequentially.

Your job is to edit `baseline.cpp` so that the code runs concurrently (multiple threads simultaneously).
Philosophers should be able to write to 2 files at once, and the maximum number of philosophers that can write at the same time should be variable (read from the command line).
All of the threads (philosophers) should be executing simultaneously without corrupting the output by accessing the same file at the same time.

The simulated philosophers will choose lines randomly from the vectors in `poemerize.h` (you may change the strings in this file to modify the poems if you would like, it will not affect the output when I run the program).

Each thread is referred to as Phil (five executing concurrently would be Phil 0, Phil 1, … P3, P4). The purpose of each Phil is to its associated 2 files.
Phil 0 will write to File 0 and File 1. Phil 1 will write to File 1 and File 2. … Phil N (the last Phil) will write to File N and File 0.
Clearly there is a possibility Phil 0 and Phil 1 could attempt to write to File 1 at the same time (this is the critical resource whose safe access we need to ensure).
 
You are provided a Semaphore class which implements binary (and general) semaphores with the .wait() and .signal() functionality already implemented.
It is your job to adjust the usage of these calls to allow for interleaving of activity while also safely protecting resource access.
Correct code should allow for any valid interleaving.

Hint: you can probably achieve thread safe code without the general semaphores (if you use the binary ones properly) 

Tips for writing:  
While (messages < 100000)
Ensure your thread has access to both files.
Only secure the critical section when you need it.
Recall how the Dining Philosophers can deadlock…
Needs to handle ANY number of Philosophers (greater than 1).
Need to consider neighbor’s requests
Handle mutual exclusion with the use of semaphores

The difficulty in this project comes from the inherent difficulties of multi-threaded programs.
Debugging will be difficult, compiler errors may not make sense, and when multiple threads share the same resource... I'm sure you'll find out.

### Compiling
You **must** use the provided make file to compile
To use it navigate to the directory containing your code and make file in the terminal
Type `make` and press enter
Upon successful compilation the compile command will be printed and no errors will be displayed
If your code does not compile with the `make` command and/or does not compile on at least 1 campus machine I will give you a 0.
Programs that fail to compile will receive a 0 with no chance for a regrade.

### Running
After compiling the program while in the directory where the binary is compiled execute:
`./program`

When running on a campus machine this program **must** be run on a machine of the form "rc##ucs213.managed.mst.edu" (the numbers run from 01 - 08). AKA one of the physical machines, this will allow the program to run multithreaded.

If you run on a virtual machine / a machine that only supports single threading your program may not work and you won't know (and I won't take pity on you if it breaks when run multi-threaded).

##### command line arguments
./program [numPhilosophers] [maxMessages] [sleepTime] [randomSeed]
**numPhilosophers** (int) - number of concurrent threads
**maxMessages** (int) - number of poems the philosophers write per file
**sleepTime** (float) - how long threads sleep for each iteration
**randomSeed** (int) - seed for random number generation

### Files
`poemerize.h` - poemerize class, contains poem lines
`semaphores.h` - binary and general semaphore classes
`baseline.cpp` - equivalent of main.cpp (this is the only file you can edit)
`README.md` - the file you are reading right now
`exampleOutput.txt` - example output file
\*Note you can edit `poemerize.h` if you would like to switch up the poems a bit, this will not affect how it executes when I run the programs.

### Input
There is no input other than the command line arguments.
I will use different numbers for the command line arguments so I recommend you test with various numbers.

### Output
Output **must** be in the form of files name with the following format: outFile# 
\*pro tip, you can delete these with `make clean`

While files are being written the program should output which philosopher is writing and to which files. This output may get garbled, but that's okay.

Files will contain poems such as the one in exampleOutput.txt.

**INCORRECT OUTPUT**  
Roses are red, violets are blue, The center of my world  
rry, so  
a love so true.  
I need to be with you.

Roses are red, violets are blue,  
What I feel in my heart  
that super glue.  
begins and ends with you.

### Code Quality
Your code does not need to be perfect, it does not need to be extremely space efficient or fast.
Your code **does** need to be readable, commented where necessary, formatted consistently, and able to compile on at least 1 campus machine.

### Grading
20%	Appropriate code commenting and good programming practices  
50%	Correct and safe implementation of only 1 Phil writing to a file at a time (distributed mutual exclusion)  
30% Able to handle any number of Phils (2 or more) 

#### Do Not Touch
I reserve the right to grade you as low as a zero if you break any of the following:  
The command line inputs (you can use them, but do not change how they are taken in)  
Changing the naming scheme for the output files  
Modifying the way threads are created  
Editing any files other than `baseline.cpp` (again `poemerize.h` strings are an exception)
Subverting the program (example you cannot just hard code file output streams to write the files)    
Anything else that could reasonably be considered cheating (don't worry you won't violate this unintentionally)
