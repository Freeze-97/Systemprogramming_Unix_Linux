/*  File:       Point.cpp
    Purpose:    Implementation of class Point
    Date:       Sept 2007 / �rjan Sterner 
*/

#include "Point.h"


Point::Point() {
    xc=yc=0.0;
}

Point::Point(float x,float y) {
    xc=x;
    yc=y;
}


Point Point::setXY(float newX, float newY) {
    Point oldP(xc,yc); // save old values
    xc=newX;   // set new values
    yc=newY;
    return oldP;
}

Point Point::setXY(const Point &p) {
    Point oldP(getX(), getY()); // save old values
    xc = p.getX(); // set new values
    yc = p.getY();
    return oldP;
}


Point Point::move(const Point &p) {
   Point oldP(getX(), getY()); // save old values
   xc += p.getX(); // add new values
   yc += p.getY();
   return oldP;
}

void Point::printOn(ostream &os) const {
    os << '(' << xc << ',' << yc << ')';
}

