#include <TChain.h>
#include "TProofServ.h"
#include "TProof.h"
void MakeEff()
{
  TChain *Effchain = new TChain("LostLeptonExpectation");
  Effchain->Add("macros/junews/lostleptons/arne/Expectation/Expectation.root");
  Effchain->Process("macros/junews/lostleptons/arne/RTree.C+");
}
