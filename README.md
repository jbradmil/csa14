csa14
=====

Code for making plots for CSA14 from cfA ntuples.  Originally based on ald77/Hbb.git and SusyRa2b/NtupleTools.git

Note: the makefile is by default written using OSX sed syntax.  To
make it lunux-compatible, change
	sed -i '' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@
to
	sed -i'' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@
