void doAll()
{
  gROOT->ProcessLine(".L FillInputHists.C++");
  TChain* mc = new TChain("TreeMaker2/PreSelection");
  mc->Add("/cms8r0/johnbr/2014/csa14/TreeMaker/mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/*.root");
  FillInputHists* mc_tree = new FillInputHists(mc);
  mc_tree->Run();

  delete mc_tree;

  gROOT->ProcessLine(".L DTTMHTRatioAnalysis.C++");
  TChain* data = new TChain("TreeMaker2/PreSelection");
  data->Add("/cms8r0/johnbr/2014/csa14/TreeMaker/mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/*.root");
  DTTMHTRatioAnalysis* data_tree = new DTTMHTRatioAnalysis(data);
  data_tree->Run();

  delete data_tree;
}
