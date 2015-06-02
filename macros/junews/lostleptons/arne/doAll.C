void doAll(TString directory="macros/junews/lostleptons/ht400met70l15/")
{
  gROOT->ProcessLine(".L macros/junews/lostleptons/RatioError.C++");
  gROOT->ProcessLine(".L "+directory+"RTree.C++");
  TChain* mc = new TChain("reduced_tree");
  mc->Add("reduced_trees/13TeV/skimmed/test_mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78_skimmed.root");
  RTree* mc_tree = new RTree(mc);
  mc_tree->MakeLFRatioHist(directory);			  
  mc_tree->MakeDTTPDFs(directory);
  TChain* data = new TChain("reduced_tree");
  data->Add("reduced_trees/13TeV/skimmed/test_dataset/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78_skimmed.root");
  RTree* data_tree = new RTree(data);
  data_tree->MakeMHTPDFs(directory);
}
