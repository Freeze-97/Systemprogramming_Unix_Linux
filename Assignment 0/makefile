output: Test.o Triangle.o Point.o
 g++ Test.o Triangle.o Point.o -o output
 
all: Test.cpp Triangle.cpp Point.cpp
 g++ -c Test.cpp Triangle.cpp Point.cpp
 
Triangle: Triangle.cpp
 g++ -c Triangle.cpp
 
Point: Point.cpp
  g++ -c Point.cpp
  
clean: 
 rm *.o output