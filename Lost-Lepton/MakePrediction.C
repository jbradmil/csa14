#include <TChain.h>
#include "TProofServ.h"
#include "TProof.h"
//#include "EffMaker.h"
void MakePrediction()
{
// 	TString connect = gSystem->GetFromPipe("pod-info -c");
	//TProof *proof = TProof::Open("adraeger@nafhh-cms03.desy.de:21001");
//     	TProof *proof = TProof::Open("workers=10");
	//TProof *proof = TProof::Open("");
// 	TProof *proof = TProof::Open(connect);
	// analyse the trees
// 	TChain *Effchain = new TChain("TreeMaker2/PreSelection");
	// 	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola/*root");
	// 	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/WJetsToLNu_HT-200to400/*root");
	// 	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/WJetsToLNu_HT-400to600/*root");
	// 	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/WJetsToLNu_HT-600toInf/*root");
// 	TString connect = gSystem.GetFromPipe("pod-info -c");
// 	TProof *proof = TProof::Open(connect);
	TChain *Effchain = new TChain("TreeMaker2/PreSelection");
	Effchain->Add("../reduced_trees/13TeV/ttbar_arne/*.root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/WJetsToLNu_HT-200to400/*root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/WJetsToLNu_HT-400to600/*root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/WJetsToLNu_HT-600toInf/*root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/TBarToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/*.root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/TBarToLeptons_t-channel_Tune4C_CSA14_13TeV-aMCatNLO-tauola/*.root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/TToLeptons_s-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/*.root");
	// Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/TToLeptons_t-channel-CSA14_Tune4C_13TeV-aMCatNLO-tauola/*.root");
	
	

	//    	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/SMS-T1bbbb_2J_mGl-1000_mLSP-900/*root");
	//   	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/SMS-T1bbbb_2J_mGl-1500_mLSP-100/*root");
	//   	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/SMS-T1qqqq_2J_mGl-1000_mLSP-800/*root");
	//   	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/SMS-T1qqqq_2J_mGl-1400_mLSP-100/*root");
	//    	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/SMS-T1tttt_2J_mGl-1200_mLSP-800/*root");
	//   	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/SMS-T1tttt_2J_mGl-1500_mLSP-100/*root");
//  	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/QCD_HT_1000ToInf_13TeV-madgraph_v1/*root");
//  	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/QCD_HT_1000ToInf_13TeV-madgraph_v1_ext1/*root");
//  	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/QCD_HT_250To500_13TeV-madgraph_v1/*root");
//  	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/QCD_HT_250To500_13TeV-madgraph_v1_ext1/*root");
//  	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/QCD_HT-500To1000_13TeV-madgraph_v1/*root");
//  	Effchain->Add("/nfs/dust/cms/user/adraeger/phys14/mc/QCD_HT-500To1000_13TeV-madgraph_v1_ext1/*root");
//      	Effchain->SetProof();
	//	Effchain->Process("EffMaker.C+g",0,800000);
	Effchain->Process("Prediction.C+");
	//Effchain->Process("EffMaker.C+",0,800000);
//      	Effchain->SetProof(0);
//      	delete proof;
}
