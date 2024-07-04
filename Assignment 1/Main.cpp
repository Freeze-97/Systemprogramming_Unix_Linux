/*
 * Main program running monte carlo sim
 * Input from user on nr of processes and throws
 *
 * Tommy Yasi (toya1800)
 */

#include "MonteCarlo.h"
#include "Restart.h"
#include <iostream>
#include <unistd.h>

int main(int argc, char *argv[]) {
    pid_t child_pid = 0;
    int n = 10; // number of processes
    int throws = 10000;

    if(argc > 3) { // Check if number of arguments is valid, only 1,2 and 3
        fprintf(stderr, "Usage %s processes\n", argv[0]);
        return 1;
    }

    try {
        if (argc >= 2) {
            n = std::stoi(argv[1]);
        }
        if (argc >= 3) {
            throws = std::stoi(argv[2]);
        }
    } catch(std::exception &err) {
        perror("Error! Invalid arguments!");
    }

    // Create the child processes
    for(int i = 0; i < n; i++) {
        child_pid = fork();
        if(child_pid == -1) {
            perror("Failed to fork");
            return 1;
        }
        if(child_pid == 0) { // If child process
            break;
        }
    }

    // Create child processes, every child execute MonteCarlo and saves it to a separate file
    if(child_pid == 0) {
        MonteCarlo mc(throws);
        mc.write_to_file(std::to_string(getpid()));
        return EXIT_SUCCESS;
    } else {
        double average = 0;
        int status;
        for (int i = 0; i < n; i++) {
            while (!WIFSIGNALED(status) && !WIFEXITED(status)) {
                // Wait for child process
                pid_t child_p = r_wait(&status);

                if (WIFEXITED(status)) {
                    std::string filename = std::to_string(child_p);
                    std::ifstream read_file(filename);
                    if (read_file.is_open()) {
                        double pi;
                        read_file >> pi;
                        read_file.close();
                        average += pi;

                        // Output all PI values
                        std::cout << "Process: " << child_p << " terminated normally with return value " <<
                                  WEXITSTATUS(status) << " PI: " << std::fixed
                                  << std::setprecision(4) << pi << std::endl;
                    } else {
                        perror("Unable to open file");
                    }

                    // Delete file
                    if (std::remove(std::string
                    (std::to_string(child_p)+"dat").c_str()) != 0) {
                        perror("Error deleting file!");
                    }
                } else if(WIFSIGNALED(status)) {
                    printf("Child %lid terminated due to uncaught signal %d\n", (long) child_p, WTERMSIG(status));
                } else if(WIFSTOPPED(status)) {
                    printf("Child %lid stopped due to signal %d\n", (long) child_p, WSTOPSIG(status));
                } 
            }
        }
        // Output average
        average = average / n;
        printf("Average: %f", average);
    }
	return 0;
}
