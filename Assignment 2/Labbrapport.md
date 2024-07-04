# Laboration 2
## Environment & Tools / Utvecklingsmiljö & Verktyg
Operating system: Ubuntu 20.04.1 LTS
IDE: CLion
Compiler: GCC

## Purpose / Syfte
The goal was to explore the idea behind UNIX I/O, read and write, select/poll and files. This assignment focused mainly on the select/poll part to handle file descriptors (via pipes) where the parent process was the reading part while the child processes were counting pi and e to write to the pipe. The parent process then output stats for every 1000 number that has ben counted for. The parent process will write the numbers to two different files (pi, e) at the same time as the child processes will write the numbers to their pipes in different time. Many other classes were included to help us focus on the main task which is communication between the processes instead of the mathematical part of pi and e. 

## Procedures / Genomförande
What's important to understand is first which processes you need and how they chould communicate to get the desired result without anyone overlapping the other. The parent process should only read from the pipes while the children 
are the ones producing the numbers and writing them to their respective pipe. 
To make sure that the parent can read this while there is still numbers to be read using either select or poll is needed to make sure that the parent process will get all numbers before quitting. 
The only differnce is that while select focuses on the condition while poll depends on the information of the file descriptors. 
Problems that occurred were mostly down to poll and select. I did not find it easy to understand what the book was trying to tell me at the beginning and googling about poll and select gave me some but limited answers. So to make it easier I did decide to more or less copy the poll part from the book but adapt it to my program, while doing this my understanding for the poll structure became clearer and every part made more sense in the end. The main issue with my program is that im not sure if it actually works since i'm getting an error regarding a close function which is outside of my own code. I hope to get helps with this when you check my code.

## Discussion / Diskussion
In general the purpose has ben fulfilled as the key parts were implemented into the progam and were used correctly. This could possibly be done in a much different way but the point was to understand how the UNIX I/O works and what the power of having multiple processes with different tasks can do for us in terms of structure and performance. 
This assignment could of course look a little bit different had select been used instead of poll, but it wouldn't really change the goal of this program or how it really acts. 
Personally I found this assignment very difficult and complex, atleast at the start but understanding poll was the key part which was missing at first. The book explains all key parts but it can be a bit complex and straight forward. It would have been nice with more help via the lessons. 
