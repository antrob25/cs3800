// Baseline code created by Seth Kitchen
#include <iostream>
#include <thread>
#include <mutex>
#include <condition_variable>
#include <queue>
#include <string>
#include <fstream>
#include "poemerize.h"
#include "semaphores.h"
//#include <cstdlib>
#include <cerrno>
#include <unistd.h>
#include <random>
#include <vector>

using namespace std;

const string fileBase = "outFile";

binary_semaphore ExampleB; // Note: auto-assigned to 1 in constructor
binary_semaphore fileWrite; //semaphore array for each fork
general_semaphore ExampleG;  // Note: auto assigned to 0 in constructor
enum State { THINKING, WAITING, WRITING }; // declaring enum to hold states for the philosophers
vector<State> philStates; // holds the states for the number of philosophers

// checks the Philosopher's neighbors before writing on a page
void checkPhil( const int& id, const int& left, const int& right, const float& sleepTime )
{
    if ( (philStates[id] == WAITING) && (philStates[left] != WRITING) && (philStates[right]) != WRITING )
    {
        fileWrite.unlock();
        cout << "Phil " << id << " is no longer waiting" << endl;
        philStates[id] = WRITING;
        cout << "Philosopher " << id << " is writing on pages " << id << " and " << right << ".\n";
        sleep(sleepTime);    
    }
}

// Sets the Philosopher to waiting before it receives a page
void takePage( const int& id, const int& left, const int& right, const int& leftP, const float& sleepTime )
{
    philStates[id] = WAITING; 
    cout << "Philosopher " << id << " is WAITING on pages " << left << " and " << right << ".\n";
    checkPhil( id, leftP, right, sleepTime);
    if ( philStates[id] != WRITING )
    {
        fileWrite.lock();
        cout << "Locked Page  " << id << endl;
        sleep(sleepTime);
    }
}

// sets the Philosopher to thinking after it is done with a page
void returnPage( const int& id, const int& left, const int& right, const int& numPhils, const int& leftP, const float& sleepTime )
{
    philStates[id] = THINKING;
    cout << "Philosopher " << id << " is finished using pages " << left << " and " << right << ".\n";
    checkPhil( leftP, ((leftP + (numPhils-1)) % numPhils), id, sleepTime );
    checkPhil( right, id, ((right + 1) % numPhils), sleepTime );
}

void Phil(int id, int totalPhils, int maxMessages, float sleepTime, int seed)
{ 
    int numWritten = 0;

    //setup message storage locations
    int leftNeighbor = id;
    int rightNeighbor = (id + 1) % totalPhils;
    int leftPage = ((id + (totalPhils-1)) % totalPhils); // determines the left neighbor of the philosopher
  
    poemerize P(seed + id);
  
    string lFile = fileBase + to_string(leftNeighbor);
    string rFile = fileBase + to_string(rightNeighbor);
    ofstream foutLeft(lFile.c_str(), ios::out | ios::app );
    ofstream foutRight(rFile.c_str(), ios::out | ios::app );
  
    while (numWritten < maxMessages) 
    {
  	    sleep(sleepTime);
        takePage( id, leftNeighbor, rightNeighbor, leftPage, sleepTime );
        sleep(sleepTime);

        //construct poem & output stanzas into the files 'simultaneously'
        //we do this with an intermediate variable so both files contain the same poem!
        if ( philStates[id] == WRITING ) // checks if the Philosopher is writing
        {
            string stanza1, stanza2, stanza3;
            stanza1 = P.getLine();

            cout << "Philosopher " << id << " is writing the first stanza." << endl;
        
            foutLeft << stanza1 << endl;
            cout << "Philosopher " << id << " wrote the first stanza on page " << leftNeighbor << "." << endl;

            foutRight << stanza1 << endl;
            cout << "Philosopher " << id << " wrote the first stanza on page " << rightNeighbor << "." << endl;

            stanza2 = P.getLine();

            cout << "Philosopher " << id << " is writing the second stanza." << endl;

            foutLeft << stanza2 << endl;
            cout << "Philosopher " << id << " wrote the second stanza on page " << leftNeighbor << "." << endl;

            foutRight << stanza2 << endl;
            cout << "Philosopher " << id << " wrote the second stanza on page " << rightNeighbor << "." << endl;
    
            stanza3 = P.getLine();

            cout << "Philosopher " << id << " is writing the third stanza." << endl;

            foutLeft << stanza3 << endl << endl;
            cout << "Philosopher " << id << " wrote the third stanza on page " << leftNeighbor << "." << endl;

            foutRight << stanza3 << endl << endl;
            cout << "Philosopher " << id << " wrote the third stanza on page " << rightNeighbor << "." << endl;

            returnPage( id, leftNeighbor, rightNeighbor, totalPhils, leftPage, sleepTime );
            numWritten++;
        }
    }
  
    foutLeft.close();
    foutRight.close();
}


int main ( int argc, char *argv[] ) 
{
    int philosophers= 5;   //total Phils (threads)
    int maxMessages = 10;  //poems to write
    float sleepTime = 1;   //amount of time to sleep threads
    int randomSeed = 2;    //default seed for RNG

    for ( int i = 0; i < 5; i++ )
    {
        philStates.push_back(THINKING);
    }

    switch (argc)
    {
        case 5:
            randomSeed = atoi(argv[4]);
            //fall thru
        case 4:
            sleepTime = atof(argv[3]);
            //fall thru
        case 3:
            maxMessages = atoi(argv[2]);
            //fall thru
        case 2:
            philosophers = atoi(argv[1]);
            for ( int i = 5; i < philosophers; i++ )
            {
                philStates.push_back(THINKING);
            }
        case 1:
            break;
        default:
            cerr << "Incorrect number of command line arguments" << endl;
            break;
    }
  
    // safety check - need at least 2 philosophers to make sense
    if (philosophers < 2) {
        cerr << "Need at least 2 philosophers! Try again" << endl;
        return 1; //non-normal exit
    }
  
    thread threadArray[philosophers]; // need to keep track of the threads to .join() later
    cout << "Running " << philosophers << " threads in parallel: \r\n";
  
    // spawn Phil threads
    for (int id = 0; id < philosophers; id++)
    {
        threadArray[id] = thread(Phil, id, philosophers, maxMessages, sleepTime, randomSeed);
    }
  
    // Merge all threads to the main thread before exiting
    for (int id = 0; id < philosophers; id++)
    {
        threadArray[id].join();
    }
  
    cout << "Lets read some poems and support worldwide literacy!\n";
    cout << endl;
  
    return 0;
}
