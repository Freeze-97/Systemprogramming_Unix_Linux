# Laboration 4 - Producer / Consumer
### Envoirment & Tools / Utvecklingsmiljö & Verktyg
Operating system: Ubuntu 20.04.1 LTS
IDE: CLion
Compiler: GCC

### Purpose / Syfte
The purpose of this laboration was to learn about shared memory with processes, how to use a queue where
we have one process being the producer and the other being a consumer. The queue has a limited size of course and we use semaphores to make sure the processes don't produce or consume when the queue is full or empty. It is important that the buffer is atomic so only one process at a time can work on it

### Procedures / Genomförande
The first thing I did was focusing on creating the queue, which was circular. I searched for answers on the web to get it done quicker as the queue part wasn't really the main focus of the assigment, I hope that's alright. Later I created the objects which was used on the shared memory and also the semaphores, one checks if it's possible to put an int into the buffer while the other checks if it's possible to consume one.
```cpp
 // The shared memory
// Second parameter is the memory size
auto *shared_memory =
new SharedMem(IPC_PRIVATE,(2 * sizeof(sem_t))
+ sizeof(Queue<int, BUFFSIZE>));

// Attach memory
if(shared_memory->attach() == -1) {
perror("ERROR: Failed to attach memory allocation");
return EXIT_FAILURE;
}

// Create queue
auto *queue = new(shared_memory->getAddr()) Queue<int, BUFFSIZE>;

/*
 * Semaphores
 * emptySpot: checks for empty spot to put in an int value
 * availableSpot: checks for spot available for consuming
*/
auto *emptySpot = new(shared_memory->getAddr() +
sizeof(Queue<int, BUFFSIZE>)) sem_t;
sem_init(emptySpot, true, BUFFSIZE);

auto *availableSpot = new(shared_memory->getAddr() +
sizeof(Queue<int, BUFFSIZE>) +
sizeof(sem_t)) sem_t;
sem_init(availableSpot, true, 0);
```
Then I created the producer and consumer processes via fork(). Allowing them to work on their seperate missions which they were created for. The main process waits for both of them to finish.

### Discussion / Diskussion
This was a little bit tricky, the queue and the idea of a producer and consumer is not new to me. I have seen it many times but I had never done it with multiple processes and using semaphores like this. It made it more complicated but also much "smoother" which is always wanted. I do think the explanation from the PDF was helpful and so was the lab PDF as well with the great tips. I also don't understand why I needed isFull or getSize for my queue as it was possible to create it without these functions. 
And I can't actually test my program since I get an error regarding this:
```cpp
// Create queue
auto *queue = new(shared_memory->getAddr()) Queue<int, BUFFSIZE>;
```
Which is the way the instruction tips did it with the code of: 
```cpp
Queue *theQ =
        new(mySharedMemObject.getAddress) Queue;
```
But other than that I think I included all the needed parts for this lab.
