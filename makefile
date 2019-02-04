ifeq ($(VENDOR),cray)
	MPICXX=cc
else
	MPICXX=mpic++
endif
ifeq ($(HAS_HDF5STMAN),)
	HAS_HDF5STMAN=yes
endif
ifeq ($(HAS_ADIOSSTMAN),)
	HAS_ADIOSSTMAN=yes
endif

CXX=g++

CCFLAGS+=-std=c++11
LDFLAGS+=-lcasa_tables -lcasa_casa

ifeq ($(HAS_HDF5STMAN),yes)
	CCFLAGS+=-DHAS_HDF5STMAN
	LDFLAGS+=-lhdf5stman
endif
ifeq ($(HAS_ADIOSSTMAN),yes)
	CCFLAGS+=-DHAS_ADIOSSTMAN
	LDFLAGS+=-ladiosstman
endif

mpi:write.cc read.cc
	$(MPICXX) -g write.cc $(CCFLAGS) $(LDFLAGS) -o write -DHAVE_MPI
	$(MPICXX) -g write_concat.cc $(CCFLAGS) $(LDFLAGS) -o write_concat -DHAVE_MPI
	$(MPICXX) -g write_example.cc $(CCFLAGS) $(LDFLAGS) -o write_example -DHAVE_MPI
	$(MPICXX) -g read.cc $(CCFLAGS) $(LDFLAGS) -o read -DHAVE_MPI

$(TARGET): $(TARGET:=.cc)
	$(CXX) $@.cc -o $@ $(CCFLAGS) $(LDFLAGS)


cl:
	rm -rf *.casa *.out *.table *.o* *.e*

clean:cl
	rm -rf write write_concat read *.dSYM *.so *.table

re: clean mpi
