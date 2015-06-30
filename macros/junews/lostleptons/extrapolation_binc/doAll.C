void doAll(TString directory="macros/junews/lostleptons/extrapolation_binc/")
{
  gROOT->ProcessLine(".L "+directory+"FillInputHists.C++");
  TChain* mc = new TChain("TreeMaker2/PreSelection");
  mc->Add("TreeMaker/mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/*.root");
  FillInputHists* mc_tree = new FillInputHists(mc);
  mc_tree->Run(directory+"outputfiles/cdtt_input_no_dp_cut.root");

  delete mc_tree;

  gROOT->ProcessLine(".L "+directory+"DTTMHTRatioAnalysis.C++");
  TChain* data = new TChain("TreeMaker2/PreSelection");
  data->Add("TreeMaker/mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/*.root");
  DTTMHTRatioAnalysis* data_tree = new DTTMHTRatioAnalysis(data);
  data_tree->Run(directory+"outputfiles/cdtt_input_no_dp_cut.root", directory+"outputfiles/analysis_output_no_dp_cut.root");

  delete data_tree;
}
