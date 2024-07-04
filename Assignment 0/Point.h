/*  File:       Point.h
    Purpose:    Definition of Point
    Date:       2012 / Örjan Sterner
*/


#ifndef POINT_H
#define POINT_H

#include <iostream>
using namespace std; 
 
class Point {


public: // Public interface

    // Constructors
    Point();    // default constructor
    Point(float,float); // overloaded constructor

    // Destructor
    ~Point() {}   // does nothing

    // Getters & setters
    void setX(float newX) { xc = newX; } // set x-coord
    void setY(float newY) { yc = newY; }    // set y-coord
    Point setXY(float,float); // set x- and y-coord, returns old Point
    Point setXY(const Point &); // set x and from argument
                                     // returns old Point
    Point move(const Point &p); // move this point as indicated by p
                                     // returns old Point
    float getX() const { return xc; } // get x-coord
    float getY() const { return yc; } // get y-coord

    // Facilitators
    void printOn(ostream &os=cout) const;   // print coordinates on os

private:    // Private members
    float xc,yc;

};


#endif

