/*	File:	main.cpp
	Topic:	Test of class Triangle
	Date: 	2012 / Örjan Sterner
*/

#include "Triangle.h"
#include "Point.h"

int main() {

    Triangle t1( Point(10,-10), Point(0,20), Point(50,50));

    cout << "triangle: ";
    t1.printOn();  cout << endl;

    Point tp = t1.tp();
    cout << "TP=";
    tp.printOn();
    cout << endl;

    t1.move(-5,-10);
    
    cout << "Modified triangle: ";
    t1.printOn();   cout << endl;
    tp  = t1.tp();
    cout << "TP=";
    tp.printOn();
    cout << endl;


    return 0;
}

