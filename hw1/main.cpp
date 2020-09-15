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
    unsigned long processes = processList.size();
    unsigned long* current_index = new unsigned long [processes];
    unsigned long** event_index = new unsigned long* [processes];
    for ( unsigned long i = 0; i < processes; i++ )
    {
        event_index[i] = new unsigned long [processList.at(i).ioEvents.size()];
        current_index[i] = 0;
        event_index[i][current_index[i]] = 0;
    }         
    while( allProcessesComplete(processList) == false )
    {
        // Set state of all processes that should be ready to ready
        // hint: arrival time and processes becoming unblocked

        for ( unsigned long i = 0; i < processes; i++ )
        {
            if ( processorAvailable == true )
            {
                for ( unsigned long j = 0; j < processes; j++ )
                {
                    if ( processList.at(j).state == ready && processorAvailable == true)
                    {
                        processorAvailable = false;
                        processList.at(j).state = processing;
                    }
                } 
            }    

            if ( processList.at(i).state == notArrived )
            {
                if ( processList.at(i).arrivalTime == time )
                {
                    processList.at(i).state = ready;
                    if ( processList.at(i).ioEvents.size() != 0 )
                        processList.at(i).currentEvent = processList.at(i).ioEvents.front();

                    if ( processList.at(i).reqProcessorTime == 0 )
                    {
                        processList.at(i).state = done;
                        processList.at(i).processorTime = 0;
                        processList.at(i).doneTime = time;
                    }
                }
            }

            if ( processList.at(i).state == blocked )
            {
                if ( static_cast<long>(event_index[i][current_index[i]]) == processList.at(i).currentEvent.duration )
                {
                    processList.at(i).state = ready;

                    if ( current_index[i] < processList.at(i).ioEvents.size() )
                    {
                        current_index[i]++;
                        processList.at(i).currentEvent = processList.at(i).ioEvents.at(current_index[i]);
                    }
                }

                else
                    event_index[i][current_index[i]]++;        
            }

            if ( processList.at(i).state == processing )
            {
                if ( processList.at(i).processorTime == processList.at(i).reqProcessorTime )
                {
                    processList.at(i).state = done;
                    processList.at(i).doneTime = time;
                    processorAvailable = true;
                }

                else if ( processList.at(i).ioEvents.size() != 0 && processList.at(i).currentEvent.time == processList.at(i).processorTime )
                {
                    processList.at(i).state = blocked;
                    processorAvailable = true;
                }

                else 
                    processList.at(i).processorTime++;
            }     
        }
        printProcessSet(processList);
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

    // freeing the memory used in the program 
    for ( unsigned long i = 0; i < processes; i++ )
    {
        delete[] event_index[i];
    }
    delete[] event_index;
	event_index = NULL;
    delete[] current_index;
    current_index = NULL;

    return 0;
}
