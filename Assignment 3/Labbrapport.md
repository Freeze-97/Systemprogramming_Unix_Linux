# Laboration 1
## Environment & Tools / Utvecklingsmiljö & Verktyg
Operating system: Ubuntu 20.04.1 LTS
IDE: CLion
Compiler: GCC

## Purpose / Syfte
The purpose of this lab is to understand signals and timers. How to use them and why they can help us better understand processes and how the CPU works.
Different types of signals will determinate how the program, processes, should respond. In this specific assignment the goals is to use both timers and signals to print out stats regarding 3 different types of timers: real, virtual and prof. 
Real represents the "wall clock", virtual represents how much time the process has actually spent on the CPU, prof does the same but plus the time the kernel has been used.
The test uses 3 different types of "work" to be done, CPU, IO and CPU+IO.

## Procedures / Genomförande
We have to first create 3 different types of input for the user which is CPU, IO and CPU+IO.
Later we needed to put up all types of signals which were gonna be used and decide what should happen based on that. Like this:
```cpp
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
    }
```
Later the timers should start and the tasks will fire. When it's all done the stats will be printed. I used 5 second intervals as it was suggested in the assignment.

## Discussion / Diskussion
Overall this was a very difficult assignment for me. It's an important subject for sure and the positive side is that it was something truly new and different as I haven't worked with timers before in C++. I do believe the book is too much concentraded into details rather than first make sure that the reader truly understands the basics of the subject. The PDF lesson did help a lot though, could possibly be because of the language as well. I also found the assignment information to be too limited, it should explains more what should be used in the program and a possible structure that can be used to write the prorgam. And I can't really run my program so help regarding that would be much appreciated. 

To answer the question in the assignment about the different results: As I could not run the program I saw the results from other similar programs on stackoverflow, which I hope is ok. The CPU result showed high % on both virtual and prof time. The IOtask showed much lower % of the real time. The CPU + IO gave a middle ground basically.
This is because the CPU task is mostly on the CPU therefor giving close to 100% on both virtual and prof. The IO task is not working on the CPU in the same way at all, like the harddisk and other stuff are working there, giving low virtual time but still high prof since it takes the kernels work into account which is something IO tasks are used by mostly, the kernel system. 

