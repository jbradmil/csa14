#include <TChain.h>
#include "TProofServ.h"
#include "TProof.h"
void MakePred()
{
  TChain *Predchain = new TChain("reduced_tree");
  //  Predchain->Add("reduced_trees/13TeV/skimmed/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78_skimmed.root");
  Predchain->Add("reduced_trees/13TeV/skimmed/test_dataset/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78_skimmed.root");
  Predchain->Process("macros/junews/lostleptons/arne/Prediction.C+");
}
