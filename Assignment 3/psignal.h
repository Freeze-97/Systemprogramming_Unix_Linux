/* 
File:	psignal.h

Objective: 	Declaration of a wrapper for installing a POSIX signal handler
   
Created:	Oct 2004 / orjste
*/

#ifndef PSIGNAL_H
#define PSIGNAL_H

#include <signal.h>
#include <string>

using std::string;

extern "C" {
	typedef  void (*SigHandler)(int);  // Signal handler function type
}

SigHandler psignal(int sigNo, SigHandler) throw (string);


#endif

