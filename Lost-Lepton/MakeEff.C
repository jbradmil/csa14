#include <TChain.h>
#include "TProofServ.h"
#include "TProof.h"
//#include "EffMaker.h"
void MakeEff()
{
	// 	TProof *proof = TProof::Open("workers=20");
	TChain *Effchain = new TChain("LostLeptonExpectation");
	// 	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/*root");
	Effchain->Add("Expectation.root");
	Effchain->Process("EffMaker.C+");
}
