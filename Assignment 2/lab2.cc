/*
 *
 * Laboration 2: I/O Multiplexing
 * Student: Tommy Yasi / toya1800
 *
 */

#include <iostream>
#include <iomanip>
#include <unistd.h>
#include <fcntl.h>
#include <cstdlib>
#include <cstdio>
#include <sys/poll.h>
#include <sys/wait.h>
#include <vector>
#include <algorithm>
#include "PiCalculator.h"
#include "eCalculator.h"
#include "Restart.h"
#define BUFFSIZE 1024

void print_num(const vector<int>& nums, bool is_pi);

int main(int argc, char *argv[]) {
    int n; // Standard if no input can be read
    int status; // used to check on child processes

    // Check if arguments from the commandline are correct
    if(argc == 1) {
        n = 50000;
    } else if(argc == 2) {
        n = stoi(argv[1]);
    } else {
        fprintf(stderr, "Usage %s processes\n", argv[0]);
        return 1;
    }

    /* Create 2 pipes
     *
     * One between Parent - pi_child process
     * Another between Parent - e_child process
     *
     * ...fd[0] - read
     * ...fd[1] - write
     */
    int pi_fd[2];
    int e_fd[2];

    // Control pipes
    if(pipe(pi_fd) == -1) {
        perror("Failed to create pipe (pi)");
        return -1;
    }
    if(pipe(e_fd) == -1) {
        perror("Failed to create pipe (pi)");
        return -1;
    }

    // Open/create files
    int fd_pi, fd_e;

    if((fd_pi = open("pi.dat", O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        perror("Failed to open pi.dat");
        return 5;
    }
    if((fd_e = open("e.dat", O_WRONLY | O_APPEND | O_CREAT | O_TRUNC, S_IRWXU)) == -1) {
        perror("Failed to open e.dat");
        return 6;
    }

    /* Decided to use poll
     * Most of it is copied from the book
     * int poll(struct pollfd *fds, nfds_t nfds, int timeout);
     * fds = array of struct pollfd
     * nfds = number of descriptors to be monitored
     * timeout = wait for event before returning
     */
    struct pollfd *pollfd;
    if((pollfd = (struct pollfd*) calloc(2, sizeof(struct pollfd))) == nullptr) {
        perror("Could not poll");
        return 2;
    }
    // Initialize pollfd variables and event flags
    (pollfd + 0)->fd = pi_fd[0];
    (pollfd + 0)->events = (POLLIN | POLLRDNORM);
    (pollfd + 1)->fd = e_fd[0];
    (pollfd + 1)->events = (POLLIN | POLLRDNORM);

    // Create 2 child_process, one for pi and one for e
    pid_t child_pid;
    int pi_child_process_id;
    for(int i = 0; i < 2; i++) {
        if ((child_pid = fork()) <= 0){
            if(child_pid == -1) {
                perror("Failed to fork");
                return 1;
            }
            if(child_pid == 0 && i == 0) { // First child process is for pi, other one is for e
                pi_child_process_id = getpid();
            }
            break;
        }
    }

    if(child_pid == 0) { // Child processes execute this part
        if(getpid() == pi_child_process_id) { /* pi child process */
            // Close, will not be used
            r_close(pi_fd[0]); // Read pi
            r_close(e_fd[0]); // Read e
            r_close (e_fd[1]); // Write e

            // Calculate
            PiCalculator pi_calc(n, pi_fd[1]);
            pi_calc.doCalc();

            r_close(pi_fd[1]); // Close write, won't be used by this process anymore
            return EXIT_SUCCESS;
        } else { /* e child process */
            // Close, will not be used
            r_close(e_fd[0]); // Read e
            r_close(pi_fd[0]); // Read pi
            r_close(pi_fd[1]); // Write e

            // Calculate
            eCalculator e_calc(n, e_fd[1]);
            e_calc.doCalc();

            r_close(e_fd[1]); // Close write
            return EXIT_SUCCESS;
        }
    } else { /* Parent process */
        // Close write on both pipes, will not be used by parent
        r_close(pi_fd[1]);
        r_close(e_fd[1]);
        char buf[BUFFSIZE];
        long bytes_read;
        int i;
        int num_now = 2;
        int num_ready;

        // Count numbers of pi and e
        int pi_read = 0;
        int e_read = 0;

        /*
         * This will be used to see how many integers there
         * are out of every number 0-9 for pi and e
         */
        vector<int> count_pi;
        vector<int> count_e;

        while(num_now > 0) { // Continue monitoring until descriptors done
            num_ready = poll(pollfd, 2, -1);
            if((num_ready == -1) && (errno == EINTR)) {
                continue; // poll got interrupted by signal, try again
            } else if(num_ready == -1) {
                break; // real poll error, can't continue
            }

            for(i = 0; i < 2 && num_ready > 0; i++) {
                if((pollfd + i)->revents) {
                    if((pollfd + i)->revents & (POLLIN | POLLRDNORM)) {
                        if(i == 0) {
                            bytes_read = r_read(pi_fd[0], buf, BUFFSIZE);
                        } else {
                            bytes_read = r_read(e_fd[0], buf, BUFFSIZE);
                        }
                        num_ready--;

                        if(bytes_read > 0) {
                            for(int a = 0; a < bytes_read; a++) {
                                // Reading pi
                                if((pollfd + i)->fd == pi_fd[0]) {
                                    count_pi.push_back(buf[a] - '0'); // Add the num to the vector
                                    pi_read++;
                                    if(pi_read >= 1000) { // For every 1000 num, print
                                        print_num(count_pi, true);
                                        pi_read = pi_read % 1000; // Divide 1000 to repeat this process
                                    }
                                } else if ((pollfd + i)->fd == e_fd[0]) { // Reading e
                                    count_e.push_back(buf[a] - '0'); // Add the num to the vector
                                    e_read++;
                                    if(e_read >= 1000) { // For every 1000 num, print
                                        print_num(count_pi, false);
                                        e_read = e_read % 1000; // Divide 1000 to repeat this process
                                    }
                                }
                            }
                            //r_write file and check if it's pi or e
                            if((pollfd + i)->fd == fd_pi) {
                                r_write(fd_pi, buf, bytes_read); // pi
                            } else if((pollfd + i)->fd == fd_e) {
                                r_write(fd_e, buf, bytes_read); // e
                            }
                        } else {
                            bytes_read = -1; // end of file
                        }
                    } else if((pollfd + i)->revents & (POLLERR | POLLHUP)) {
                        bytes_read = -1;
                    } else { // descriptor not involved in this round
                        bytes_read = 0;
                    }
                    if(bytes_read == -1) { // error occurred, remove descriptor
                        if(i == 0) {
                            r_close(pi_fd[0]);
                        } else {
                            r_close(e_fd[0]);
                        }
                        (pollfd + i)->fd = -1;
                        num_now--;
                    }
                }
            }
        }
        //
        for(i = 0; i < 2; i++) {
            if(i == 0) {
                r_close(pi_fd[0]);
            } else {
                r_close(e_fd[0]);
            }
        }
        free(pollfd);

        // Wait and check for the child processes
        for(i = 0; i < 2; i++) {
            while (!WIFSIGNALED(status) && !WIFEXITED(status)) {
                // Wait for child process
                pid_t child_p = waitpid(-1, &status, 0);

                if (child_pid == -1) {
                    perror("Error, could not wait for child process");
                } else if (WIFEXITED(status)) {
                    // Output all PI values
                    std::cout << "Process: " << child_p << " terminated normally with return value " <<
                              status << " PI: " << std::endl;
                } else if (WIFSIGNALED(status)) {
                    printf("Child %lid terminated due to uncaught signal %d\n", (long) child_p, WTERMSIG(status));
                } else if (WIFSTOPPED(status)) {
                    printf("Child %lid stopped due to signal %d\n", (long) child_p, WSTOPSIG(status));
                }
            }
        }

    }
    return 0;
}

void print_num(const vector<int>& nums, bool is_pi) {
    string type; // If it's pi or e
    if(is_pi) {
        type = "pi numbers ";
    } else {
        type = "e numbers ";
    }
    // Print type
    std::cout << type << nums.size() << endl;

    for(int i = 0; i < 10; i++) {
        long occurrence = std::count(nums.begin(), nums.end(), i); // How many times does this int occur
        double percentage = ((double) occurrence / (double)nums.size())*100;
        std::cout << i << ": " << occurrence << " / " <<
        std::setprecision(2) << fixed << percentage << "%" << endl;
    }
    std::cout << endl; // just extra space
}
