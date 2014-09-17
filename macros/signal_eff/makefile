CXX = $(shell root-config --cxx)
CXXFLAGS = -isystem$(shell root-config --incdir) -Wall -Wextra -pedantic -Wshadow $(shell root-config --cflags) -O2
LD = $(shell root-config --ld)
LDFLAGS = $(shell root-config --ldflags)
LDLIBS = $(shell root-config --libs) -lMinuit

default: signal_eff.exe

signal_eff.exe: signal_eff.o
	$(LD) $(LDFLAGS) -o signal_eff.exe signal_eff.o $(LDLIBS)

signal_eff.o: signal_eff.cpp ../include/my_style.hpp
	$(CXX) $(CXXFLAGS) -c signal_eff.cpp


.PHONY: clean

clean:
	-rm -rf *.exe *.o *~ *#
