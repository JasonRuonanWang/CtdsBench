ifeq ($(VENDOR),cray)
	MPICXX=cc
else
	MPICXX=mpic++
endif

CXX=g++

CCFLAGS=-std=c++11
LDFLAGS=-lcasa_tables -lcasa_casa
STMANFILES=libadios2stman.so
STMANFILES=

mpi:write.cc read.cc $(STMANFILES)
	$(MPICXX) -g write.cc $(CCFLAGS) $(LDFLAGS) $(STMANFILES) -o write -DHAVE_MPI
	$(MPICXX) -g read.cc $(CCFLAGS) $(LDFLAGS) $(STMANFILES) -o read -DHAVE_MPI

$(TARGET): $(TARGET:=.cc) $(STMANFILES)
	$(CXX) $@.cc -o $@ $(CCFLAGS) $(LDFLAGS) $(STMANFILES) 



$(STMANFILES):  
	cd ..; make ln;

cl:
	rm -rf *.casa *.out *.table *.o* *.e*

clean:cl
	rm -rf write read *.dSYM *.so *.table

re: clean mpi
