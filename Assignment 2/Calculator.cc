/* File: Calculator.cc 
   Topic: Implementation of callback function pipeWriter
*/

#include <cstdio>
#include "Calculator.h"
#include "Restart.h"

using namespace std;

void pipeWriter(int writeFd, char aDigit) {
  int nWritten;	
  if((nWritten = r_write(writeFd,&aDigit,1))==-1)
    perror("pipeWriter");
   
}
