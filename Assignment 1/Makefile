output: Main.o MonteCarlo.o Restart.o
	g++ Main.o MonteCarlo.o Restart.o -o output

Main.o: Main.cpp
	g++ -c Main.cpp

MonteCarlo.o: MonteCarlo.cpp MonteCarlo.h
	g++ -c MonteCarlo.cpp

Restart.o: Restart.cc Restart.h
	g++ -c Restart.cc

clean:
	rm *.o output
