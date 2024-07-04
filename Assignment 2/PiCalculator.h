/* File: PiCalculator.h 
   Topic: Definition of class PiCalculator which uses a MacLaurin serie to
   expand decimal digits of pi.
*/

#ifndef PICALCULATOR_H
#define PICALCULATOR_H

#include <unistd.h>
#include "Calculator.h"
#include "MultiLengthInteger.h"

class PiCalculator : public Calculator {



public:
	PiCalculator(int nDecimalPlaces,int aFD,CallbackFunc=pipeWriter);
	
	virtual void doCalc();

private:
	void do239term(void);
	void do5term(void);
        void calculate();

	int NELEMENTS;
	MultiLengthInteger answer,zero,term5,term239,term5m,term239m;
	int n5,n239,nDecPlaces;
	int pipeFd;  // pipe fd to write a digit 
  	CallbackFunc iCBF; // Called for each digit
};

#endif 
