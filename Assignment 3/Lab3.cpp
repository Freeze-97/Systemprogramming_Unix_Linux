// Laboration 3
// Tommy Yasi (toya1800)

#include <iostream>
#include <ctime>
#include <lastlog.h>
#include <csignal>
#include <cstdio>
#include <cstdlib>
#include <iomanip>
#include "TestTask.h"
#include "psignal.h"

#define DEFAULT_SECONDS 5
#define MILLION 1000000

using namespace::std;

void signalHandler(int signalNum); // This will handle all the signals
void printTimers();
void closeTimers();
int addTime();
void startTimers();
time_t convertTime(time_t time, int type); // Type refers to real, virtual and prof

// Global variables
sig_atomic_t stopFlag = 0;
time_t startTime, realTime, virtualTime, profTime;
itimerval realTimeValue, virtualTimeValue, profTimeValue;

int main(int argc, char *argv[]) {
    // Needs to be 2+ arguments
    if(argc < 2) {
        perror("Error! Arguments is needed.");
    }

    // Depending on the arguments, different tasks will be done
    int choice = 0;
    if((string)argv[1] == "io"){
        choice = 1;
    } else if((string)argv[1] == "cpu") {
        choice = 2;
    } else if((string)argv[1] == "cpu-io") { // cpu + io
        choice = 3;
    } else {
        perror("Error! inputs are not valid. "
               "'io', 'cpu' and 'cpu-io' are allowed");
        return EXIT_FAILURE;
    }

    // Set timers to 0
    realTime = 0;
    virtualTime = 0;
    profTime = 0;

    TestTask *testTask;
    switch (choice) {
        case 1: // io
            testTask = new IOtask(stopFlag, "Daffodils.txt");
            break;
        case 2: //cpu
            testTask = new CPUtask(stopFlag);
            break;
        case 3: // cpu + io
            testTask = new CPUandIOtask(stopFlag, "Daffodils.txt");
            break;
    }

    // Signals + handlers
    psignal(SIGUSR1, signalHandler); // User defined 1
    psignal(SIGUSR2, signalHandler); // User defined 2
    psignal(SIGALRM, signalHandler); // Real timer
    psignal(SIGVTALRM, signalHandler); // Virtual timer
    psignal(SIGPROF, signalHandler); // Prof timer
    psignal(SIGINT, signalHandler); // Interrupt
    psignal(SIGTERM, signalHandler); // Kill

    // Start the timers
    startTimers();

    // Run all tasks
    testTask->run();

    // Show timers
    printTimers();

    return 0;
}

void signalHandler(int signalNum) {
    switch (signalNum) {
        case SIGUSR1:
            printTimers();
            break;
        case SIGUSR2: // These 2 are when the program stops
        case SIGTERM:
            stopFlag = 1;
            closeTimers();
            break;
        case SIGALRM:
            realTime += addTime();
            break;
        case SIGVTALRM:
            virtualTime += addTime();
            break;
        case SIGPROF:
            profTime += addTime();
            break;
        default:
            cout << "Other signal" << endl;
            break;
    }
}

void printTimers() {
    // Get time for all the timers in seconds using readTime function
    auto realTimeSeconds = double(convertTime(realTime, ITIMER_REAL)) / MILLION;
    auto virtualTimeSeconds = double(convertTime(virtualTime, ITIMER_VIRTUAL)) / MILLION;
    auto profTimeSeconds = double(convertTime(profTime, ITIMER_PROF)) / MILLION;

    // Get time of today (ms)
    struct timeval tp{};
    gettimeofday(&tp, nullptr);
    auto ms = time_t(tp.tv_usec+(tp.tv_sec*MILLION));

    // Time it has taken, comparing to start time of course
    time_t timeElapsed = ms - startTime;

    // Get the precision of the timers (%)
    double percentageRealTime = realTimeSeconds / (double(timeElapsed) / MILLION) ;
    double percentageVirtualTime = virtualTimeSeconds / (double(timeElapsed) / MILLION);
    double percentageProfTime = profTimeSeconds / (double(timeElapsed) / MILLION);

    // Print the numbers
    cout << setprecision(2) << fixed;
    cout << "Real time: " << realTimeSeconds << " sec, " << percentageRealTime * 100 << "%" << endl;
    cout << "Virtual time: " << virtualTimeSeconds << " sec, " << percentageVirtualTime * 100 << "%" << endl;
    cout << "Prof time: " << profTimeSeconds << " sec, " << percentageProfTime * 100 << "%" << endl;
}

// Closes all x timeval, tips from others
void closeTimers() {
    realTimeValue.it_interval.tv_sec = -1;
    realTimeValue.it_interval.tv_usec = -1;
    realTimeValue.it_value = realTimeValue.it_interval;

    virtualTimeValue.it_interval.tv_sec = -1;
    virtualTimeValue.it_interval.tv_usec = -1;
    virtualTimeValue.it_value = realTimeValue.it_interval;

    profTimeValue.it_interval.tv_sec = -1;
    profTimeValue.it_interval.tv_usec = -1;
    profTimeValue.it_value = realTimeValue.it_interval;
}

int addTime() {
    return DEFAULT_SECONDS * MILLION;
}

void startTimers() {
    struct itimerval defaultValues{};
    defaultValues.it_interval.tv_sec = DEFAULT_SECONDS;
    defaultValues.it_interval.tv_usec = 0;
    defaultValues.it_value = defaultValues.it_interval;

    // Set all timers to default
    realTimeValue = defaultValues;
    virtualTimeValue = defaultValues;
    profTimeValue = defaultValues;

    // Get time of today (ms)
    struct timeval tp{};
    gettimeofday(&tp, nullptr);
    startTime = time_t(tp.tv_usec+(tp.tv_sec*MILLION));

    if(setitimer(ITIMER_REAL, &realTimeValue, nullptr) == -1) {
        perror("Error! Failed to start real timer!");
    }

    if(setitimer(ITIMER_VIRTUAL, &virtualTimeValue, nullptr) == -1) {
        perror("Error! Failed to start virtual timer!");
    }


    if(setitimer(ITIMER_PROF, &profTimeValue, nullptr) == -1) {
        perror("Error! Failed to start prof timer!");
    }
}

time_t convertTime(time_t time, int type) {
    struct itimerval val{};
    time_t finalTime = 0;
    if(getitimer(type, &val) == 0) {
        time_t num = (val.it_value.tv_sec * MILLION) + val.it_value.tv_usec;
        finalTime = time + time_t ((DEFAULT_SECONDS * MILLION) - num);
    } else {
        perror("Error! Could not convertTime");
    }

    return finalTime;
}
