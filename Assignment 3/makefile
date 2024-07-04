#   File:	makefile
#   Builds:	Lab3
#   Date: December 2021
CCX = g++

CCFLAGS		=  -ansi -pedantic -std=c++11
#LDFLAGS		= -lrt
LDFLAGS	=

PROGS		= Lab3
OBJS		= Lab3.o TestTask.o Restart.o Random.o psignal.o
HDRS		= psignal.h Random.h TestTask.h

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
