#   File:	makefile
#   Builds:	Lab4
#   Date:	October 2019
						
CCX = g++

CCFLAGS		= -ansi -pedantic -std=c++11
LDFLAGS	    = -pthread

PROGS		= Lab4
OBJS		= lab4.o Queue.o SharedMem.o
HDRS		= Queue.h SharedMem.h

.cc.o.h:
	@echo "Compiling " $<
	$(CCX) -c $(CCFLAGS) $<
	@echo $@ "done"

all:	$(PROGS)


Lab4:	$(OBJS)
	@echo "Linking " $@
	$(CCX)  -o $@ $(OBJS) $(LDFLAGS)
	@echo $@ "done"

clean:
	$(RM) $(PROGS) $(OBJS) core *~
