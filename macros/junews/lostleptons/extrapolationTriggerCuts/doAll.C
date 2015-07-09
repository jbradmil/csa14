void doAll()
{
  gROOT->ProcessLine(".L FillInputHists.C++");
  TChain* mc = new TChain("PreSelection");
  mc->Add("/cms8r0/johnbr/2014/csa14/TreeMaker/mc/skimmed/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimmed.root");
  FillInputHists* mc_tree = new FillInputHists(mc);
  mc_tree->Run();

  delete mc_tree;

  gROOT->ProcessLine(".L DTTMHTRatioAnalysis.C++");
  TChain* data = new TChain("PreSelection");
  data->Add("/cms8r0/johnbr/2014/csa14/TreeMaker/mc/skimmed/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_skimmed.root");
  DTTMHTRatioAnalysis* data_tree = new DTTMHTRatioAnalysis(data);
  data_tree->Run();

  delete data_tree;
}
