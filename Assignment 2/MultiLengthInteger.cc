// MultiLengthInteger.cpp: implementation of the MultiLengthInteger class.
//
//////////////////////////////////////////////////////////////////////

#include <sstream>
#include <iomanip>
#include <algorithm>
#include "MultiLengthInteger.h"

using namespace std;

//////////////////////////////////////////////////////////////////////
// Construction/Destruction
//////////////////////////////////////////////////////////////////////

MultiLengthInteger::MultiLengthInteger(int nElements)
:NElements(nElements)
{
   iArray = new int[NElements];
}

MultiLengthInteger::~MultiLengthInteger()
{
   delete [] iArray;
}

//////////////////////////////////////////////////////////////////////


void MultiLengthInteger::addfunc(MultiLengthInteger const *Adelta)
{ 
  int *Ai;
  const int *Ad;
  int j;

  for (j = 0, Ai = &iArray[NElements-1], Ad = &Adelta->iArray[NElements-1]; j < NElements; j++, Ai--, Ad--)
    {
	*Ai += *Ad;
	if (*Ai >= MaxElementValuePlus1)
	{
	    *Ai -= MaxElementValuePlus1;
	    if (Ai <= &iArray[0])
		error();	// overflow 
	    Ai[-1]++;
	}
    }
}

void MultiLengthInteger::subfunc(MultiLengthInteger const *Adelta)
{ 
  int *Ai;
  const int *Ad;
  int j;

    for (j = 0, Ai = &iArray[NElements-1], Ad = &Adelta->iArray[NElements-1]; j < NElements; j++, Ai--, Ad--)
    {
	*Ai -= *Ad;
	if (*Ai < 0)
	{
	    *Ai += MaxElementValuePlus1;
	    if (Ai <= &iArray[0])
		error();	// overflow
	    Ai[-1]--;
	}
    }
}

void MultiLengthInteger::assign(int const val)
{
  int *Ai;

    for (Ai = &iArray[0]; Ai < &iArray[NElements]; Ai++)
	*Ai = 0;
    iArray[0] = val;
}

void MultiLengthInteger::divfunc(unsigned int const divisor)
{ 
  int *Ai;
  long long d;

    for (Ai = &iArray[0], d = 0; Ai < &iArray[NElements]; Ai++)	/* high to low */
    {
	d += *Ai;
	*Ai = d / divisor;
	d = (d % divisor) * MaxElementValuePlus1;
        if (d < 0)
            error();
    }
}

int MultiLengthInteger::compop(MultiLengthInteger const *Ab) const
{ 
  const int *lAa;
  const int *lAb;

    for (lAa = &iArray[0], lAb = &Ab->iArray[0]; lAa < &iArray[NElements]; lAa++, lAb++)   /* high to low */
	if (*lAa != *lAb)
	    return(*lAa - *lAb);
    return(0);
}

void MultiLengthInteger::error() const
{
    printf("\nError\n");
    exit(4);
}

void MultiLengthInteger::operator =(int const val)
{
    assign(val);
}

void MultiLengthInteger::operator =(const MultiLengthInteger & aMLI) {
    
    this->NElements = aMLI.NElements;
    copy(aMLI.iArray,aMLI.iArray+NElements,this->iArray);
}

void MultiLengthInteger::operator /=(unsigned int const divisor)
{
    divfunc(divisor);
}

bool MultiLengthInteger::operator==(MultiLengthInteger const & b) const
{
    return(compop(&b) == 0);
}

bool MultiLengthInteger::operator>(MultiLengthInteger const & b) const
{
    return(compop(&b) > 0);
}

void MultiLengthInteger::operator-=(MultiLengthInteger const & delta)
{
    subfunc(&delta);
}

void MultiLengthInteger::operator+=(MultiLengthInteger const & delta)
{
    addfunc(&delta);
}


int  MultiLengthInteger::operator[](int idx) const
{
    return iArray[idx];
}

string MultiLengthInteger::toString() const
{
    
    string s;
    ostringstream oss;

    	for (int j = 0; j < NElements; j++) {
          int k = iArray[j];
	  oss.str("");
          if (j == 0)
	     oss << ' ' << k << ". "; 
	    
          else
	     oss << ' ' << setw(NDigits) << setfill('0')  << k;

          s += oss.str();
          if ((j % 10) == 0)
            s+="\n";
        }	    
        return s;
}

