/*
 * Program illustrating the producer/consumer problem.
 *
 * Parent forks a child process which then starts producing items(integers) and stores them in a buffer (queue).
 * Parent keeps reading from the buffer and printing the integers to the screen.
 * If the option -wdel is passed from the commandline delays of random durations are placed in both producer
 * and consumer, in which case the symmetry of reading/writing is switched up.
 *
 * Created by Gustaf on 10/5/2019.
*/

#include <iostream>
#include <sstream>
#include <semaphore.h>
#include <new>
#include <sys/types.h>
#include <sys/wait.h>
#include <unistd.h>
#include <random>
#include <cstring>

#include "Queue.h"
#include "SharedMem.h"

#define MAX_DELAY_TIME 10000000   //10 ms
#define DEFAULT_INTEGERS 5000

typedef struct {
    Queue *shared_queue;      // buffer
    sem_t *space_available;   // indicate empty space in buffer
    sem_t *item_available;    // indicate numbers available in buffer
    sem_t *queue_lock;        // mutex for locking the queue so only one process can manipulate it.
                              // (the buffer semaphores can allow both processes access at the same time.
                              // queue_lock is also used to protect std::cout)
} PointerCollection;

static bool random_delays = false;
static int num_integers = DEFAULT_INTEGERS;

/*
 * Creates random integers and stores them in the shared Queue.
 *
 * @param pc collection of pointers to the items in shared memory
 */
void produce(const PointerCollection &pc);

/*
 * Dequeues numbers from the shared queue and prints them on the screen together with queue length.
 *
 * @param pc collection of pointers to the items in shared memory
 */
void consume(const PointerCollection &pc);

/*
 * Generates a random int in the specified interval.
 *
 * @param min lower limit
 * @param max upper limit
 */
int get_random_int(const int min, const int max);

/*
 * Delays execution for a random number of nanoseconds (between 0 and MAX_DELAY_TIME)
 */
void waste_random_time();

/*
 * Parses commandline arguments searching for a valid number of integers to produce and an option to add random delays
 * while producing/consuming. If no arguments are passed DEFAULT_INTEGERS will be created and no delay will be added.
 *
 * @param a argc
 * @param v argv
 */
bool parse_commandline(int a, char *v[]);

int main(int argc, char *argv[]) {

    if (!parse_commandline(argc, argv))
        return 1;

    // create shared memory object and attach to it
    SharedMem shared_mem;
    shared_mem.allocate(IPC_PRIVATE, (sizeof(Queue) + sizeof(sem_t) * 3));
    if (shared_mem.attach() == -1) {
        std::cerr << "Failed to attach to shared memory area\n - Exiting -" << std::endl;
        return 1;
    }

    PointerCollection pointers;

    //create objects in shared memory to be accessed by both processes
    try {
        pointers.shared_queue = new (shared_mem.getAddr()) Queue;
        pointers.space_available = new (shared_mem.getAddr() + sizeof(Queue)) sem_t;
        pointers.item_available = new (shared_mem.getAddr() + sizeof(Queue) + sizeof(sem_t)) sem_t;
        pointers.queue_lock = new (shared_mem.getAddr() + sizeof(Queue) + sizeof(sem_t) * 2) sem_t;
    }
    catch (std::bad_alloc &e) {
        std::cerr << "Failed to create object in shared memory: " << e.what() << std::endl;
        std::cerr << "- Exiting -" << std::endl;
        return 1;
    }

    //initialize semaphores
    if (sem_init(pointers.space_available, 1, Q_CAPACITY) || sem_init(pointers.item_available, 1, 0) ||
            sem_init(pointers.queue_lock, 1, 1)) {
        perror("Failed to initialize semaphore");
        std::cerr << "- Exiting -" << std::endl;
        return 1;
    }

    //create child process
    pid_t child_pid;
    if ((child_pid = fork()) == -1) {
        perror("Failed to fork child process");
        std::cerr << "- Exiting -" << std::endl;
        return 1;
    }

    if (child_pid == 0) {
        //child code
        produce(pointers);
        shared_mem.detach();
    }
    else {
        //parent code
        consume(pointers);

        //wait for child
        int status;
        while (int res = wait(&status) < -1 && (errno == EINTR)) {
            std::cout << "waiting" << std::endl;
        }
        //analyse child process' exit status
        if (WIFEXITED(status) && WEXITSTATUS(status)) {
            std::cerr << "Process " << child_pid << " terminated with return value " << WEXITSTATUS(status) << std::endl;
        }
        else if (WIFSTOPPED(status)) {
            std::cerr << "Process " << child_pid << " terminated due to an uncaught signal: "
                      << WTERMSIG(status) << std::endl;
        }
        else if (WIFSTOPPED(status)) {
            std::cerr << "Process " << child_pid << " stopped to signal: " << WSTOPSIG(status) << std::endl;
        }
        else
            std::cout << " -- DONE --" << std::endl;

        // wrap things up (destroy semaphores, detach from and remove shared memory)
        if (sem_destroy(pointers.space_available) || sem_destroy(pointers.item_available) || sem_destroy(pointers.queue_lock)) {
            perror("Failed to destroy semaphore");
        }

        shared_mem.detach();

        if (shared_mem.remove() < 0)
            std::cout << "Failed to deallocate shared memory" << std::endl;
    }
}

void produce(const PointerCollection &pc) {
    bool produced = false;
    int produce_count = 0;
    while (produce_count < num_integers) {

        if (random_delays)
            waste_random_time();

        while (sem_wait(pc.space_available) == -1 && errno == EINTR);

        int random_int = get_random_int(0, 1000);

        while (sem_wait(pc.queue_lock) == -1 && errno == EINTR);

        if ((produced = pc.shared_queue->enqueue(random_int))) {
            std::cout << "Produced: " << random_int;
            std::cout << "\tItems in buffer: " << pc.shared_queue->length() << std::endl;

            ++produce_count;
        }

        //make sure to unlock queue even if no number was enqueued
        if (sem_post(pc.queue_lock)) {
            perror("Failed to unlock queue");
        }

        if (produced && sem_post(pc.item_available) == -1) {
            perror("Failed to indicate item available");
        }
    }
}

void consume(const PointerCollection &pc) {
    bool consumed = false;
    int num;
    int consume_count = 0;
    while (consume_count < num_integers) {

        if (random_delays)
            waste_random_time();

        while (sem_wait(pc.item_available) == -1 && errno == EINTR);

        while (sem_wait(pc.queue_lock) == -1 && errno == EINTR);

        if ((consumed = pc.shared_queue->dequeue(num))) {
            std::cout << "\tConsumed: " << num;
            std::cout << "\tItems in buffer: " << pc.shared_queue->length() << std::endl;

            ++consume_count;
        }

        //make sure to unlock queue even if no number was dequeued
        if (sem_post(pc.queue_lock)) {
            perror("Failed to unlock queue");
        }

        if (consumed && sem_post(pc.space_available)) {
            perror("Failed to indicate space available");
        }
    }
}

int get_random_int(const int min, const int max) {
    static std::random_device rd;
    static std::mt19937 generator(rd());
    std::uniform_int_distribution<int> dist(min, max);
    return dist(generator);
}

void waste_random_time() {
    timespec req;
    req.tv_sec = 0;
    req.tv_nsec = static_cast<long>(get_random_int(0, MAX_DELAY_TIME));
    nanosleep(&req, NULL); //sleep random number of nanoseconds (or until interrupted)
}

bool parse_commandline(int ac, char *av[]) {
    std::ostringstream usage;
    usage << "Usage: " << av[0] << " [-wdel] [#integers]" << std::endl;
    if (ac == 3) {
        std::istringstream argv_stream(av[2]);
        if (!(argv_stream >> num_integers) || strcmp(av[1], "-wdel") != 0) {
            std::cerr << usage.str();
            return false;
        }

        random_delays = true;
    }
    else if (ac == 2) {
        std::istringstream argv_stream(av[1]);
        if (!(argv_stream >> num_integers)) {
            if (argv_stream.str() == "-wdel") {
                random_delays = true;
                num_integers = DEFAULT_INTEGERS;
            }
            else {
                std::cerr << usage.str();
                return false;
            }
        }
    }
    else if (ac != 1) {
        std::cerr << "Bad number of arguments" << std::endl << usage.str();
        return false;
    }

    return true;
}
