void doAll(TString directory="macros/junews/lostleptons/extrapolation_binc/")
{
  // gROOT->ProcessLine(".L "+directory+"FillInputHists.C++");
  // TChain* mc = new TChain("TreeMaker2/PreSelection");
  // mc->Add("reduced_trees/13TeV/ttbar_arne/*.root");
  // FillInputHists* mc_tree = new FillInputHists(mc);
  // mc_tree->Run(directory);

  // delete mc_tree;

  gROOT->ProcessLine(".L "+directory+"DTTMHTRatioAnalysis.C++");
  TChain* data = new TChain("TreeMaker2/PreSelection");
  data->Add("reduced_trees/13TeV/ttbar_arne/*.root");
  DTTMHTRatioAnalysis* data_tree = new DTTMHTRatioAnalysis(data);
  data_tree->Run(directory);

   delete data_tree;
}
