CXX = $(shell root-config --cxx)
CXXFLAGS = -isystem$(shell root-config --incdir) -Wall -Wextra -pedantic -Wshadow $(shell root-config --cflags) -O2
LD = $(shell root-config --ld)
LDFLAGS = $(shell root-config --ldflags)
LDLIBS = $(shell root-config --libs) -lMinuit

default: draw_from_trees.exe

draw_from_trees.exe: draw_from_trees.o
	$(LD) $(LDFLAGS) -o draw_from_trees.exe draw_from_trees.o $(LDLIBS)

draw_from_trees.o: draw_from_trees.cpp ../include/my_style.hpp
	$(CXX) $(CXXFLAGS) -c draw_from_trees.cpp


.PHONY: clean

clean:
	-rm -rf *.exe *.o *~ *#
