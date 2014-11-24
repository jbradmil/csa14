csa14
=====

Code for making plots for CSA14 from cfA ntuples.  Originally based on ald77/Hbb.git and manuelfs/ucsb_analysis.git

Note: the makefile is by default written using linux sed syntax.  To
make it OSX-compatible, change
	sed -i'' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@
to
	sed -i '' 's#$*.o#$(OBJDIR)/$*.o $(MAKEDIR)/$*.d#g' $@

Quick start--make small reduced ntuples:

-- checkout and compile code

git clone git@github.com:jbradmil/csa14.git

cd csa14

./compile.sh

-- make ntuple from (small) cfA sample

./scripts/make_reduced_tree.exe -c -i SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76

-- output file is reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76.root

# make ntuple from (larger) cfA sample

./scripts/make_lists_and_trees.sh TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2208_v76

# splits sample into 9 batch jobs, 9 output files--output in reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2208_v76/*.root
