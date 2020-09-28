#include "schedulers.h"

//Round Robin scheduler implementation. In general, this function maintains a double ended queue
//of processes that are candidates for scheduling (the ready variable) and always schedules
//the first process on that list, if available (i.e., if the list has members)
int RoundRobin(const int& curTime, const vector<Process>& procList, const int& timeQuantum)
{
    static int timeToNextSched = timeQuantum;  //keeps track of when we should actually schedule a new process
    static deque<int> ready;  //keeps track of the processes that are ready to be scheduled

    int idx = -1, chk;
    bool done;

    // first look through the process list and find any processes that are newly ready and
    // add them to the back of the ready queue
    for(int i = 0, i_end = procList.size(); i < i_end; ++i)
    {
        if(procList[i].startTime == curTime)
        {
            ready.push_back(i);
        }
    }

    // now take a look the head of the ready queue, and update if needed
    // (i.e., if we are supposed to schedule now or the process is done)
    if(timeToNextSched == 0 || procList[ready[0]].isDone)
    {
        // the process at the start of the ready queue is being taken off of the
        // processor

        // if the process isn't done, add it to the back of the ready queue
        if(!procList[ready[0]].isDone)
        {
            ready.push_back(ready[0]);
        }

        // remove the process from the front of the ready queue and reset the time until
        // the next scheduling
        ready.pop_front();
        timeToNextSched = timeQuantum;
    }

    // if the ready queue has any processes on it
    if(ready.size() > 0)
    {
        // grab the front process and decrement the time to next scheduling
        idx = ready[0];
        --timeToNextSched;
    }
    // if the ready queue has no processes on it
    else
    {
        // send back an invalid process index and set the time to next scheduling
        // value so that we try again next time step
        idx = -1;
        timeToNextSched = 0;
    }

    // return back the index of the process to schedule next
    return idx;
}

int SPN( const int& currentTime, const vector<Process>& procList )
{
    static vector<int> readyList; // sorts ready processes
    int index = -1; // returns the index of the next process
    int shortest; // The shortest process time
    static int index2 = 0; // index of the previously ran process

    // Fills readyList with the arrived process
    for ( int i = 0; i < procList.size(); i++ )
    {
        if ( procList[i].startTime == currentTime )
        {
            readyList.push_back(i);
        }
    }
    
    // Checks the readyList if it is not empty
    if ( readyList.size() > 0 )
    {
        index = readyList[index2];
        // checks if the process is done and erases the previous process and finds the shortest process
        if ( procList[readyList[index2]].isDone )
        {
            readyList.erase(readyList.begin() + index2);
            index2 = 0;
            index = readyList[0];
            shortest = procList[readyList[0]].totalTimeNeeded;

            for ( int i = 1; i < readyList.size(); i++ )
            {
                // compares the current shortest time with the time needed of the remaining processes
                if ( procList[readyList[i]].totalTimeNeeded < shortest )
                {
                    shortest = procList[readyList[i]].totalTimeNeeded;
                    index = readyList[i];
                    index2 = i;
                }
            }
        }
    }

    return index;
}

int SRT( const int& currentTime, const vector<Process>& procList )
{
    static vector<int> readyList; // sorts ready processes
    int index = -1; // returns the index of the next process
    int shortest; // The shortest process time
    int timeRemaining; // the process time remaining
    static int index2 = 0; // index of the previously ran process

    // Fills readyList with the arrived process
    for ( int i = 0; i < procList.size(); i++ )
    {
        if ( procList[i].startTime == currentTime )
        {
            readyList.push_back(i);
        }
    }

    // Checks the readyList if it is not empty
    if ( readyList.size() > 0 )
    {
        // checks if the process is done and erases the previous process and finds the shortest process
        if ( procList[readyList[index2]].isDone == true )
        {
            readyList.erase(readyList.begin() + index2);
            index2 = 0;
        }    

        index = readyList[index2];
        shortest = ( procList[readyList[index2]].totalTimeNeeded - procList[readyList[index2]].timeScheduled );

        // compares the current shortest time with the time remaining of the remaining processes
        for ( int i = 0; i < readyList.size(); i++ )
        {
            timeRemaining = ( procList[readyList[i]].totalTimeNeeded - procList[readyList[i]].timeScheduled );

            if ( timeRemaining < shortest )
            {
                shortest = timeRemaining;
                index = readyList[i];
                index2 = i;
            }
        }
    }

    return index;
}

int HRRN( const int& currentTime, const vector<Process>& procList )
{
    static vector<int> readyList; // sorts ready processes
    int index = -1; // returns the index of the next process
    static int index2 = 0; // index of the previously ran process
    float ratio; // ratio of (waittime + timeNeeded) / timeNeeded
    float highestRatio; // highest ratio of the ready processes
    
    // Fills readyList with the arrived process
    for ( int i = 0; i < procList.size(); i++ )
    {
        if ( procList[i].startTime == currentTime )
        {
            readyList.push_back(i);
        }
    }

    // Checks the readyList if it is not empty
    if ( readyList.size() > 0 )
    {
        index = readyList[index2];
        // checks if the process is done and erases the previous process and finds the highest ratio process
        if ( procList[readyList[index2]].isDone )
        {
            readyList.erase(readyList.begin() + index2);
            index2 = 0;
            index = readyList[0];
            highestRatio = (static_cast<float>((currentTime - procList[index].startTime)) / procList[index].totalTimeNeeded) +1;

            // compares the current highest ratio with the ratio of the remaining processes
            for ( int i = 1; i < readyList.size(); i++ )
            {
                ratio = (static_cast<float>((currentTime - procList[readyList[i]].startTime)) / procList[readyList[i]].totalTimeNeeded) +1;

                if ( ratio > highestRatio )
                {
                    highestRatio = ratio;
                    index = readyList[i];
                    index2 = i;
                }
            }
        }
    }

    return index;
}

ostream& runStats( ostream& out, const vector<Process>& procList, const int& numProcess )
{
    vector<float> turnTime, normTurnTime; // vector of all the turnaround times and normalized turnaround times of the processes
    float avgTurnTime, avgNormTurnTime; 
    float tempAvg = 0; 
    float tempAvgNorm = 0;
    turnTime.reserve(procList.size()); // reserving the size of the vector to the process list
    normTurnTime.reserve(procList.size()); // reserving the size of the vector to the process list

    // calculates the turnaround time and normal turnaround time of the processes
    for ( int i = 0; i < procList.size(); i++ )
    {
        turnTime.push_back( (procList[i].timeFinished+1) - procList[i].startTime );
        normTurnTime.push_back( turnTime.at(i) / procList[i].timeScheduled );
        tempAvg = tempAvg + turnTime.at(i);
        tempAvgNorm = tempAvgNorm + normTurnTime.at(i);
    }

    avgTurnTime = ( tempAvg / numProcess );
    avgNormTurnTime = ( tempAvgNorm / numProcess );

    // formats the stats of the processes
    out << "Process | Finish Time | Turnaround Time | Normalized Turnaround Time |" << endl;

    for ( int i = 0; i < 70; i++ )
        out << '-';

    out << endl << fixed << setprecision(2); // sets the precision of floats to 2 decimals 

    for ( int i = 0; i < procList.size(); i++ )
    {
        out << setw(7) << procList[i].id << " | " << setw(11) << procList[i].timeFinished+1;
        out << " | " << setw(15) << static_cast<int>(turnTime.at(i)) << " | ";
        out << setw(26) << normTurnTime.at(i) << " |\n";
    }

    for ( int i = 0; i < 70; i++ )
        out << '-';

    out << endl << setw(7) << "Mean" << " | " << setw(14) << " | " << setw(15) << avgTurnTime << " | "; 
    out << setw(26) << avgNormTurnTime << " |\n";

    return out;
}