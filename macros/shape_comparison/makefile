CXX = $(shell root-config --cxx)
CXXFLAGS = -isystem$(shell root-config --incdir) -Wall -Wextra -pedantic -Wshadow $(shell root-config --cflags) -O2
LD = $(shell root-config --ld)
LDFLAGS = $(shell root-config --ldflags)
LDLIBS = $(shell root-config --libs) -lMinuit

default: shape_comparison.exe

shape_comparison.exe: shape_comparison.o
	$(LD) $(LDFLAGS) -o shape_comparison.exe shape_comparison.o $(LDLIBS)

shape_comparison.o: shape_comparison.cpp ../include/my_style.hpp
	$(CXX) $(CXXFLAGS) -c shape_comparison.cpp


.PHONY: clean

clean:
	-rm -rf *.exe *.o *~ *#
