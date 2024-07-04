#   File:	makefile
#   Builds:	Lab5
#   Date: January 2022
CCX = g++

CCFLAGS		=  -ansi -pedantic -std=c++11
#LDFLAGS		= -lrt
LDFLAGS	=

PROGS		= lab5
OBJS		= lab5.o Restart.o TCPsocket.o
HDRS		= Restart.h TCPsocket.h

.cc.o:
	@echo "Compiling " $<
	$(CCX) -c $(CCFLAGS) $<
	@echo $@ "done"

all:	$(PROGS)


Lab3:	$(OBJS)
	@echo "Linking " $@
	$(CCX)  -o $@ $(OBJS) $(LDFLAGS)
	@echo $@ "done"

clean:
	$(RM) $(PROGS) $(OBJS) core *~
