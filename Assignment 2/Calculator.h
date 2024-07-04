/* File: Calculator.h 
   Topic: Definition of base class Calculator which gives a common inteface to
   derived calculator classes.
*/

#ifndef CALCULATOR_H
#define CALCULATOR_H

/* Callback for the calculators
   A Calculator is configured with a callback function which is called for each digit produced.
   The default callback is the pipeWriter function which writes aDigit to writeFd, the write end of the pipe.
*/

void pipeWriter(int writeFd, char aDigit); 


class Calculator {

public:
  virtual ~Calculator() {}	
  virtual void doCalc()=0;
  
protected:
  typedef void (*CallbackFunc)(int aFd, char aDigit);
  
};

#endif
