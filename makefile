EXEDIR := scripts
OBJDIR := bin
SRCDIR := src
INCDIR := inc
MAKEDIR := bin

CXX := $(shell root-config --cxx)
EXTRA_WARNINGS := -Wcast-align -Wcast-qual -Wdisabled-optimization -Wformat=2 -Wformat-nonliteral -Wformat-security -Wformat-y2k -Winit-self -Winvalid-pch -Wlong-long -Wmissing-format-attribute -Wmissing-include-dirs -Wmissing-noreturn -Wpacked -Wpointer-arith -Wredundant-decls -Wstack-protector -Wswitch-default -Wswitch-enum -Wundef -Wunused -Wvariadic-macros -Wwrite-strings -Wabi -Wctor-dtor-privacy -Wnon-virtual-dtor -Wsign-promo -Wsign-compare #-Wunsafe-loop-optimizations -Wfloat-equal -Wsign-conversion -Wunreachable-code 
CXXFLAGS := $(subst -I,-isystem,$(shell fastjet/bin/fastjet-config --cxxflags)) -isystem $(shell root-config --incdir) -Wall -Wextra -pedantic -Werror -Wshadow -Woverloaded-virtual -Wold-style-cast $(EXTRA_WARNINGS) $(shell root-config --cflags) -O2 -I $(INCDIR) -g -pg
LD := $(shell root-config --ld)
LDFLAGS := $(shell fastjet/bin/fastjet-config --libs --plugins) $(shell root-config --ldflags) -g -pg
LDLIBS := $(shell root-config --libs) -lMinuit

vpath %.cpp $(SRCDIR)
vpath %.hpp $(INCDIR)
vpath %.o $(OBJDIR)
vpath %.so $(OBJDIR)
vpath %.exe $(EXEDIR)
vpath %.d $(MAKEDIR)

# Add new executables to this list
all: make_reduced_tree.exe print_collections.exe skim_file.exe skim_reduced_tree.exe slim_cfa_files.exe

# List any object files your executable needs to be linked with
$(EXEDIR)/generate_cfa_class.exe: generate_cfa_class.o
$(EXEDIR)/make_reduced_tree.exe: make_reduced_tree.o event_handler.o event_number.o b_jet.o fat_jet.o math.o pu_constants.o timer.o cfa2014.o weights.o reduced_tree_maker.o gen_muon.o gen_electron.o gen_tau.o in_json_2012.o BTagWeight2.o
$(EXEDIR)/print_collections.exe: print_collections.o object_printer.o cfa2014.o event_handler.o event_number.o b_jet.o fat_jet.o math.o pu_constants.o timer.o cfa2014.o weights.o reduced_tree_maker.o gen_muon.o gen_electron.o gen_tau.o in_json_2012.o BTagWeight2.o
$(EXEDIR)/skim_file.exe: skim_file.o event_handler.o event_number.o b_jet.o fat_jet.o math.o pu_constants.o timer.o cfa2014.o cfa_skimmer.o gen_muon.o gen_electron.o gen_tau.o in_json_2012.o BTagWeight2.o
$(EXEDIR)/skim_reduced_tree.exe: skim_reduced_tree.o
$(EXEDIR)/slim_cfa_files.exe: slim_cfa_files.o

-include $(addsuffix .d,$(addprefix $(MAKEDIR)/,$(notdir $(basename $(wildcard $(SRCDIR)/*.cpp)))))
-include $(MAKEDIR)/cfa2014.d

# Note: you need a space after sed -i to compile in Mac OSX.  You must NOT have a space in linux.
$(MAKEDIR)/%.d: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -MM -MG -MF $@ $< 
	sed -i'' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@

$(OBJDIR)/%.o: $(SRCDIR)/%.cpp
	$(CXX) $(CXXFLAGS) -o $@ -c $<

# This is a bit ugly. Shouldn't need the dependency explicitly.
$(EXEDIR)/%.exe: $(OBJDIR)/%.o
	$(LD) $(LDFLAGS) -o $@ $^ $(LDLIBS)

# cfa.cpp and cfa.hpp need special treatment. Probably cleaner ways to do this.
$(SRCDIR)/cfa.cpp $(INCDIR)/cfa.hpp: dummy_cfa.all
.SECONDARY: dummy_cfa.all
dummy_cfa.all: $(EXEDIR)/generate_cfa_class.exe example_root_file.root
	./$< $(word 2,$^)
.PRECIOUS: generate_cfa_class.o

.DELETE_ON_ERROR:

.PHONY: clean

clean:
	-rm -rf $(EXEDIR)/*.exe $(OBJDIR)/*.o $(MAKEDIR)/*.d $(SRCDIR)/cfa.cpp $(INCDIR)/cfa.hpp *.exe *.o *.d
	./scripts/remove_backups.sh
