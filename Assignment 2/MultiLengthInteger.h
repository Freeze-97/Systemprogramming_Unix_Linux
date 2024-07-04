// MultiLengthInteger.h: used by class piCalculator
//
//////////////////////////////////////////////////////////////////////

#ifndef MULTILLENGTHINT_H
#define MULTILLENGTHINT_H

#include <cstdio>
#include <cstdlib>
#include <string>

const int MaxElementValuePlus1	= 1000000;
const int NDigits = 6; // Present result in groups of six digits

using namespace std;

class MultiLengthInteger  
{
public:
	MultiLengthInteger(int nElements);
	virtual ~MultiLengthInteger();

public:
        void operator =(int const val);
	void operator =(const MultiLengthInteger & aMLI);
        void operator +=(MultiLengthInteger const & delta);
        void operator -=(MultiLengthInteger const & delta);
        void operator /=(unsigned int const divisor);
        bool operator >(MultiLengthInteger const & b) const;
        bool operator ==(MultiLengthInteger const & b) const;
	int  operator[](int) const;
        string toString() const;
	int nElements() const { return NElements; }

private:
	void assign(int const val);
	void addfunc(MultiLengthInteger const *Adelta);
	void subfunc(MultiLengthInteger const *Adelta);
	int compop(MultiLengthInteger const *Ab) const;
	void divfunc(unsigned divisor);

	void error(void) const;

	int NElements;
	int* iArray; // [0] is the most significant element  
};

#endif 
