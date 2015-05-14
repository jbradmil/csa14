void doAll()
{
  gROOT->ProcessLine(".L macros/junews/lostleptons/RTree.C++");
  TChain* mc = new TChain("reduced_tree");
  mc->Add("reduced_trees/13TeV/skimmed/special/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78_skimmed.root");
  RTree* mc_tree = new RTree(mc);
  //mc_tree->MakeLFRatioHist("macros/junews/lostleptons/default/RLF/default.root");			  
  //mc_tree->MakeDTTPDFs("macros/junews/lostleptons/default/");
  TChain* data = new TChain("reduced_tree");
  data->Add("reduced_trees/13TeV/skimmed/test_dataset/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78_skimmed.root");
  RTree* data_tree = new RTree(data);
  data_tree->MakeMHTPDFs("macros/junews/lostleptons/default/");
}
