/* File: eCalculator.cc 
   Topic: Implementation of class eCalculator.
*/

#include <unistd.h>
#include <cmath>
#include <iostream>
#include <cstdio>
#include <errno.h>
#include "eCalculator.h"

using namespace std;

eCalculator::eCalculator(int aDecimalPlaces,int aFD, CallbackFunc aCBF)
:iDecimalPlaces(aDecimalPlaces),pipeFd(aFD),iCBF(aCBF)
{
   iTerms = calcNrOfTermsNeeded(iDecimalPlaces);
   coeffArr = new int[iTerms+1];
   for(int i=2; i<=iTerms; ++i)
      coeffArr[i] = 1;
}

eCalculator::~eCalculator() {

   delete [] coeffArr;
}


void eCalculator::doCalc() {
   iCBF(pipeFd,'2');  // Integer part of e
   int carry, temp;
   for(int i=1; i<=iDecimalPlaces;++i) {
        carry=0;
        for(int j=iTerms;j>=2;--j) {
           temp = coeffArr[j]*10+carry;
           carry = temp/j;
           coeffArr[j] = temp-carry*j;
        }
        iCBF(pipeFd,char(carry+'0'));
   }
   if(close(pipeFd)==-1)
     perror("eCalculator closing pipeFd");
}

int eCalculator::calcNrOfTermsNeeded(int aDecimalPlaces) {
/*
  Estimates the #terms needed to give (decimalPlaces+1) correct digits.
  Uses Stirling's formula to approximate m!
  Stirling's forula for approximation of n! where n is 'large':
   n! ~ sqrt(2*pi*n) * (n/e)^n

  terms is increased from the initial value until
  m! > 10^(decimalPlaces+1)
*/

  int terms = 4;    // terms needed
  double tmp, test = (aDecimalPlaces+1)*log(10.0);
  do {
        ++terms;
        tmp = terms*(log(double(terms))-1.0) + 0.5*log(2.0*M_PI*terms);
  } while(tmp < test);
  return terms+1;
}

