CCX 		= g++
CCINCLUDE	=
CCFLAGS		= -std=c++11
LDFLAGS		=  
LIBS		= 
PROGS		= lab2
OBJS		= lab2.o Calculator.o eCalculator.o PiCalculator.o Restart.o \
		  MultiLengthInteger.o
HDRS		=


.cc.o:
	@echo "Compiling " $<
	$(CCX) -c $(CCFLAGS) $(CCINCLUDE) $<
	@echo $@ "done"

all:	$(PROGS)

lab2: $(OBJS)
	$(CCX) -o $@ $(OBJS)  $(LDFLAGS) $(LIBS) 

clean:
	rm -f a.out core $(PROGS) $(OBJS) 
	




