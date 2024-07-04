# Laboration 1

## Environment & Tools / Utvecklingsmiljö & Verktyg 
Operating system: Ubuntu 20.04.1 LTS
IDE: CLion
Compiler: GCC

## Purpose / SyftePerspective: 
The goal of this lab was to explore processes in UNIX and how to both handle them and create them in our C++ program. The main points, which were going to be used, were fork and wait to create child processes and make sure that they collaborate in the correct way with the main process. 
The other part was to create a Monte Carlo simulation which will give us a close result to PI but it was already specfied that it could be done in a more effective way but it was never the main purpose of the lab, the important part was to understand processes and how they can help us build better performing programs and also how we can catch errors safely when working with multiple processes.

## Procedures / GenomförandePerspective: 
What's important is to first understand how the fork function works. Creating child processes can be different depending on how you want to create them. The first problem was that I wanted the main process to be the only parent process which produces the child prcoesses, instead I got a chain system were every child prcoess produced children, except for the last one of cours. 
This problem was solved by only letting the main process create the children, like this:
```cpp
    for(i = 0; i < n; i++) {
        if ((child_pid = fork()) <= 0){
            if(child_pid == -1) {
                perror("Failed to fork");
                return 1;
            }
            break;
        }
    }
```
n = Amount of childprocesses.

Another important part is to keep track of potential errors via "stderr", "perror" etc to catch and handle them. 

And another important part is to always let the child process terminate before their parents so you don't get zombies in ur program. Use wait for every childprcoess, like in this case it was used in a for-loop:
```cpp
  pid_t child_p = waitpid(-1, &state, 0);
```
And always keep checking the status of all the processes to see if they return the expected values or potential errors.

## Discussion / DiskussionPerspective: 
Generally the purpose has been achieved which was learning about processes in UNIX, how we can create them and use them to our advantage but also be aware of the error risks that comes with multiple processes in our programs. Personally I did find this lab very complicated in the beginning, I didn't fully understand where to begin and how to create childprocesses only from the main process. Another "issue" is how I should use the error messages, how you actually build a prorgam which takes arguments via the console should have been explained in an early stage as well. 
I know that I do have all parts in my prgoram however I'm still getting this message when running my code, I don't understand if it's a code problem or more of my envoirment, I hope I can get help with that part. The error message: 
```cpp
  Process finished with exit code 139(interrupted by signal 11:SIGSEGV)
```
And help regarding the makefile as well.

