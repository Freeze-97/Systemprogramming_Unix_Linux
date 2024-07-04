
/*	File:	Triangle.h
	Topic:	Definition of class Triangle
             using Point
	Date: 	2012 / Örjan Sterner 
*/


#ifndef TRIANGLE_H
#define TRIANGLE_H

#include "Point.h"
#include <iostream>

using namespace std;

class Triangle {
private:
    Point p1,p2,p3;
public:
    /* Constructors */
    Triangle(Point a, Point b, Point c);

   /* Destructor */
    ~Triangle() {}  // inline definition

   /* Getters & setters */
    void move(Point);
    void move(float,float);

    Point tp() const; 

   /* Facilitators */
    void printOn(ostream &os=cout) const;
};


#endif

