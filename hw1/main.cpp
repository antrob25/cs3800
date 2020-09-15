#include "process.h"

#include <chrono> // for sleep
#include <thread> // for sleep

int main(int argc, char* argv[])
{
    // single thread processor
    // it's either processing something or it's not
    bool processorAvailable = true;
    // vector of processes, fill by reading from a file
    vector<Process> processList;

    // Do not touch
    long time = 1;
    long sleepDuration = 50;
    string file;
    stringstream ss;

    // Do not touch
    switch(argc)
    {
        case 1:
            file = "procList.txt";  // default input file
            break;
        case 2:
            file = argv[1];         // file given from command line
            break;
        case 3:
            file = argv[1];         // file given
            ss.str(argv[2]);        // sleep duration given
            ss >> sleepDuration;
            break;
        default:
            cerr << "incorrect number of command line arguments" << endl;
            cout << "usage: " << argv[0] << " [file] [sleepDuration]" << endl;
            return 1;
            break;
    }

    // Read in from the file
    initProcessSetFromFile(file, processList);

    while()
    {
        // Set state of all processes that should be ready to ready
        // hint: arrival time and processes becoming unblocked
        

        // Select the next process that should be given to the processor

        // Increment the selected process's processorTime
        // Set state of the process based on the following: 
        // required process time met - done; IOEvent started - blocked; 

        // Make sure to track the time that processes have been blocked
        // you can do update this just about anywhere as long as it gets
        // updated each time iteration

        // Do not touch
        cout << time++ << '\t'; // Do not touch
        // You may wish to use a second vector of processes (you don't need to, but you can)
        printProcessStates(processList); // change processList to another vector of processes if desired
        this_thread::sleep_for(chrono::milliseconds(sleepDuration)); // Do not touch
    }

    return 0;
}
