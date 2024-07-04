/*
File:	psignal.cc

Objective:	implementation of psignal, wrapper för POSIX reliable signal.
psignal installs func as a handler for signal sigNo. The old handler is returned.

Created:	Oct 2004 / orjste
*/

#include <errno.h>
#include <signal.h>
#include <cstring>
#include"psignal.h"

SigHandler psignal(int sigNo, SigHandler func) throw(string)
{
	struct sigaction	act, oact;

	act.sa_handler = func;
	sigemptyset(&act.sa_mask);
	act.sa_flags = 0;

	if (sigaction(sigNo, &act, &oact) == -1)
		throw(string("Exception in psignal: " + string(strerror(errno))));
	return(oact.sa_handler);
}


