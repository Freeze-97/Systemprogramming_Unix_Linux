/* File: PiCalculator.cc 
   Topic: Implementation of class PiCalculator.
*/

#include "PiCalculator.h"

#include <sstream>
#include <fstream>
#include <sys/time.h>
#include <errno.h>
#include <iostream>
#include <iomanip>

using namespace std;


PiCalculator::PiCalculator(int nDecimalPlaces,int aFD, CallbackFunc aCBF)
:nDecPlaces(nDecimalPlaces),NELEMENTS(1+(nDecimalPlaces+5)/6),answer(NELEMENTS),
 zero(NELEMENTS),term5(NELEMENTS),term239(NELEMENTS),term5m(NELEMENTS),
 term239m(NELEMENTS),pipeFd(aFD),iCBF(aCBF)
{
    term5 = 16;     
    term5 /= 5;     
    n5 = 0;

    term239 = 4;    
    term239 /= 239; 
    n239 = 0;

    answer = 0;     
    zero = 0;    
}


void PiCalculator::doCalc()
{
    calculate();
   
}



void PiCalculator::calculate(void)
{
   
    int quitCount, reportCount=0, pos=0;
    do5term();
    do239term();

    char c;
    string s;
    int count = 0;
    

    for(;;) {
    	quitCount = 0;
	if(!(term5m==zero)) 
	   do5term();
	else
	   ++quitCount;
	     
	if(!(term239m==zero))
	   do239term();
	else
	   ++quitCount;    
	   
	if((++reportCount%5)==0) {
	   stringstream ss;
	   if(reportCount==5) // First time, no leading zeros before '3'
	     ss << answer[pos++];
	   else  
	     ss << setw(6) << setfill('0') << answer[pos++];
	 
	   while(ss >> c) {
	      iCBF(pipeFd,c);
	      ++count;
	   }
   	}
	    
	if(quitCount==2)
	   break;
    }
    
    for(int i=pos;i<=nDecPlaces/6+1;++i) {  // Get the rest
       
        stringstream ss;
	ss << setw(6) << setfill('0') << answer[pos++];
	 
	while((ss >> c) && count <= nDecPlaces) {
	   iCBF(pipeFd,c);      
	   ++count;
	}
	if(count>nDecPlaces)
	  break;
    } 
    
    if(close(pipeFd)==-1)
      perror("PiCalculator closing pipeFd");
}

void PiCalculator::do5term()
{
    term5m = term5;
    term5m /= n5 * 2 + 1; 
    if (n5 % 2 == 0)
	answer += term5m;  /* n5 is even */
    else	
        answer -= term5m;  /* n5 is odd */
    term5 /= 5 * 5;     
    n5++;
    
}

void PiCalculator::do239term()
{
    term239m = term239;
    term239m /= n239 * 2 + 1;   
    if (n239 % 2 == 0)
	answer -= term239m;  /* n239 is even */
    else	
        answer += term239m;  /* n239 is odd */
    term239 /= 239 * 239;     
    n239++;
}

