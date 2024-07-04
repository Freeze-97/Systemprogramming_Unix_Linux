/* File: eCalculator.h 
   Topic: Definition of class eCalculator which uses a MacLaurin serie to
   expand decimal digits of e.
*/

#ifndef ECALCULATOR_H
#define ECALCULATOR_H

#include <cmath>
#include <iostream>
#include "Calculator.h"

using namespace std;

class eCalculator : public Calculator {

public:
  eCalculator(int aDecimalPlaces, int aFD, CallbackFunc=pipeWriter);
  ~eCalculator();
  
  virtual void doCalc();
  
private:
  int iDecimalPlaces;
  int iTerms; // # terms needed for given # decimal places
  int *coeffArr;  // Coeff array
  int pipeFd;  // pipe fd to write a digit 
  CallbackFunc iCBF; // Called for each digit
  
  int calcNrOfTermsNeeded(int aDecimalPlaces);

};

#endif

