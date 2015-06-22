// Developed by Arne-Rasmus Drager, adapted by Jack
// this script measures the lepton efficiencies from MC gen info and saves them to a root file

#define RTree_cxx
// The class definition in RTree.h has been generated automatically
// by the ROOT utility TTree::MakeSelector(). This class is derived
// from the ROOT class TSelector. For more information on the TSelector
// framework see $ROOTSYS/README/README.SELECTOR or the ROOT User Manual.

// The following methods are defined in this file:
//    Begin():        called every time a loop on the tree starts,
//                    a convenient place to create your histograms.
//    SlaveBegin():   called after Begin(), when on PROOF called only on the
//                    slave servers.
//    Process():      called for each event, in this function you decide what
//                    to read and fill your histograms.
//    SlaveTerminate: called at the end of the loop on the tree, when on PROOF
//                    called only on the slave servers.
//    Terminate():    called at the end of the loop on the tree,
//                    a convenient place to draw/fit your histograms.
//
// To use this file, try the following session on your Tree T:
//
// Root > T->Process("RTree.C")
// Root > T->Process("RTree.C","some options")
// Root > T->Process("RTree.C+")
//

#include "RTree.h"

using std::cout;
using std::endl;


void RTree::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).
  TH1::SetDefaultSumw2();
  TString option = GetOption();

}

void RTree::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();
	
  // total event count for expecation
	
  totalExpectation_ = new SearchBinEventCount("TotalLostLeptonExpecation");
  // purity
  // muon
  //1D
  MuPurityBTag_ = new TH1D("MuPurityBTag1D","MuPurityBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuPurityBTag_);
  MuPurityBTagFail_ = (TH1D*)MuPurityBTag_->Clone();
  MuPurityBTagFail_->SetName("MuPurityBTag1DFail");
  //GetOutputList()->Add(MuPurityBTagFail_); 
  
  MuPurityNJets_ = new TH1D("MuPurityNJets1D","MuPurityNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuPurityNJets_);
  MuPurityNJetsFail_ = (TH1D*)MuPurityNJets_->Clone();
  MuPurityNJetsFail_->SetName("MuPurityNJets1DFail");
  //GetOutputList()->Add(MuPurityNJetsFail_); 
  
  MuPurityHT_ = new TH1D("MuPurityHT1D","MuPurityHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuPurityHT_);
  MuPurityHTFail_ = (TH1D*)MuPurityHT_->Clone();
  MuPurityHTFail_->SetName("MuPurityHT1DFail");
  //GetOutputList()->Add(MuPurityHTFail_); 
  
  MuPurityMHT_ = new TH1D("MuPurityMHT1D","MuPurityMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuPurityMHT_);
  MuPurityMHTFail_ = (TH1D*)MuPurityMHT_->Clone();
  MuPurityMHTFail_->SetName("MuPurityMHT1DFail");
  //GetOutputList()->Add(MuPurityMHTFail_); 
  //2D
  MuonPurityMHTNJet = new TH2D("MuonPurity","MuonPurity",mupurityMHT_-1,muPurityMHT_,mupurityNJet_-1,muPurityNJet_);
  //GetOutputList()->Add(MuonPurityMHTNJet);
  MuonPurityMHTNJetFail = (TH2D*)MuonPurityMHTNJet->Clone();
  MuonPurityMHTNJetFail->SetName("MuonPurityFail");
  //GetOutputList()->Add(MuonPurityMHTNJetFail);  
  // electron
  //1D
  ElecPurityBTag_ = new TH1D("ElecPurityBTag1D","ElecPurityBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecPurityBTag_);
  ElecPurityBTagFail_ = (TH1D*)ElecPurityBTag_->Clone();
  ElecPurityBTagFail_->SetName("ElecPurityBTag1DFail");
  //GetOutputList()->Add(ElecPurityBTagFail_); 
  
  ElecPurityNJets_ = new TH1D("ElecPurityNJets1D","ElecPurityNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecPurityNJets_);
  ElecPurityNJetsFail_ = (TH1D*)ElecPurityNJets_->Clone();
  ElecPurityNJetsFail_->SetName("ElecPurityNJets1DFail");
  //GetOutputList()->Add(ElecPurityNJetsFail_); 
  
  ElecPurityHT_ = new TH1D("ElecPurityHT1D","ElecPurityHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecPurityHT_);
  ElecPurityHTFail_ = (TH1D*)ElecPurityHT_->Clone();
  ElecPurityHTFail_->SetName("ElecPurityHT1DFail");
  //GetOutputList()->Add(ElecPurityHTFail_); 
  
  ElecPurityMHT_ = new TH1D("ElecPurityMHT1D","ElecPurityMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecPurityMHT_);
  ElecPurityMHTFail_ = (TH1D*)ElecPurityMHT_->Clone();
  ElecPurityMHTFail_->SetName("ElecPurityMHT1DFail");
  //GetOutputList()->Add(ElecPurityMHTFail_); 
  //2D
  //std::cout<<"POin5"<<std::endl;
  ElecPurityMHTNJet = new TH2D("ElecPurity","ElecPurity",mupurityMHT_-1,muPurityMHT_,mupurityNJet_-1,muPurityNJet_);
  //GetOutputList()->Add(ElecPurityMHTNJet);
  ElecPurityMHTNJetFail = (TH2D*)ElecPurityMHTNJet->Clone();
  ElecPurityMHTNJetFail->SetName("ElecPurityFail");
  //GetOutputList()->Add(ElecPurityMHTNJetFail); 
  
  //
  
  MuPurityCheckBTag_ = new TH1D("MuPurityCheckBTag1D","MuPurityCheckBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuPurityCheckBTag_);
  MuPurityCheckBTagFail_ = (TH1D*)MuPurityCheckBTag_->Clone();
  MuPurityCheckBTagFail_->SetName("MuPurityCheckBTag1DFail");
  //GetOutputList()->Add(MuPurityCheckBTagFail_); 
  
  MuPurityCheckNJets_ = new TH1D("MuPurityCheckNJets1D","MuPurityCheckNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuPurityCheckNJets_);
  MuPurityCheckNJetsFail_ = (TH1D*)MuPurityCheckNJets_->Clone();
  MuPurityCheckNJetsFail_->SetName("MuPurityCheckNJets1DFail");
  //GetOutputList()->Add(MuPurityCheckNJetsFail_); 
  
  MuPurityCheckHT_ = new TH1D("MuPurityCheckHT1D","MuPurityCheckHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuPurityCheckHT_);
  MuPurityCheckHTFail_ = (TH1D*)MuPurityCheckHT_->Clone();
  MuPurityCheckHTFail_->SetName("MuPurityCheckHT1DFail");
  //GetOutputList()->Add(MuPurityCheckHTFail_); 
  
  MuPurityCheckMHT_ = new TH1D("MuPurityCheckMHT1D","MuPurityCheckMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuPurityCheckMHT_);
  MuPurityCheckMHTFail_ = (TH1D*)MuPurityCheckMHT_->Clone();
  MuPurityCheckMHTFail_->SetName("MuPurityCheckMHT1DFail");
  //GetOutputList()->Add(MuPurityCheckMHTFail_); 
  //2D
  MuonPurityCheckMHTNJet = new TH2D("MuonPurityCheck","MuonPurityCheck",mupurityMHT_-1,muPurityMHT_,mupurityNJet_-1,muPurityNJet_);
  //GetOutputList()->Add(MuonPurityCheckMHTNJet);
  MuonPurityCheckMHTNJetFail = (TH2D*)MuonPurityCheckMHTNJet->Clone();
  MuonPurityCheckMHTNJetFail->SetName("MuonPurityCheckFail");
  //GetOutputList()->Add(MuonPurityCheckMHTNJetFail);  
  // electron
  //1D
  ElecPurityCheckBTag_ = new TH1D("ElecPurityCheckBTag1D","ElecPurityCheckBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecPurityCheckBTag_);
  ElecPurityCheckBTagFail_ = (TH1D*)ElecPurityCheckBTag_->Clone();
  ElecPurityCheckBTagFail_->SetName("ElecPurityCheckBTag1DFail");
  //GetOutputList()->Add(ElecPurityCheckBTagFail_); 
  
  ElecPurityCheckNJets_ = new TH1D("ElecPurityCheckNJets1D","ElecPurityCheckNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecPurityCheckNJets_);
  ElecPurityCheckNJetsFail_ = (TH1D*)ElecPurityCheckNJets_->Clone();
  ElecPurityCheckNJetsFail_->SetName("ElecPurityCheckNJets1DFail");
  //GetOutputList()->Add(ElecPurityCheckNJetsFail_); 
  
  ElecPurityCheckHT_ = new TH1D("ElecPurityCheckHT1D","ElecPurityCheckHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecPurityCheckHT_);
  ElecPurityCheckHTFail_ = (TH1D*)ElecPurityCheckHT_->Clone();
  ElecPurityCheckHTFail_->SetName("ElecPurityCheckHT1DFail");
  //GetOutputList()->Add(ElecPurityCheckHTFail_); 
  
  ElecPurityCheckMHT_ = new TH1D("ElecPurityCheckMHT1D","ElecPurityCheckMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecPurityCheckMHT_);
  ElecPurityCheckMHTFail_ = (TH1D*)ElecPurityCheckMHT_->Clone();
  ElecPurityCheckMHTFail_->SetName("ElecPurityCheckMHT1DFail");
  //GetOutputList()->Add(ElecPurityCheckMHTFail_); 
  //2D
  //  std::cout<<"POin5"<<std::endl;
  ElecPurityCheckMHTNJet = new TH2D("ElecPurityCheck","ElecPurityCheck",mupurityMHT_-1,muPurityMHT_,mupurityNJet_-1,muPurityNJet_);
  //GetOutputList()->Add(ElecPurityCheckMHTNJet);
  ElecPurityCheckMHTNJetFail = (TH2D*)ElecPurityCheckMHTNJet->Clone();
  ElecPurityCheckMHTNJetFail->SetName("ElecPurityCheckFail");
  //GetOutputList()->Add(ElecPurityCheckMHTNJetFail); 
  
  
  // acceptance
  //muon
  //1D
  MuAccBTag_ = new TH1D("MuAccBTag1D","MuAccBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuAccBTag_);
  MuAccBTagFail_ = (TH1D*)MuAccBTag_->Clone();
  MuAccBTagFail_->SetName("MuAccBTag1DFail");
  //GetOutputList()->Add(MuAccBTagFail_); 
  
  MuAccNJets_ = new TH1D("MuAccNJets1D","MuAccNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuAccNJets_);
  MuAccNJetsFail_ = (TH1D*)MuAccNJets_->Clone();
  MuAccNJetsFail_->SetName("MuAccNJets1DFail");
  //GetOutputList()->Add(MuAccNJetsFail_); 
  
  MuAccHT_ = new TH1D("MuAccHT1D","MuAccHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuAccHT_);
  MuAccHTFail_ = (TH1D*)MuAccHT_->Clone();
  MuAccHTFail_->SetName("MuAccHT1DFail");
  //GetOutputList()->Add(MuAccHTFail_); 
  
  MuAccMHT_ = new TH1D("MuAccMHT1D","MuAccMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuAccMHT_);
  MuAccMHTFail_ = (TH1D*)MuAccMHT_->Clone();
  MuAccMHTFail_->SetName("MuAccMHT1DFail");
  //GetOutputList()->Add(MuAccMHTFail_); 
  
  //2D
  MuAccHTNJets_ = new TH2D("MuAccHTNJets","MuAccHTNJets",muaccHT_-1,muAccHT_, muaccNJets_-1, muAccNJets_);
  //GetOutputList()->Add(MuAccHTNJets_);
  MuAccHTNJetsFail_ = (TH2D*)MuAccHTNJets_->Clone();
  MuAccHTNJetsFail_->SetName("MuAccHTNJetsFail");
  //GetOutputList()->Add(MuAccHTNJetsFail_); 
	
  MuAccMHTNJets_ = new TH2D("MuAccMHTNJets","MuAccMHTNJets",muaccMHT_-1,muAccMHT_, muaccNJets_-1, muAccNJets_);
  //GetOutputList()->Add(MuAccMHTNJets_);
  MuAccMHTNJetsFail_ = (TH2D*)MuAccMHTNJets_->Clone();
  MuAccMHTNJetsFail_->SetName("MuAccMHTNJetsFail");
  //GetOutputList()->Add(MuAccMHTNJetsFail_); 
	
  MuAccBTagNJets_ = new TH2D("MuAccBTagNJets","MuAccBTagNJets",muaccBTags_-1,muAccBTags_, muaccNJets_-1, muAccNJets_);
  //GetOutputList()->Add(MuAccBTagNJets_);
  MuAccBTagNJetsFail_ = (TH2D*)MuAccBTagNJets_->Clone();
  MuAccBTagNJetsFail_->SetName("MuAccBTagNJetsFail");
	
  //GetOutputList()->Add(MuAccBTagNJetsFail_); 
	
  MuAccMHTNJetsB0_ = new TH2D("MuAccMHTNJetsB0","MuAccMHTNJetsB0",muaccMHT_-1,muAccMHT_, muaccNJets_-1, muAccNJets_);
  //GetOutputList()->Add(MuAccMHTNJets_);
  MuAccMHTNJetsB0Fail_ = (TH2D*)MuAccMHTNJetsB0_->Clone();
  MuAccMHTNJetsB0Fail_->SetName("MuAccMHTNJetsB0Fail");
	
  MuAccMHTNJetsB1_Inf_ = new TH2D("MuAccMHTNJetsB1_Inf","MuAccMHTNJetsB1_Inf",muaccMHT_-1,muAccMHT_, muaccNJets_-1, muAccNJets_);
  //GetOutputList()->Add(MuAccMHTNJets_);
  MuAccMHTNJetsB1_InfFail_ = (TH2D*)MuAccMHTNJetsB1_Inf_->Clone();
  MuAccMHTNJetsB1_InfFail_->SetName("MuAccMHTNJetsB1_InfFail");
  
  //muon
  //1D
  ElecAccBTag_ = new TH1D("ElecAccBTag1D","ElecAccBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecAccBTag_);
  ElecAccBTagFail_ = (TH1D*)ElecAccBTag_->Clone();
  ElecAccBTagFail_->SetName("ElecAccBTag1DFail");
  //GetOutputList()->Add(ElecAccBTagFail_); 
  
  ElecAccNJets_ = new TH1D("ElecAccNJets1D","ElecAccNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecAccNJets_);
  ElecAccNJetsFail_ = (TH1D*)ElecAccNJets_->Clone();
  ElecAccNJetsFail_->SetName("ElecAccNJets1DFail");
  //GetOutputList()->Add(ElecAccNJetsFail_); 
  
  ElecAccHT_ = new TH1D("ElecAccHT1D","ElecAccHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecAccHT_);
  ElecAccHTFail_ = (TH1D*)ElecAccHT_->Clone();
  ElecAccHTFail_->SetName("ElecAccHT1DFail");
  //GetOutputList()->Add(ElecAccHTFail_); 
  
  ElecAccMHT_ = new TH1D("ElecAccMHT1D","ElecAccMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecAccMHT_);
  ElecAccMHTFail_ = (TH1D*)ElecAccMHT_->Clone();
  ElecAccMHTFail_->SetName("ElecAccMHT1DFail");
  //GetOutputList()->Add(ElecAccMHTFail_); 
  
  //2D
  ElecAccHTNJets_ = new TH2D("ElecAccHTNJets","ElecAccHTNJets",elaccHT_-1,elecAccHT_, elaccNJets_-1, elecAccNJets_);
  //GetOutputList()->Add(ElecAccHTNJets_);
  ElecAccHTNJetsFail_ = (TH2D*)ElecAccHTNJets_->Clone();
  ElecAccHTNJetsFail_->SetName("ElecAccHTNJetsFail");
  //GetOutputList()->Add(ElecAccHTNJetsFail_); 
	
  ElecAccMHTNJets_ = new TH2D("ElecAccMHTNJets","ElecAccMHTNJets",elaccMHT_-1,elecAccMHT_, elaccNJets_-1, elecAccNJets_);
  //GetOutputList()->Add(ElecAccMHTNJets_);
  ElecAccMHTNJetsFail_ = (TH2D*)ElecAccMHTNJets_->Clone();
  ElecAccMHTNJetsFail_->SetName("ElecAccMHTNJetsFail");
  //GetOutputList()->Add(ElecAccMHTNJetsFail_); 
	
  ElecAccBTagNJets_ = new TH2D("ElecAccBTagNJets","ElecAccBTagNJets",elaccBTags_-1,elecAccBTags_, elaccNJets_-1, elecAccNJets_);
  //GetOutputList()->Add(ElecAccBTagNJets_);
  ElecAccBTagNJetsFail_ = (TH2D*)ElecAccBTagNJets_->Clone();
  ElecAccBTagNJetsFail_->SetName("ElecAccBTagNJetsFail");
  //GetOutputList()->Add(ElecAccBTagNJetsFail_); 
	
  ElecAccMHTNJetsB0_ = new TH2D("ElecAccMHTNJetsB0","ElecAccMHTNJetsB0",elaccMHT_-1,elecAccMHT_, elaccNJets_-1, elecAccNJets_);
  //GetOutputList()->Add(ElecAccMHTNJets_);
  ElecAccMHTNJetsB0Fail_ = (TH2D*)ElecAccMHTNJetsB0_->Clone();
  ElecAccMHTNJetsB0Fail_->SetName("ElecAccMHTNJetsB0Fail");
	
  ElecAccMHTNJetsB1_Inf_ = new TH2D("ElecAccMHTNJetsB1_Inf","ElecAccMHTNJetsB1_Inf",elaccMHT_-1,elecAccMHT_, elaccNJets_-1, elecAccNJets_);
  //GetOutputList()->Add(ElecAccMHTNJets_);
  ElecAccMHTNJetsB1_InfFail_ = (TH2D*)ElecAccMHTNJetsB1_Inf_->Clone();
  ElecAccMHTNJetsB1_InfFail_->SetName("ElecAccMHTNJetsB1_InfFail");
  
  // reconstruction
  //muon
  //1D
  MuRecoBTag_ = new TH1D("MuRecoBTag1D","MuRecoBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuRecoBTag_);
  MuRecoBTagFail_ = (TH1D*)MuRecoBTag_->Clone();
  MuRecoBTagFail_->SetName("MuRecoBTag1DFail");
  //GetOutputList()->Add(MuRecoBTagFail_); 
  
  MuRecoNJets_ = new TH1D("MuRecoNJets1D","MuRecoNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuRecoNJets_);
  MuRecoNJetsFail_ = (TH1D*)MuRecoNJets_->Clone();
  MuRecoNJetsFail_->SetName("MuRecoNJets1DFail");
  //GetOutputList()->Add(MuRecoNJetsFail_); 
  
  MuRecoHT_ = new TH1D("MuRecoHT1D","MuRecoHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuRecoHT_);
  MuRecoHTFail_ = (TH1D*)MuRecoHT_->Clone();
  MuRecoHTFail_->SetName("MuRecoHT1DFail");
  //GetOutputList()->Add(MuRecoHTFail_); 
  
  MuRecoMHT_ = new TH1D("MuRecoMHT1D","MuRecoMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuRecoMHT_);
  MuRecoMHTFail_ = (TH1D*)MuRecoMHT_->Clone();
  MuRecoMHTFail_->SetName("MuRecoMHT1DFail");
  //GetOutputList()->Add(MuRecoMHTFail_); 
  
  //el
  //1D
  ElecRecoBTag_ = new TH1D("ElecRecoBTag1D","ElecRecoBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecRecoBTag_);
  ElecRecoBTagFail_ = (TH1D*)ElecRecoBTag_->Clone();
  ElecRecoBTagFail_->SetName("ElecRecoBTag1DFail");
  //GetOutputList()->Add(ElecRecoBTagFail_); 
  
  ElecRecoNJets_ = new TH1D("ElecRecoNJets1D","ElecRecoNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecRecoNJets_);
  ElecRecoNJetsFail_ = (TH1D*)ElecRecoNJets_->Clone();
  ElecRecoNJetsFail_->SetName("ElecRecoNJets1DFail");
  //GetOutputList()->Add(ElecRecoNJetsFail_); 
  
  ElecRecoHT_ = new TH1D("ElecRecoHT1D","ElecRecoHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecRecoHT_);
  ElecRecoHTFail_ = (TH1D*)ElecRecoHT_->Clone();
  ElecRecoHTFail_->SetName("ElecRecoHT1DFail");
  //GetOutputList()->Add(ElecRecoHTFail_); 
  
  ElecRecoMHT_ = new TH1D("ElecRecoMHT1D","ElecRecoMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecRecoMHT_);
  ElecRecoMHTFail_ = (TH1D*)ElecRecoMHT_->Clone();
  ElecRecoMHTFail_->SetName("ElecRecoMHT1DFail");
  //GetOutputList()->Add(ElecRecoMHTFail_); 
  
  // isolation
  //muon
  //1D
  MuIsoBTag_ = new TH1D("MuIsoBTag1D","MuIsoBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuIsoBTag_);
  MuIsoBTagFail_ = (TH1D*)MuIsoBTag_->Clone();
  MuIsoBTagFail_->SetName("MuIsoBTag1DFail");
  //GetOutputList()->Add(MuIsoBTagFail_); 
  
  MuIsoNJets_ = new TH1D("MuIsoNJets1D","MuIsoNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuIsoNJets_);
  MuIsoNJetsFail_ = (TH1D*)MuIsoNJets_->Clone();
  MuIsoNJetsFail_->SetName("MuIsoNJets1DFail");
  //GetOutputList()->Add(MuIsoNJetsFail_); 
  
  MuIsoHT_ = new TH1D("MuIsoHT1D","MuIsoHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuIsoHT_);
  MuIsoHTFail_ = (TH1D*)MuIsoHT_->Clone();
  MuIsoHTFail_->SetName("MuIsoHT1DFail");
  //GetOutputList()->Add(MuIsoHTFail_); 
  
  MuIsoMHT_ = new TH1D("MuIsoMHT1D","MuIsoMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuIsoMHT_);
  MuIsoMHTFail_ = (TH1D*)MuIsoMHT_->Clone();
  MuIsoMHTFail_->SetName("MuIsoMHT1DFail");
  //GetOutputList()->Add(MuIsoMHTFail_); 
  
  //el
  //1D
  ElecIsoBTag_ = new TH1D("ElecIsoBTag1D","ElecIsoBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecIsoBTag_);
  ElecIsoBTagFail_ = (TH1D*)ElecIsoBTag_->Clone();
  ElecIsoBTagFail_->SetName("ElecIsoBTag1DFail");
  //GetOutputList()->Add(ElecIsoBTagFail_); 
  
  ElecIsoNJets_ = new TH1D("ElecIsoNJets1D","ElecIsoNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecIsoNJets_);
  ElecIsoNJetsFail_ = (TH1D*)ElecIsoNJets_->Clone();
  ElecIsoNJetsFail_->SetName("ElecIsoNJets1DFail");
  //GetOutputList()->Add(ElecIsoNJetsFail_); 
  
  ElecIsoHT_ = new TH1D("ElecIsoHT1D","ElecIsoHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecIsoHT_);
  ElecIsoHTFail_ = (TH1D*)ElecIsoHT_->Clone();
  ElecIsoHTFail_->SetName("ElecIsoHT1DFail");
  //GetOutputList()->Add(ElecIsoHTFail_); 
  
  ElecIsoMHT_ = new TH1D("ElecIsoMHT1D","ElecIsoMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecIsoMHT_);
  ElecIsoMHTFail_ = (TH1D*)ElecIsoMHT_->Clone();
  ElecIsoMHTFail_->SetName("ElecIsoMHT1DFail");
  //GetOutputList()->Add(ElecIsoMHTFail_); 
  
  // mtw
  //muon
  //1D
  MuMTWBTag_ = new TH1D("MuMTWBTag1D","MuMTWBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuMTWBTag_);
  MuMTWBTagFail_ = (TH1D*)MuMTWBTag_->Clone();
  MuMTWBTagFail_->SetName("MuMTWBTag1DFail");
  //GetOutputList()->Add(MuMTWBTagFail_); 
  
  MuMTWNJets_ = new TH1D("MuMTWNJets1D","MuMTWNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuMTWNJets_);
  MuMTWNJetsFail_ = (TH1D*)MuMTWNJets_->Clone();
  MuMTWNJetsFail_->SetName("MuMTWNJets1DFail");
  //GetOutputList()->Add(MuMTWNJetsFail_); 
  
  MuMTWHT_ = new TH1D("MuMTWHT1D","MuMTWHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuMTWHT_);
  MuMTWHTFail_ = (TH1D*)MuMTWHT_->Clone();
  MuMTWHTFail_->SetName("MuMTWHT1DFail");
  //GetOutputList()->Add(MuMTWHTFail_); 
  
  MuMTWMHT_ = new TH1D("MuMTWMHT1D","MuMTWMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuMTWMHT_);
  MuMTWMHTFail_ = (TH1D*)MuMTWMHT_->Clone();
  MuMTWMHTFail_->SetName("MuMTWMHT1DFail");
  //GetOutputList()->Add(MuMTWMHTFail_); 
  
  //muon
  //1D
  ElecMTWBTag_ = new TH1D("ElecMTWBTag1D","ElecMTWBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecMTWBTag_);
  ElecMTWBTagFail_ = (TH1D*)ElecMTWBTag_->Clone();
  ElecMTWBTagFail_->SetName("ElecMTWBTag1DFail");
  //GetOutputList()->Add(ElecMTWBTagFail_); 
  
  ElecMTWNJets_ = new TH1D("ElecMTWNJets1D","ElecMTWNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecMTWNJets_);
  ElecMTWNJetsFail_ = (TH1D*)ElecMTWNJets_->Clone();
  ElecMTWNJetsFail_->SetName("ElecMTWNJets1DFail");
  //GetOutputList()->Add(ElecMTWNJetsFail_); 
  
  ElecMTWHT_ = new TH1D("ElecMTWHT1D","ElecMTWHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecMTWHT_);
  ElecMTWHTFail_ = (TH1D*)ElecMTWHT_->Clone();
  ElecMTWHTFail_->SetName("ElecMTWHT1DFail");
  //GetOutputList()->Add(ElecMTWHTFail_); 
  
  ElecMTWMHT_ = new TH1D("ElecMTWMHT1D","ElecMTWMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecMTWMHT_);
  ElecMTWMHTFail_ = (TH1D*)ElecMTWMHT_->Clone();
  ElecMTWMHTFail_->SetName("ElecMTWMHT1DFail");
  //GetOutputList()->Add(ElecMTWMHTFail_); 
  
  // di lep
  
  // mtw
  //muon
  //1D
  MuDiLepBTag_ = new TH1D("MuDiLepBTag1D","MuDiLepBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuDiLepBTag_);
  MuDiLepBTagFail_ = (TH1D*)MuDiLepBTag_->Clone();
  MuDiLepBTagFail_->SetName("MuDiLepBTag1DFail");
  //GetOutputList()->Add(MuDiLepBTagFail_); 
  
  MuDiLepNJets_ = new TH1D("MuDiLepNJets1D","MuDiLepNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuDiLepNJets_);
  MuDiLepNJetsFail_ = (TH1D*)MuDiLepNJets_->Clone();
  MuDiLepNJetsFail_->SetName("MuDiLepNJets1DFail");
  //GetOutputList()->Add(MuDiLepNJetsFail_); 
  
  MuDiLepHT_ = new TH1D("MuDiLepHT1D","MuDiLepHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuDiLepHT_);
  MuDiLepHTFail_ = (TH1D*)MuDiLepHT_->Clone();
  MuDiLepHTFail_->SetName("MuDiLepHT1DFail");
  //GetOutputList()->Add(MuDiLepHTFail_); 
  
  MuDiLepMHT_ = new TH1D("MuDiLepMHT1D","MuDiLepMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuDiLepMHT_);
  MuDiLepMHTFail_ = (TH1D*)MuDiLepMHT_->Clone();
  MuDiLepMHTFail_->SetName("MuDiLepMHT1DFail");
  //GetOutputList()->Add(MuDiLepMHTFail_); 
  
  //muon
  //1D
  ElecDiLepBTag_ = new TH1D("ElecDiLepBTag1D","ElecDiLepBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecDiLepBTag_);
  ElecDiLepBTagFail_ = (TH1D*)ElecDiLepBTag_->Clone();
  ElecDiLepBTagFail_->SetName("ElecDiLepBTag1DFail");
  //GetOutputList()->Add(ElecDiLepBTagFail_); 
  
  ElecDiLepNJets_ = new TH1D("ElecDiLepNJets1D","ElecDiLepNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecDiLepNJets_);
  ElecDiLepNJetsFail_ = (TH1D*)ElecDiLepNJets_->Clone();
  ElecDiLepNJetsFail_->SetName("ElecDiLepNJets1DFail");
  //GetOutputList()->Add(ElecDiLepNJetsFail_); 
  
  ElecDiLepHT_ = new TH1D("ElecDiLepHT1D","ElecDiLepHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecDiLepHT_);
  ElecDiLepHTFail_ = (TH1D*)ElecDiLepHT_->Clone();
  ElecDiLepHTFail_->SetName("ElecDiLepHT1DFail");
  //GetOutputList()->Add(ElecDiLepHTFail_); 
  
  ElecDiLepMHT_ = new TH1D("ElecDiLepMHT1D","ElecDiLepMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecDiLepMHT_);
  ElecDiLepMHTFail_ = (TH1D*)ElecDiLepMHT_->Clone();
  ElecDiLepMHTFail_->SetName("ElecDiLepMHT1DFail");
  //GetOutputList()->Add(ElecDiLepMHTFail_); 
  
  
  // mtw
  //muon
  //1D
  MuDiLepMTWBTag_ = new TH1D("MuDiLepMTWBTag1D","MuDiLepMTWBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuDiLepMTWBTag_);
  MuDiLepMTWBTagFail_ = (TH1D*)MuDiLepMTWBTag_->Clone();
  MuDiLepMTWBTagFail_->SetName("MuDiLepMTWBTag1DFail");
  //GetOutputList()->Add(MuDiLepMTWBTagFail_); 
  
  MuDiLepMTWNJets_ = new TH1D("MuDiLepMTWNJets1D","MuDiLepMTWNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuDiLepMTWNJets_);
  MuDiLepMTWNJetsFail_ = (TH1D*)MuDiLepMTWNJets_->Clone();
  MuDiLepMTWNJetsFail_->SetName("MuDiLepMTWNJets1DFail");
  //GetOutputList()->Add(MuDiLepMTWNJetsFail_); 
  
  MuDiLepMTWHT_ = new TH1D("MuDiLepMTWHT1D","MuDiLepMTWHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuDiLepMTWHT_);
  MuDiLepMTWHTFail_ = (TH1D*)MuDiLepMTWHT_->Clone();
  MuDiLepMTWHTFail_->SetName("MuDiLepMTWHT1DFail");
  //GetOutputList()->Add(MuDiLepMTWHTFail_); 
  
  MuDiLepMTWMHT_ = new TH1D("MuDiLepMTWMHT1D","MuDiLepMTWMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuDiLepMTWMHT_);
  MuDiLepMTWMHTFail_ = (TH1D*)MuDiLepMTWMHT_->Clone();
  MuDiLepMTWMHTFail_->SetName("MuDiLepMTWMHT1DFail");
  //GetOutputList()->Add(MuDiLepMTWMHTFail_); 
  
  //muon
  //1D
  ElecDiLepMTWBTag_ = new TH1D("ElecDiLepMTWBTag1D","ElecDiLepMTWBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecDiLepMTWBTag_);
  ElecDiLepMTWBTagFail_ = (TH1D*)ElecDiLepMTWBTag_->Clone();
  ElecDiLepMTWBTagFail_->SetName("ElecDiLepMTWBTag1DFail");
  //GetOutputList()->Add(ElecDiLepMTWBTagFail_); 
  
  ElecDiLepMTWNJets_ = new TH1D("ElecDiLepMTWNJets1D","ElecDiLepMTWNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecDiLepMTWNJets_);
  ElecDiLepMTWNJetsFail_ = (TH1D*)ElecDiLepMTWNJets_->Clone();
  ElecDiLepMTWNJetsFail_->SetName("ElecDiLepMTWNJets1DFail");
  //GetOutputList()->Add(ElecDiLepMTWNJetsFail_); 
  
  ElecDiLepMTWHT_ = new TH1D("ElecDiLepMTWHT1D","ElecDiLepMTWHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecDiLepMTWHT_);
  ElecDiLepMTWHTFail_ = (TH1D*)ElecDiLepMTWHT_->Clone();
  ElecDiLepMTWHTFail_->SetName("ElecDiLepMTWHT1DFail");
  //GetOutputList()->Add(ElecDiLepMTWHTFail_); 
  
  ElecDiLepMTWMHT_ = new TH1D("ElecDiLepMTWMHT1D","ElecDiLepMTWMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecDiLepMTWMHT_);
  ElecDiLepMTWMHTFail_ = (TH1D*)ElecDiLepMTWMHT_->Clone();
  ElecDiLepMTWMHTFail_->SetName("ElecDiLepMTWMHT1DFail");
  //GetOutputList()->Add(ElecDiLepMTWMHTFail_); 
  
  
  // mtw
  //muon
  //1D
  MuDiLepContributionBTag_ = new TH1D("MuDiLepContributionBTag1D","MuDiLepContributionBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuDiLepContributionBTag_);
  MuDiLepContributionBTagFail_ = (TH1D*)MuDiLepContributionBTag_->Clone();
  MuDiLepContributionBTagFail_->SetName("MuDiLepContributionBTag1DFail");
  //GetOutputList()->Add(MuDiLepContributionBTagFail_); 
  
  MuDiLepContributionNJets_ = new TH1D("MuDiLepContributionNJets1D","MuDiLepContributionNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuDiLepContributionNJets_);
  MuDiLepContributionNJetsFail_ = (TH1D*)MuDiLepContributionNJets_->Clone();
  MuDiLepContributionNJetsFail_->SetName("MuDiLepContributionNJets1DFail");
  //GetOutputList()->Add(MuDiLepContributionNJetsFail_); 
  
  MuDiLepContributionHT_ = new TH1D("MuDiLepContributionHT1D","MuDiLepContributionHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuDiLepContributionHT_);
  MuDiLepContributionHTFail_ = (TH1D*)MuDiLepContributionHT_->Clone();
  MuDiLepContributionHTFail_->SetName("MuDiLepContributionHT1DFail");
  //GetOutputList()->Add(MuDiLepContributionHTFail_); 
  
  MuDiLepContributionMHT_ = new TH1D("MuDiLepContributionMHT1D","MuDiLepContributionMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuDiLepContributionMHT_);
  MuDiLepContributionMHTFail_ = (TH1D*)MuDiLepContributionMHT_->Clone();
  MuDiLepContributionMHTFail_->SetName("MuDiLepContributionMHT1DFail");
  //GetOutputList()->Add(MuDiLepContributionMHTFail_); 
  
  //muon
  //1D
  ElecDiLepContributionBTag_ = new TH1D("ElecDiLepContributionBTag1D","ElecDiLepContributionBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecDiLepContributionBTag_);
  ElecDiLepContributionBTagFail_ = (TH1D*)ElecDiLepContributionBTag_->Clone();
  ElecDiLepContributionBTagFail_->SetName("ElecDiLepContributionBTag1DFail");
  //GetOutputList()->Add(ElecDiLepContributionBTagFail_); 
  
  ElecDiLepContributionNJets_ = new TH1D("ElecDiLepContributionNJets1D","ElecDiLepContributionNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecDiLepContributionNJets_);
  ElecDiLepContributionNJetsFail_ = (TH1D*)ElecDiLepContributionNJets_->Clone();
  ElecDiLepContributionNJetsFail_->SetName("ElecDiLepContributionNJets1DFail");
  //GetOutputList()->Add(ElecDiLepContributionNJetsFail_); 
  
  ElecDiLepContributionHT_ = new TH1D("ElecDiLepContributionHT1D","ElecDiLepContributionHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecDiLepContributionHT_);
  ElecDiLepContributionHTFail_ = (TH1D*)ElecDiLepContributionHT_->Clone();
  ElecDiLepContributionHTFail_->SetName("ElecDiLepContributionHT1DFail");
  //GetOutputList()->Add(ElecDiLepContributionHTFail_); 
  
  ElecDiLepContributionMHT_ = new TH1D("ElecDiLepContributionMHT1D","ElecDiLepContributionMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecDiLepContributionMHT_);
  ElecDiLepContributionMHTFail_ = (TH1D*)ElecDiLepContributionMHT_->Clone();
  ElecDiLepContributionMHTFail_->SetName("ElecDiLepContributionMHT1DFail");
  //GetOutputList()->Add(ElecDiLepContributionMHTFail_); 
  
  
  // mtw
  //muon
  //1D
  MuDiLepContributionMTWBTag_ = new TH1D("MuDiLepContributionMTWBTag1D","MuDiLepContributionMTWBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(MuDiLepContributionMTWBTag_);
  MuDiLepContributionMTWBTagFail_ = (TH1D*)MuDiLepContributionMTWBTag_->Clone();
  MuDiLepContributionMTWBTagFail_->SetName("MuDiLepContributionMTWBTag1DFail");
  //GetOutputList()->Add(MuDiLepContributionMTWBTagFail_); 
  
  MuDiLepContributionMTWNJets_ = new TH1D("MuDiLepContributionMTWNJets1D","MuDiLepContributionMTWNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(MuDiLepContributionMTWNJets_);
  MuDiLepContributionMTWNJetsFail_ = (TH1D*)MuDiLepContributionMTWNJets_->Clone();
  MuDiLepContributionMTWNJetsFail_->SetName("MuDiLepContributionMTWNJets1DFail");
  //GetOutputList()->Add(MuDiLepContributionMTWNJetsFail_); 
  
  MuDiLepContributionMTWHT_ = new TH1D("MuDiLepContributionMTWHT1D","MuDiLepContributionMTWHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(MuDiLepContributionMTWHT_);
  MuDiLepContributionMTWHTFail_ = (TH1D*)MuDiLepContributionMTWHT_->Clone();
  MuDiLepContributionMTWHTFail_->SetName("MuDiLepContributionMTWHT1DFail");
  //GetOutputList()->Add(MuDiLepContributionMTWHTFail_); 
  
  MuDiLepContributionMTWMHT_ = new TH1D("MuDiLepContributionMTWMHT1D","MuDiLepContributionMTWMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(MuDiLepContributionMTWMHT_);
  MuDiLepContributionMTWMHTFail_ = (TH1D*)MuDiLepContributionMTWMHT_->Clone();
  MuDiLepContributionMTWMHTFail_->SetName("MuDiLepContributionMTWMHT1DFail");
  //GetOutputList()->Add(MuDiLepContributionMTWMHTFail_); 
  
  //muon
  //1D
  ElecDiLepContributionMTWBTag_ = new TH1D("ElecDiLepContributionMTWBTag1D","ElecDiLepContributionMTWBTag1D",oneDBJets_-1,OneDBJets_);
  //GetOutputList()->Add(ElecDiLepContributionMTWBTag_);
  ElecDiLepContributionMTWBTagFail_ = (TH1D*)ElecDiLepContributionMTWBTag_->Clone();
  ElecDiLepContributionMTWBTagFail_->SetName("ElecDiLepContributionMTWBTag1DFail");
  //GetOutputList()->Add(ElecDiLepContributionMTWBTagFail_); 
  
  ElecDiLepContributionMTWNJets_ = new TH1D("ElecDiLepContributionMTWNJets1D","ElecDiLepContributionMTWNJets1D",oneDNJets_-1,OneDNJets_);
  //GetOutputList()->Add(ElecDiLepContributionMTWNJets_);
  ElecDiLepContributionMTWNJetsFail_ = (TH1D*)ElecDiLepContributionMTWNJets_->Clone();
  ElecDiLepContributionMTWNJetsFail_->SetName("ElecDiLepContributionMTWNJets1DFail");
  //GetOutputList()->Add(ElecDiLepContributionMTWNJetsFail_); 
  
  ElecDiLepContributionMTWHT_ = new TH1D("ElecDiLepContributionMTWHT1D","ElecDiLepContributionMTWHT1D",oneDHT_-1,OneDHT_);
  //GetOutputList()->Add(ElecDiLepContributionMTWHT_);
  ElecDiLepContributionMTWHTFail_ = (TH1D*)ElecDiLepContributionMTWHT_->Clone();
  ElecDiLepContributionMTWHTFail_->SetName("ElecDiLepContributionMTWHT1DFail");
  //GetOutputList()->Add(ElecDiLepContributionMTWHTFail_); 

  ElecDiLepContributionMTWMHT_ = new TH1D("ElecDiLepContributionMTWMHT1D","ElecDiLepContributionMTWMHT1D",oneDMHT_-1,OneDMHT_);
  //GetOutputList()->Add(ElecDiLepContributionMTWMHT_);
  ElecDiLepContributionMTWMHTFail_ = (TH1D*)ElecDiLepContributionMTWMHT_->Clone();
  ElecDiLepContributionMTWMHTFail_->SetName("ElecDiLepContributionMTWMHT1DFail");
  //GetOutputList()->Add(ElecDiLepContributionMTWMHTFail_); 

  MuPurityPT_ = new TH1D("MuPurityPT","MuPurityPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuPurityPT_);
  MuPurityPTFail_ = (TH1D*)MuPurityPT_->Clone();
  MuPurityPTFail_->SetName("MuPurityPTFail");
  //GetOutputList()->Add(MuPurityPTFail_); 
  
  MuPurityActivity_ = new TH1D("MuPurityActivity","MuPurityActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuPurityActivity_);
  MuPurityActivityFail_ = (TH1D*)MuPurityActivity_->Clone();
  MuPurityActivityFail_->SetName("MuPurityActivityFail");
  //GetOutputList()->Add(MuPurityActivityFail_); 
  
  MuAccPT_ = new TH1D("MuAccPT","MuAccPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuAccPT_);
  MuAccPTFail_ = (TH1D*)MuAccPT_->Clone();
  MuAccPTFail_->SetName("MuAccPTFail");
  //GetOutputList()->Add(MuAccPTFail_); 
  
  MuAccActivity_ = new TH1D("MuAccActivity","MuAccActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuAccActivity_);
  MuAccActivityFail_ = (TH1D*)MuAccActivity_->Clone();
  MuAccActivityFail_->SetName("MuAccActivityFail");
  //GetOutputList()->Add(MuAccActivityFail_); 
  
  
  MuRecoPT_ = new TH1D("MuRecoPT","MuRecoPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuRecoPT_);
  MuRecoPTFail_ = (TH1D*)MuRecoPT_->Clone();
  MuRecoPTFail_->SetName("MuRecoPTFail");
  //GetOutputList()->Add(MuRecoPTFail_); 
  
  MuRecoActivity_ = new TH1D("MuRecoActivity","MuRecoActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuRecoActivity_);
  MuRecoActivityFail_ = (TH1D*)MuRecoActivity_->Clone();
  MuRecoActivityFail_->SetName("MuRecoActivityFail");
  //GetOutputList()->Add(MuRecoActivityFail_); 
  
  MuIsoPT_ = new TH1D("MuIsoPT","MuIsoPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuIsoPT_);
  MuIsoPTFail_ = (TH1D*)MuIsoPT_->Clone();
  MuIsoPTFail_->SetName("MuIsoPTFail");
  //GetOutputList()->Add(MuIsoPTFail_); 
  
  MuIsoActivity_ = new TH1D("MuIsoActivity","MuIsoActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuIsoActivity_);
  MuIsoActivityFail_ = (TH1D*)MuIsoActivity_->Clone();
  MuIsoActivityFail_->SetName("MuIsoActivityFail");
  //GetOutputList()->Add(MuIsoActivityFail_); 
  
  MuMTWPT_ = new TH1D("MuMTWPT","MuMTWPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuMTWPT_);
  MuMTWPTFail_ = (TH1D*)MuMTWPT_->Clone();
  MuMTWPTFail_->SetName("MuMTWPTFail");
  //GetOutputList()->Add(MuMTWPTFail_); 
  
  MuMTWActivity_ = new TH1D("MuMTWActivity","MuMTWActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuMTWActivity_);
  MuMTWActivityFail_ = (TH1D*)MuMTWActivity_->Clone();
  MuMTWActivityFail_->SetName("MuMTWActivityFail");
  //GetOutputList()->Add(MuMTWActivityFail_); 
  
  MuDiLepPT_ = new TH1D("MuDiLepPT","MuDiLepPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuDiLepPT_);
  MuDiLepPTFail_ = (TH1D*)MuDiLepPT_->Clone();
  MuDiLepPTFail_->SetName("MuDiLepPTFail");
  //GetOutputList()->Add(MuDiLepPTFail_); 
  
  MuDiLepActivity_ = new TH1D("MuDiLepActivity","MuDiLepActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuDiLepActivity_);
  MuDiLepActivityFail_ = (TH1D*)MuDiLepActivity_->Clone();
  MuDiLepActivityFail_->SetName("MuDiLepActivityFail");
  //GetOutputList()->Add(MuDiLepActivityFail_); 
  
  
  MuDiLepMTWPT_ = new TH1D("MuDiLepMTWPT","MuDiLepMTWPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(MuDiLepMTWPT_);
  MuDiLepMTWPTFail_ = (TH1D*)MuDiLepMTWPT_->Clone();
  MuDiLepMTWPTFail_->SetName("MuDiLepMTWPTFail");
  //GetOutputList()->Add(MuDiLepMTWPTFail_); 
  
  MuDiLepMTWActivity_ = new TH1D("MuDiLepMTWActivity","MuDiLepMTWActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuDiLepMTWActivity_);
  MuDiLepMTWActivityFail_ = (TH1D*)MuDiLepMTWActivity_->Clone();
  MuDiLepMTWActivityFail_->SetName("MuDiLepMTWActivityFail");
  //GetOutputList()->Add(MuDiLepMTWActivityFail_); 
  
  
  // el
  
  ElecPurityPT_ = new TH1D("ElecPurityPT","ElecPurityPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecPurityPT_);
  ElecPurityPTFail_ = (TH1D*)ElecPurityPT_->Clone();
  ElecPurityPTFail_->SetName("ElecPurityPTFail");
  //GetOutputList()->Add(ElecPurityPTFail_); 
  
  ElecPurityActivity_ = new TH1D("ElecPurityActivity","ElecPurityActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecPurityActivity_);
  ElecPurityActivityFail_ = (TH1D*)ElecPurityActivity_->Clone();
  ElecPurityActivityFail_->SetName("ElecPurityActivityFail");
  //GetOutputList()->Add(ElecPurityActivityFail_); 
  
  ElecAccPT_ = new TH1D("ElecAccPT","ElecAccPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecAccPT_);
  ElecAccPTFail_ = (TH1D*)ElecAccPT_->Clone();
  ElecAccPTFail_->SetName("ElecAccPTFail");
  //GetOutputList()->Add(ElecAccPTFail_); 
  
  ElecAccActivity_ = new TH1D("ElecAccActivity","ElecAccActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecAccActivity_);
  ElecAccActivityFail_ = (TH1D*)ElecAccActivity_->Clone();
  ElecAccActivityFail_->SetName("ElecAccActivityFail");
  //GetOutputList()->Add(ElecAccActivityFail_); 
  
  
  ElecRecoPT_ = new TH1D("ElecRecoPT","ElecRecoPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecRecoPT_);
  ElecRecoPTFail_ = (TH1D*)ElecRecoPT_->Clone();
  ElecRecoPTFail_->SetName("ElecRecoPTFail");
  //GetOutputList()->Add(ElecRecoPTFail_); 
  
  ElecRecoActivity_ = new TH1D("ElecRecoActivity","ElecRecoActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecRecoActivity_);
  ElecRecoActivityFail_ = (TH1D*)ElecRecoActivity_->Clone();
  ElecRecoActivityFail_->SetName("ElecRecoActivityFail");
  //GetOutputList()->Add(ElecRecoActivityFail_); 
  
  ElecIsoPT_ = new TH1D("ElecIsoPT","ElecIsoPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecIsoPT_);
  ElecIsoPTFail_ = (TH1D*)ElecIsoPT_->Clone();
  ElecIsoPTFail_->SetName("ElecIsoPTFail");
  //GetOutputList()->Add(ElecIsoPTFail_); 
  
  ElecIsoActivity_ = new TH1D("ElecIsoActivity","ElecIsoActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecIsoActivity_);
  ElecIsoActivityFail_ = (TH1D*)ElecIsoActivity_->Clone();
  ElecIsoActivityFail_->SetName("ElecIsoActivityFail");
  //GetOutputList()->Add(ElecIsoActivityFail_); 
  
  ElecMTWPT_ = new TH1D("ElecMTWPT","ElecMTWPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecMTWPT_);
  ElecMTWPTFail_ = (TH1D*)ElecMTWPT_->Clone();
  ElecMTWPTFail_->SetName("ElecMTWPTFail");
  //GetOutputList()->Add(ElecMTWPTFail_); 
  
  ElecMTWActivity_ = new TH1D("ElecMTWActivity","ElecMTWActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecMTWActivity_);
  ElecMTWActivityFail_ = (TH1D*)ElecMTWActivity_->Clone();
  ElecMTWActivityFail_->SetName("ElecMTWActivityFail");
  //GetOutputList()->Add(ElecMTWActivityFail_); 
  
  ElecDiLepPT_ = new TH1D("ElecDiLepPT","ElecDiLepPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecDiLepPT_);
  ElecDiLepPTFail_ = (TH1D*)ElecDiLepPT_->Clone();
  ElecDiLepPTFail_->SetName("ElecDiLepPTFail");
  //GetOutputList()->Add(ElecDiLepPTFail_); 
  
  ElecDiLepActivity_ = new TH1D("ElecDiLepActivity","ElecDiLepActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecDiLepActivity_);
  ElecDiLepActivityFail_ = (TH1D*)ElecDiLepActivity_->Clone();
  ElecDiLepActivityFail_->SetName("ElecDiLepActivityFail");
  //GetOutputList()->Add(ElecDiLepActivityFail_); 
  
  
  ElecDiLepMTWPT_ = new TH1D("ElecDiLepMTWPT","ElecDiLepMTWPT",oneDPT_-1,OneDPT_);
  //GetOutputList()->Add(ElecDiLepMTWPT_);
  ElecDiLepMTWPTFail_ = (TH1D*)ElecDiLepMTWPT_->Clone();
  ElecDiLepMTWPTFail_->SetName("ElecDiLepMTWPTFail");
  //GetOutputList()->Add(ElecDiLepMTWPTFail_); 
  
  ElecDiLepMTWActivity_ = new TH1D("ElecDiLepMTWActivity","ElecDiLepMTWActivity",oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecDiLepMTWActivity_);
  ElecDiLepMTWActivityFail_ = (TH1D*)ElecDiLepMTWActivity_->Clone();
  ElecDiLepMTWActivityFail_->SetName("ElecDiLepMTWActivityFail");
  //GetOutputList()->Add(ElecDiLepMTWActivityFail_); 
  
  // 2D
  MuIsoPTActivity_ = new TH2D("MuIsoPTActivity","MuIsoPTActivity",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuIsoPTActivity_);
  MuIsoPTActivityFail_ = (TH2D*)MuIsoPTActivity_->Clone();
  MuIsoPTActivityFail_->SetName("MuIsoPTActivityFail");
  //GetOutputList()->Add(MuIsoPTActivityFail_); 
  
  ElecIsoPTActivity_ = new TH2D("ElecIsoPTActivity","ElecIsoPTActivity",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecIsoPTActivity_);
  ElecIsoPTActivityFail_ = (TH2D*)ElecIsoPTActivity_->Clone();
  ElecIsoPTActivityFail_->SetName("ElecIsoPTActivityFail");
  //GetOutputList()->Add(ElecIsoPTActivityFail_); 
	
	
  MuRecoPTActivity_ = new TH2D("MuRecoPTActivity","MuRecoPTActivity",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(MuRecoPTActivity_);
  MuRecoPTActivityFail_ = (TH2D*)MuRecoPTActivity_->Clone();
  MuRecoPTActivityFail_->SetName("MuRecoPTActivityFail");
  //GetOutputList()->Add(MuRecoPTActivityFail_); 
	
  ElecRecoPTActivity_ = new TH2D("ElecRecoPTActivity","ElecRecoPTActivity",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
  //GetOutputList()->Add(ElecRecoPTActivity_);
  ElecRecoPTActivityFail_ = (TH2D*)ElecRecoPTActivity_->Clone();
  ElecRecoPTActivityFail_->SetName("ElecRecoPTActivityFail");
  //GetOutputList()->Add(ElecRecoPTActivityFail_); 
	
  // mtw 2d
  MuMTWPTActivity_ = new TH2D("MuMTWPTActivity","MuMTWPTActivity",muMTWPT2D_-1,MuMTWPT2D_,muMTWActivity2D_-1,MuMTWActivity2D_);
  //GetOutputList()->Add(MuMTWPTActivity_);
  MuMTWPTActivityFail_ = (TH2D*)MuMTWPTActivity_->Clone();
  MuMTWPTActivityFail_->SetName("MuMTWPTActivityFail");
  //GetOutputList()->Add(MuMTWPTActivityFail_);
	
  ElecMTWPTActivity_ = new TH2D("ElecMTWPTActivity","ElecMTWPTActivity",elMTWPT2D_-1,ElecMTWPT2D_,elMTWActivity2D_-1,ElecMTWActivity2D_);
  //GetOutputList()->Add(ElecMTWPTActivity_);
  ElecMTWPTActivityFail_ = (TH2D*)ElecMTWPTActivity_->Clone();
  ElecMTWPTActivityFail_->SetName("ElecMTWPTActivityFail");
  //GetOutputList()->Add(ElecMTWPTActivityFail_);
	
	
  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  // TEfficiencies used efficiencies
  MuPurityBTagEff_ = new TH1Deff("MuPurityBTag1D_Eff","MuPurityBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuPurityNJetsEff_ = new TH1Deff("MuPurityNJets1D_Eff","MuPurityNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuPurityHTEff_ = new TH1Deff("MuPurityHT1D_Eff","MuPurityHT1D_Eff",oneDHT_-1,OneDHT_);
  MuPurityMHTEff_ = new TH1Deff("MuPurityMHT1D_Eff","MuPurityMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  MuPurityPTEff_ = new TH1Deff("MuPurityPT_Eff","MuPurityPT_Eff",oneDPT_-1,OneDPT_);
  MuPurityActivityEff_ = new TH1Deff("MuPurityActivity_Eff","MuPurityActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  MuonPurityMHTNJetEff_ = new TH2Deff("MuonPurity_Eff","MuonPurity_Eff",mupurityMHT_-1,muPurityMHT_,mupurityNJet_-1,muPurityNJet_);
	
	
  ElecPurityBTagEff_ = new TH1Deff("ElecPurityBTag1D_Eff","ElecPurityBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecPurityNJetsEff_ = new TH1Deff("ElecPurityNJets1D_Eff","ElecPurityNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecPurityHTEff_ = new TH1Deff("ElecPurityHT1D_Eff","ElecPurityHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecPurityMHTEff_ = new TH1Deff("ElecPurityMHT1D_Eff","ElecPurityMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  ElecPurityPTEff_ = new TH1Deff("ElecPurityPT_Eff","ElecPurityPT_Eff",oneDPT_-1,OneDPT_);
  ElecPurityActivityEff_ = new TH1Deff("ElecPurityActivity_Eff","ElecPurityActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  ElecPurityMHTNJetEff_ = new TH2Deff("ElecPurity_Eff","ElecPurity_Eff",mupurityMHT_-1,muPurityMHT_,mupurityNJet_-1,muPurityNJet_);
	
  MuAccBTagEff_ = new TH1Deff("MuAccBTag1D_Eff","MuAccBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuAccNJetsEff_ = new TH1Deff("MuAccNJets1D_Eff","MuAccNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuAccHTEff_ = new TH1Deff("MuAccHT1D_Eff","MuAccHT1D_Eff",oneDHT_-1,OneDHT_);
  MuAccMHTEff_ = new TH1Deff("MuAccMHT1D_Eff","MuAccMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  MuAccPTEff_ = new TH1Deff("MuAccPT_Eff","MuAccPT_Eff",oneDPT_-1,OneDPT_);
  MuAccActivityEff_ = new TH1Deff("MuAccActivity_Eff","MuAccActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  MuAccHTNJetsEff_ = new TH2Deff("MuAccHTNJets_Eff","MuAccHTNJets_Eff",muaccHT_-1,muAccHT_, muaccNJets_-1, muAccNJets_);
  MuAccMHTNJetsEff_ = new TH2Deff("MuAccMHTNJets_Eff","MuAccMHTNJets_Eff",muaccMHT_-1,muAccMHT_, muaccNJets_-1, muAccNJets_);
  MuAccBTagNJetsEff_ = new TH2Deff("MuAccBTagNJets_Eff","MuAccBTagNJets_Eff",muaccBTags_-1,muAccBTags_, muaccNJets_-1, muAccNJets_);

	
  ElecAccBTagEff_ = new TH1Deff("ElecAccBTag1D_Eff","ElecAccBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecAccNJetsEff_ = new TH1Deff("ElecAccNJets1D_Eff","ElecAccNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecAccHTEff_ = new TH1Deff("ElecAccHT1D_Eff","ElecAccHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecAccMHTEff_ = new TH1Deff("ElecAccMHT1D_Eff","ElecAccMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  ElecAccPTEff_ = new TH1Deff("ElecAccPT_Eff","ElecAccPT_Eff",oneDPT_-1,OneDPT_);
  ElecAccActivityEff_ = new TH1Deff("ElecAccActivity_Eff","ElecAccActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  ElecAccHTNJetsEff_ = new TH2Deff("ElecAccHTNJets_Eff","ElecAccHTNJets_Eff",elaccHT_-1,elecAccHT_, elaccNJets_-1, elecAccNJets_);
  ElecAccMHTNJetsEff_ = new TH2Deff("ElecAccMHTNJets_Eff","ElecAccMHTNJets_Eff",elaccMHT_-1,elecAccMHT_, elaccNJets_-1, elecAccNJets_);
  ElecAccBTagNJetsEff_ = new TH2Deff("ElecAccBTagNJets_Eff","ElecAccBTagNJets_Eff",elaccBTags_-1,elecAccBTags_, elaccNJets_-1, elecAccNJets_);
	
	
  MuRecoBTagEff_ = new TH1Deff("MuRecoBTag1D_Eff","MuRecoBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuRecoNJetsEff_ = new TH1Deff("MuRecoNJets1D_Eff","MuRecoNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuRecoHTEff_ = new TH1Deff("MuRecoHT1D_Eff","MuRecoHT1D_Eff",oneDHT_-1,OneDHT_);
  MuRecoMHTEff_ = new TH1Deff("MuRecoMHT1D_Eff","MuRecoMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  MuRecoPTEff_ = new TH1Deff("MuRecoPT_Eff","MuRecoPT_Eff",oneDPT_-1,OneDPT_);
  MuRecoActivityEff_ = new TH1Deff("MuRecoActivity_Eff","MuRecoActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  MuRecoPTActivityEff_ = new TH2Deff("MuRecoPTActivity_Eff","MuRecoPTActivity_Eff",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);	
	
	
  ElecRecoBTagEff_ = new TH1Deff("ElecRecoBTag1D_Eff","ElecRecoBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecRecoNJetsEff_ = new TH1Deff("ElecRecoNJets1D_Eff","ElecRecoNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecRecoHTEff_ = new TH1Deff("ElecRecoHT1D_Eff","ElecRecoHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecRecoMHTEff_ = new TH1Deff("ElecRecoMHT1D_Eff","ElecRecoMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  ElecRecoPTEff_ = new TH1Deff("ElecRecoPT_Eff","ElecRecoPT_Eff",oneDPT_-1,OneDPT_);
  ElecRecoActivityEff_ = new TH1Deff("ElecRecoActivity_Eff","ElecRecoActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  ElecRecoPTActivityEff_ = new TH2Deff("ElecRecoPTActivity_Eff","ElecRecoPTActivity_Eff",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
	
	
  MuIsoBTagEff_ = new TH1Deff("MuIsoBTag1D_Eff","MuIsoBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuIsoNJetsEff_ = new TH1Deff("MuIsoNJets1D_Eff","MuIsoNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuIsoHTEff_ = new TH1Deff("MuIsoHT1D_Eff","MuIsoHT1D_Eff",oneDHT_-1,OneDHT_);
  MuIsoMHTEff_ = new TH1Deff("MuIsoMHT1D_Eff","MuIsoMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  MuIsoPTEff_ = new TH1Deff("MuIsoPT_Eff","MuIsoPT_Eff",oneDPT_-1,OneDPT_);
  MuIsoActivityEff_ = new TH1Deff("MuIsoActivity_Eff","MuIsoActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  MuIsoPTActivityEff_ = new TH2Deff("MuIsoPTActivity_Eff","MuIsoPTActivity_Eff",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
	
	
  ElecIsoBTagEff_ = new TH1Deff("ElecIsoBTag1D_Eff","ElecIsoBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecIsoNJetsEff_ = new TH1Deff("ElecIsoNJets1D_Eff","ElecIsoNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecIsoHTEff_ = new TH1Deff("ElecIsoHT1D_Eff","ElecIsoHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecIsoMHTEff_ = new TH1Deff("ElecIsoMHT1D_Eff","ElecIsoMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  ElecIsoPTEff_ = new TH1Deff("ElecIsoPT_Eff","ElecIsoPT_Eff",oneDPT_-1,OneDPT_);
  ElecIsoActivityEff_ = new TH1Deff("ElecIsoActivity_Eff","ElecIsoActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  ElecIsoPTActivityEff_ = new TH2Deff("ElecIsoPTActivity_Eff","ElecIsoPTActivity_Eff",oneDPT_-1,OneDPT_,oneDActivity_-1,OneDActivity_);
	
	
  MuMTWBTagEff_ = new TH1Deff("MuMTWBTag1D_Eff","MuMTWBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuMTWNJetsEff_ = new TH1Deff("MuMTWNJets1D_Eff","MuMTWNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuMTWHTEff_ = new TH1Deff("MuMTWHT1D_Eff","MuMTWHT1D_Eff",oneDHT_-1,OneDHT_);
  MuMTWMHTEff_ = new TH1Deff("MuMTWMHT1D_Eff","MuMTWMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  MuMTWPTEff_ = new TH1Deff("MuMTWPT_Eff","MuMTWPT_Eff",oneDPT_-1,OneDPT_);
  MuMTWActivityEff_ = new TH1Deff("MuMTWActivity_Eff","MuMTWActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  MuMTWPTActivityEff_ = new TH2Deff("MuMTWPTActivity_Eff","MuMTWPTActivity_Eff",muMTWPT2D_-1,MuMTWPT2D_,muMTWActivity2D_-1,MuMTWActivity2D_);
	
	
	
  ElecMTWBTagEff_ = new TH1Deff("ElecMTWBTag1D_Eff","ElecMTWBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecMTWNJetsEff_ = new TH1Deff("ElecMTWNJets1D_Eff","ElecMTWNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecMTWHTEff_ = new TH1Deff("ElecMTWHT1D_Eff","ElecMTWHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecMTWMHTEff_ = new TH1Deff("ElecMTWMHT1D_Eff","ElecMTWMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  ElecMTWPTEff_ = new TH1Deff("ElecMTWPT_Eff","ElecMTWPT_Eff",oneDPT_-1,OneDPT_);
  ElecMTWActivityEff_ = new TH1Deff("ElecMTWActivity_Eff","ElecMTWActivity_Eff",oneDActivity_-1,OneDActivity_);
	
  ElecMTWPTActivityEff_ = new TH2Deff("ElecMTWPTActivity_Eff","ElecMTWPTActivity_Eff",elMTWPT2D_-1,ElecMTWPT2D_,elMTWActivity2D_-1,ElecMTWActivity2D_);
	
	
	
  MuDiLepBTagEff_ = new TH1Deff("MuDiLepBTag1D_Eff","MuDiLepBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuDiLepNJetsEff_ = new TH1Deff("MuDiLepNJets1D_Eff","MuDiLepNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuDiLepHTEff_ = new TH1Deff("MuDiLepHT1D_Eff","MuDiLepHT1D_Eff",oneDHT_-1,OneDHT_);
  MuDiLepMHTEff_ = new TH1Deff("MuDiLepMHT1D_Eff","MuDiLepMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  // 	MuDiLepPTEff_ = new TH1Deff("MuDiLepPT_Eff","MuDiLepPT_Eff",oneDPT_-1,OneDPT_);
  // 	MuDiLepActivityEff_ = new TH1Deff("MuDiLepActivity_Eff","MuDiLepActivity_Eff",oneDActivity_-1,OneDActivity_);
	
	
  ElecDiLepBTagEff_ = new TH1Deff("ElecDiLepBTag1D_Eff","ElecDiLepBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecDiLepNJetsEff_ = new TH1Deff("ElecDiLepNJets1D_Eff","ElecDiLepNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecDiLepHTEff_ = new TH1Deff("ElecDiLepHT1D_Eff","ElecDiLepHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecDiLepMHTEff_ = new TH1Deff("ElecDiLepMHT1D_Eff","ElecDiLepMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  // 	ElecDiLepPTEff_ = new TH1Deff("ElecDiLepPT_Eff","ElecDiLepPT_Eff",oneDPT_-1,OneDPT_);
  // 	ElecDiLepActivityEff_ = new TH1Deff("ElecDiLepActivity_Eff","ElecDiLepActivity_Eff",oneDActivity_-1,OneDActivity_);
	
	
  MuDiLepMTWBTagEff_ = new TH1Deff("MuDiLepMTWBTag1D_Eff","MuDiLepMTWBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuDiLepMTWNJetsEff_ = new TH1Deff("MuDiLepMTWNJets1D_Eff","MuDiLepMTWNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuDiLepMTWHTEff_ = new TH1Deff("MuDiLepMTWHT1D_Eff","MuDiLepMTWHT1D_Eff",oneDHT_-1,OneDHT_);
  MuDiLepMTWMHTEff_ = new TH1Deff("MuDiLepMTWMHT1D_Eff","MuDiLepMTWMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  // 	MuDiLepMTWPTEff_ = new TH1Deff("MuDiLepMTWPT_Eff","MuDiLepMTWPT_Eff",oneDPT_-1,OneDPT_);
  // 	MuDiLepMTWActivityEff_ = new TH1Deff("MuDiLepMTWActivity_Eff","MuDiLepMTWActivity_Eff",oneDActivity_-1,OneDActivity_);
	
	
  ElecDiLepMTWBTagEff_ = new TH1Deff("ElecDiLepMTWBTag1D_Eff","ElecDiLepMTWBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecDiLepMTWNJetsEff_ = new TH1Deff("ElecDiLepMTWNJets1D_Eff","ElecDiLepMTWNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecDiLepMTWHTEff_ = new TH1Deff("ElecDiLepMTWHT1D_Eff","ElecDiLepMTWHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecDiLepMTWMHTEff_ = new TH1Deff("ElecDiLepMTWMHT1D_Eff","ElecDiLepMTWMHT1D_Eff",oneDMHT_-1,OneDMHT_);
  // 	ElecDiLepMTWPTEff_ = new TH1Deff("ElecDiLepMTWPT_Eff","ElecDiLepMTWPT_Eff",oneDPT_-1,OneDPT_);
  // 	ElecDiLepMTWActivityEff_ = new TH1Deff("ElecDiLepMTWActivity_Eff","ElecDiLepMTWActivity_Eff",oneDActivity_-1,OneDActivity_);
	
	
  MuDiLepContributionBTagEff_ = new TH1Deff("MuDiLepContributionBTag1D_Eff","MuDiLepContributionBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuDiLepContributionNJetsEff_ = new TH1Deff("MuDiLepContributionNJets1D_Eff","MuDiLepContributionNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuDiLepContributionHTEff_ = new TH1Deff("MuDiLepContributionHT1D_Eff","MuDiLepContributionHT1D_Eff",oneDHT_-1,OneDHT_);
  MuDiLepContributionMHTEff_ = new TH1Deff("MuDiLepContributionMHT1D_Eff","MuDiLepContributionMHT1D_Eff",oneDMHT_-1,OneDMHT_);
	
	
  ElecDiLepContributionBTagEff_ = new TH1Deff("ElecDiLepContributionBTag1D_Eff","ElecDiLepContributionBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecDiLepContributionNJetsEff_ = new TH1Deff("ElecDiLepContributionNJets1D_Eff","ElecDiLepContributionNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecDiLepContributionHTEff_ = new TH1Deff("ElecDiLepContributionHT1D_Eff","ElecDiLepContributionHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecDiLepContributionMHTEff_ = new TH1Deff("ElecDiLepContributionMHT1D_Eff","ElecDiLepContributionMHT1D_Eff",oneDMHT_-1,OneDMHT_);
	
	
  MuDiLepContributionMTWBTagEff_ = new TH1Deff("MuDiLepContributionMTWBTag1D_Eff","MuDiLepContributionMTWBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  MuDiLepContributionMTWNJetsEff_ = new TH1Deff("MuDiLepContributionMTWNJets1D_Eff","MuDiLepContributionMTWNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  MuDiLepContributionMTWHTEff_ = new TH1Deff("MuDiLepContributionMTWHT1D_Eff","MuDiLepContributionMTWHT1D_Eff",oneDHT_-1,OneDHT_);
  MuDiLepContributionMTWMHTEff_ = new TH1Deff("MuDiLepContributionMTWMHT1D_Eff","MuDiLepContributionMTWMHT1D_Eff",oneDMHT_-1,OneDMHT_);
	
	
  ElecDiLepContributionMTWBTagEff_ = new TH1Deff("ElecDiLepContributionMTWBTag1D_Eff","ElecDiLepContributionMTWBTag1D_Eff",oneDBJets_-1,OneDBJets_);
  ElecDiLepContributionMTWNJetsEff_ = new TH1Deff("ElecDiLepContributionMTWNJets1D_Eff","ElecDiLepContributionMTWNJets1D_Eff",oneDNJets_-1,OneDNJets_);
  ElecDiLepContributionMTWHTEff_ = new TH1Deff("ElecDiLepContributionMTWHT1D_Eff","ElecDiLepContributionMTWHT1D_Eff",oneDHT_-1,OneDHT_);
  ElecDiLepContributionMTWMHTEff_ = new TH1Deff("ElecDiLepContributionMTWMHT1D_Eff","ElecDiLepContributionMTWMHT1D_Eff",oneDMHT_-1,OneDMHT_);
	
  // search bin efficiencies
  MuAccSearchBinEff_ = new Efficiency("MuAccSearchBinEff","MuAccSearchBinEff");
  MuRecoSearchBinEff_ = new Efficiency("MuRecoSearchBinEff","MuRecoSearchBinEff");
  MuIsoSearchBinEff_ = new Efficiency("MuIsoSearchBinEff","MuIsoSearchBinEff");
  MuMTWSearchBinEff_ = new Efficiency("MuMTWSearchBinEff","MuMTWSearchBinEff");
  MuDiLepContributionMTWAppliedSearchBinEff_ = new Efficiency("MuDiLepContributionMTWAppliedSearchBinEff","MuDiLepContributionMTWAppliedSearchBinEff");
  MuDiLepEffMTWAppliedSearchBinEff_ = new Efficiency("MuDiLepEffMTWAppliedSearchBinEff","MuDiLepEffMTWAppliedSearchBinEff");
  MuPuritySearchBinEff_ = new Efficiency("MuPuritySearchBinEff","MuPuritySearchBinEff");
	
  ElecAccSearchBinEff_ = new Efficiency("ElecAccSearchBinEff","ElecAccSearchBinEff");
  ElecRecoSearchBinEff_ = new Efficiency("ElecRecoSearchBinEff","ElecRecoSearchBinEff");
  ElecIsoSearchBinEff_ = new Efficiency("ElecIsoSearchBinEff","ElecIsoSearchBinEff");
  ElecMTWSearchBinEff_ = new Efficiency("ElecMTWSearchBinEff","ElecMTWSearchBinEff");
  ElecDiLepContributionMTWAppliedSearchBinEff_ = new Efficiency("ElecDiLepContributionMTWAppliedSearchBinEff","ElecDiLepContributionMTWAppliedSearchBinEff");
  ElecDiLepEffMTWAppliedSearchBinEff_ = new Efficiency("ElecDiLepEffMTWAppliedSearchBinEff","ElecDiLepEffMTWAppliedSearchBinEff");
  ElecPuritySearchBinEff_ = new Efficiency("ElecPuritySearchBinEff","ElecPuritySearchBinEff");
	
}

Bool_t RTree::Process(Long64_t entry)
{

  fChain->GetTree()->GetEntry(entry);
  /*  double HT=ht30, MHT=mht30;
  int NJets=num_jets_pt30, BTags=num_csvm_jets30;
  double minDeltaPhiN = min_delta_phi_met_N;
  double Weight=weightppb*10000;
  */
    if(NJets<4) return kTRUE;
    if(BTags<2) return kTRUE;
    if(HT<500) return kTRUE;
    if(MHT<200) return kTRUE;
    if(minDeltaPhiN<6) return kTRUE;
    //if (!passesBadJetFilter || PBNRcode<1) return kTRUE;
  // for tests, only looking at 2+ b-jet events
  // int selectedIDIsoMuonsNum=num_veto_mus, selectedIDIsoElectronsNum=num_veto_els;
  
  // total expectation
  if(Expectation==1)totalExpectation_->Fill(HT,MHT,NJets,BTags,Weight);
	
  // purity
  // single muon control sample
  // x-check
  if(MuPurity==2)
    {
      //1D
      MuPurityCheckBTag_->Fill(BTags,Weight);
      MuPurityCheckNJets_->Fill(NJets,Weight);
      MuPurityCheckHT_->Fill(HT,Weight);
      MuPurityCheckMHT_->Fill(MHT,Weight);
      //2D
      MuonPurityCheckMHTNJet->Fill(MHT,NJets,Weight);
    }
  if(MuPurity==0)
    {
      //1D
      MuPurityCheckBTagFail_->Fill(BTags,Weight);
      MuPurityCheckNJetsFail_->Fill(NJets,Weight);
      MuPurityCheckHTFail_->Fill(HT,Weight);
      MuPurityCheckMHTFail_->Fill(MHT,Weight);
      //2D
      MuonPurityCheckMHTNJetFail->Fill(MHT,NJets,Weight);
    }
  if(ElecPurity==2)
    {
      //1D
      ElecPurityCheckBTag_->Fill(BTags,Weight);
      ElecPurityCheckNJets_->Fill(NJets,Weight);
      ElecPurityCheckHT_->Fill(HT,Weight);
      ElecPurityCheckMHT_->Fill(MHT,Weight);
      //2D
      ElecPurityCheckMHTNJet->Fill(MHT,NJets,Weight);
    }
  if(ElecPurity==0)
    {
      //1D
      ElecPurityCheckBTagFail_->Fill(BTags,Weight);
      ElecPurityCheckNJetsFail_->Fill(NJets,Weight);
      ElecPurityCheckHTFail_->Fill(HT,Weight);
      ElecPurityCheckMHTFail_->Fill(MHT,Weight);
      //2D
      ElecPurityCheckMHTNJetFail->Fill(MHT,NJets,Weight);
    }
  if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
    {
      // cout << "Single muon control sample" << endl;
      if(mu_tm->at(0)==1)
	{
	  //1D
	  MuPurityBTag_->Fill(BTags,Weight);
	  MuPurityNJets_->Fill(NJets,Weight);
	  MuPurityHT_->Fill(HT,Weight);
	  MuPurityMHT_->Fill(MHT,Weight);
	  MuPurityPT_->Fill(mu_pt->at(0),Weight);
	  //	  MuPurityActivity_->Fill(mu_activity->at(0),Weight);
	  MuPurityActivity_->Fill(mu_activity_mu->at(0),Weight);
	  //2D
	  MuonPurityMHTNJet->Fill(MHT,NJets,Weight);
	}
      if(mu_tm->at(0)==0)
	{
	  //1D
	  MuPurityBTagFail_->Fill(BTags,Weight);
	  MuPurityBTagEff_->Fill(BTags,Weight,false);
	  MuPurityNJetsFail_->Fill(NJets,Weight);
	  MuPurityHTFail_->Fill(HT,Weight);
	  MuPurityMHTFail_->Fill(MHT,Weight);
	  MuPurityPTFail_->Fill(mu_pt->at(0),Weight);
	  //	  MuPurityActivityFail_->Fill(mu_activity->at(0),Weight);
	  MuPurityActivityFail_->Fill(mu_activity_mu->at(0),Weight);
	  //2D
	  MuonPurityMHTNJetFail->Fill(MHT,NJets,Weight);
	  MuonPurityMHTNJetEff_->Fill(MHT,NJets,Weight,false);
	}
    }
  // single el control sample
  if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
    {
      // cout << "Single electron control sample" << endl;
      if(el_tm->at(0)==1)
	{
	  //1D
	  ElecPurityBTag_->Fill(BTags,Weight);
	  ElecPurityNJets_->Fill(NJets,Weight);
	  ElecPurityHT_->Fill(HT,Weight);
	  ElecPurityMHT_->Fill(MHT,Weight);
	  ElecPurityPT_->Fill(el_pt->at(0),Weight);
	  ElecPurityActivity_->Fill(el_activity_el->at(0),Weight);
	  //2D
	  ElecPurityMHTNJet->Fill(MHT,NJets,Weight);
	}
      if(el_tm->at(0)==0)
	{
	  //1D
	  ElecPurityBTagFail_->Fill(BTags,Weight);
	  ElecPurityNJetsFail_->Fill(NJets,Weight);
	  ElecPurityHTFail_->Fill(HT,Weight);
	  ElecPurityMHTFail_->Fill(MHT,Weight);
	  ElecPurityPTFail_->Fill(el_pt->at(0),Weight);
	  ElecPurityActivityFail_->Fill(el_activity_el->at(0),Weight);
	  //2D
	  ElecPurityMHTNJetFail->Fill(MHT,NJets,Weight);
	}
    }
  // acceptance
  // single muon control sample
  if(muAcc==2)
    {
      // cout << "Muons in acceptance" << endl;
      //1D
      MuAccBTag_->Fill(BTags,Weight);
      MuAccNJets_->Fill(NJets,Weight);
      MuAccHT_->Fill(HT,Weight);
      MuAccMHT_->Fill(MHT,Weight);
      MuAccPT_->Fill(true_mu_pt->at(0),Weight);
      MuAccActivity_->Fill(true_mu_activity_mu->at(0),Weight);
      //2D
      MuAccHTNJets_->Fill(HT,NJets,Weight);
      MuAccMHTNJets_->Fill(MHT,NJets,Weight);
      MuAccBTagNJets_->Fill(BTags,NJets,Weight);
      MuAccMHTNJetsEff_->Fill(MHT,NJets,Weight,true);
      MuAccBTagNJetsEff_->Fill(BTags,NJets,Weight,true);
      if(BTags==0)
	{
	  MuAccMHTNJetsB0_->Fill(MHT,NJets,Weight);
	}
      else
	{
	  MuAccMHTNJetsB1_Inf_->Fill(MHT,NJets,Weight);
	}
    }
  if(muAcc==0)
    {
      // cout << "Muons out of acceptance" << endl;
      //1D
      MuAccBTagFail_->Fill(BTags,Weight);
      MuAccNJetsFail_->Fill(NJets,Weight);
      MuAccHTFail_->Fill(HT,Weight);
      MuAccMHTFail_->Fill(MHT,Weight);
      MuAccPTFail_->Fill(true_mu_pt->at(0),Weight);
      MuAccActivityFail_->Fill(true_mu_activity_mu->at(0),Weight);
      //2D
      MuAccHTNJetsFail_->Fill(HT,NJets,Weight);
      MuAccMHTNJetsFail_->Fill(MHT,NJets,Weight);
      MuAccBTagNJetsFail_->Fill(BTags,NJets,Weight);
      MuAccMHTNJetsEff_->Fill(MHT,NJets,Weight,false);
      MuAccBTagNJetsEff_->Fill(BTags,NJets,Weight,false);
      if(BTags==0)
	{
	  MuAccMHTNJetsB0Fail_->Fill(MHT,NJets,Weight);
	}
      else
	{
	  MuAccMHTNJetsB1_InfFail_->Fill(MHT,NJets,Weight);
	}
    }
  
  // single elon control sample
  if(elecAcc==2)
    {
      // cout << "Electrons in acceptance" << endl;
    //1D
      ElecAccBTag_->Fill(BTags,Weight);
      ElecAccNJets_->Fill(NJets,Weight);
      ElecAccHT_->Fill(HT,Weight);
      ElecAccMHT_->Fill(MHT,Weight);
      ElecAccPT_->Fill(true_el_pt->at(0),Weight);
      ElecAccActivity_->Fill(true_el_activity_el->at(0),Weight);
      //2D
      ElecAccHTNJets_->Fill(HT,NJets,Weight);
      ElecAccMHTNJets_->Fill(MHT,NJets,Weight);
      ElecAccBTagNJets_->Fill(BTags,NJets,Weight);
      if(BTags==0)
	{
	  ElecAccMHTNJetsB0_->Fill(MHT,NJets,Weight);
	}
      else
	{
	  ElecAccMHTNJetsB1_Inf_->Fill(MHT,NJets,Weight);
	}
    }
  if(elecAcc==0)
    {
      // cout << "Electrons out of acceptance" << endl;
     //1D
      ElecAccBTagFail_->Fill(BTags,Weight);
      ElecAccNJetsFail_->Fill(NJets,Weight);
      ElecAccHTFail_->Fill(HT,Weight);
      ElecAccMHTFail_->Fill(MHT,Weight);
      ElecAccPTFail_->Fill(true_el_pt->at(0),Weight);
      ElecAccActivityFail_->Fill(true_el_activity_el->at(0),Weight);
      //2D
      ElecAccHTNJetsFail_->Fill(HT,NJets,Weight);
      ElecAccMHTNJetsFail_->Fill(MHT,NJets,Weight);
      ElecAccBTagNJetsFail_->Fill(BTags,NJets,Weight);
      if(BTags==0)
	{
	  ElecAccMHTNJetsB0Fail_->Fill(MHT,NJets,Weight);
	}
      else
	{
	  ElecAccMHTNJetsB1_InfFail_->Fill(MHT,NJets,Weight);
	}
    }
  
  // reconstruction
  // single muon control sample
  if(muReco==2)
    {
      // cout << "Reco'd muons" << endl;
      //1D
      MuRecoBTag_->Fill(BTags,Weight);
      MuRecoNJets_->Fill(NJets,Weight);
      MuRecoHT_->Fill(HT,Weight);
      MuRecoMHT_->Fill(MHT,Weight);
      MuRecoPT_->Fill(true_mu_pt->at(0),Weight);
      MuRecoActivity_->Fill(true_mu_activity_mu->at(0),Weight);
      //2D
      MuRecoPTActivity_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight);
    }
  if(muReco==0)
    {
       // cout << "Non-reco'd muons" << endl;
     //1D
      MuRecoBTagFail_->Fill(BTags,Weight);
      MuRecoNJetsFail_->Fill(NJets,Weight);
      MuRecoHTFail_->Fill(HT,Weight);
      MuRecoMHTFail_->Fill(MHT,Weight);
      MuRecoPTFail_->Fill(true_mu_pt->at(0),Weight);
      MuRecoActivityFail_->Fill(true_mu_activity_mu->at(0),Weight);
      //2D
      MuRecoPTActivityFail_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight);
    }
  
  // single elon control sample
  if(elecReco==2)
    {
      // cout << "Reco'd electrons" << endl;
     //1D
      ElecRecoBTag_->Fill(BTags,Weight);
      ElecRecoNJets_->Fill(NJets,Weight);
      ElecRecoHT_->Fill(HT,Weight);
      ElecRecoMHT_->Fill(MHT,Weight);
      ElecRecoPT_->Fill(true_el_pt->at(0),Weight);
      ElecRecoActivity_->Fill(true_el_activity_el->at(0),Weight);
      //2D
      ElecRecoPTActivity_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight);
    }
  if(elecReco==0)
    {
       // cout << "Non-reco'd electrons" << endl;
     //1D
      ElecRecoBTagFail_->Fill(BTags,Weight);
      ElecRecoNJetsFail_->Fill(NJets,Weight);
      ElecRecoHTFail_->Fill(HT,Weight);
      ElecRecoMHTFail_->Fill(MHT,Weight);
      ElecRecoPTFail_->Fill(true_el_pt->at(0),Weight);
      ElecRecoActivityFail_->Fill(true_el_activity_el->at(0),Weight);
      //2D
      ElecRecoPTActivityFail_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight);
    }
  
  // isolation
  // single muon control sample
  if(muIso==2)
    {
        // cout << "Isolated muons" << endl;
     //1D
      MuIsoBTag_->Fill(BTags,Weight);
      MuIsoNJets_->Fill(NJets,Weight);
      MuIsoHT_->Fill(HT,Weight);
      MuIsoMHT_->Fill(MHT,Weight);
      MuIsoPT_->Fill(true_mu_pt->at(0),Weight);
      MuIsoActivity_->Fill(true_mu_activity_mu->at(0),Weight);
      //2D
      MuIsoPTActivity_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight);
    }
  if(muIso==0)
    {
       // cout << "Non-isolated muons" << endl;
      //1D
      MuIsoBTagFail_->Fill(BTags,Weight);
      MuIsoNJetsFail_->Fill(NJets,Weight);
      MuIsoHTFail_->Fill(HT,Weight);
      MuIsoMHTFail_->Fill(MHT,Weight);
      MuIsoPTFail_->Fill(true_mu_pt->at(0),Weight);
      MuIsoActivityFail_->Fill(true_mu_activity_mu->at(0),Weight);
      //2D
      MuIsoPTActivityFail_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight);
    }
  
  // single elon control sample
  if(elecIso==2)
    {
        // cout << "Isolated electrons" << endl;
     //1D
      ElecIsoBTag_->Fill(BTags,Weight);
      ElecIsoNJets_->Fill(NJets,Weight);
      ElecIsoHT_->Fill(HT,Weight);
      ElecIsoMHT_->Fill(MHT,Weight);
      ElecIsoPT_->Fill(true_el_pt->at(0),Weight);
      ElecIsoActivity_->Fill(true_el_activity_el->at(0),Weight);
      //2D
      ElecIsoPTActivity_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight);
    }
  if(elecIso==0)
    {
       // cout << "Non-isolated electrons" << endl;
      //1D
      ElecIsoBTagFail_->Fill(BTags,Weight);
      ElecIsoNJetsFail_->Fill(NJets,Weight);
      ElecIsoHTFail_->Fill(HT,Weight);
      ElecIsoMHTFail_->Fill(MHT,Weight);
      ElecIsoPTFail_->Fill(true_el_pt->at(0),Weight);
      ElecIsoActivityFail_->Fill(true_el_activity_el->at(0),Weight);
      //2D
      ElecIsoPTActivityFail_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight);
    }
  // mtw
  // single muon control sample
  if(muIso==2 && MTW < mtwCut_ && MTW>0)
    {
      // 1D
      MuMTWBTag_->Fill(BTags,Weight);
      MuMTWNJets_->Fill(NJets,Weight);
      MuMTWHT_->Fill(HT,Weight);
      MuMTWMHT_->Fill(MHT,Weight);
      //cout << "Reco mu pt" << endl;
      MuMTWPT_->Fill(true_mu_pt->at(0),Weight);
      //cout << "Reco mu activity" << endl;
      MuMTWActivity_->Fill(true_mu_activity_mu->at(0),Weight);
      MuMTWPTActivity_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight );
    }
  if(muIso==2 && MTW > mtwCut_)
    {
      // 1D
      MuMTWBTagFail_->Fill(BTags,Weight);
      MuMTWNJetsFail_->Fill(NJets,Weight);
      MuMTWHTFail_->Fill(HT,Weight);
      MuMTWMHTFail_->Fill(MHT,Weight);
      MuMTWPTFail_->Fill(true_mu_pt->at(0),Weight);
      MuMTWActivityFail_->Fill(true_mu_activity_mu->at(0),Weight);
      MuMTWPTActivityFail_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight );
    }
  
  // single el control sample
  if(elecIso==2 && MTW < mtwCut_ && MTW>0)
    {
      // 1D
      ElecMTWBTag_->Fill(BTags,Weight);
      ElecMTWNJets_->Fill(NJets,Weight);
      ElecMTWHT_->Fill(HT,Weight);
      ElecMTWMHT_->Fill(MHT,Weight);
      ElecMTWPT_->Fill(true_el_pt->at(0),Weight);
      ElecMTWActivity_->Fill(true_el_activity_el->at(0),Weight);
      ElecMTWPTActivity_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight );
    }
  if(elecIso==2 && MTW > mtwCut_)
    {
      // 1D
      ElecMTWBTagFail_->Fill(BTags,Weight);
      ElecMTWNJetsFail_->Fill(NJets,Weight);
      ElecMTWHTFail_->Fill(HT,Weight);
      ElecMTWMHTFail_->Fill(MHT,Weight);
      ElecMTWPTFail_->Fill(true_el_pt->at(0),Weight);
      ElecMTWActivityFail_->Fill(true_el_activity_el->at(0),Weight);
      ElecMTWPTActivityFail_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight );
    }
  // di lep contribution
  if(MuDiLepControlSample==2)
    {
      // 1D
      MuDiLepBTag_->Fill(BTags,Weight);
      MuDiLepNJets_->Fill(NJets,Weight);
      MuDiLepHT_->Fill(HT,Weight);
      MuDiLepMHT_->Fill(MHT,Weight);
      if(MTW <mtwCut_)
	{
	  // 1D
	  MuDiLepMTWBTag_->Fill(BTags,Weight);
	  MuDiLepMTWNJets_->Fill(NJets,Weight);
	  MuDiLepMTWHT_->Fill(HT,Weight);
	  MuDiLepMTWMHT_->Fill(MHT,Weight);
	}
      // 1D
      MuDiLepContributionBTag_->Fill(BTags,Weight);
      MuDiLepContributionNJets_->Fill(NJets,Weight);
      MuDiLepContributionHT_->Fill(HT,Weight);
      MuDiLepContributionMHT_->Fill(MHT,Weight);
      if(MTW <mtwCut_)
	{
	  // 1D
	  MuDiLepContributionMTWBTag_->Fill(BTags,Weight);
	  MuDiLepContributionMTWNJets_->Fill(NJets,Weight);
	  MuDiLepContributionMTWHT_->Fill(HT,Weight);
	  MuDiLepContributionMTWMHT_->Fill(MHT,Weight);
	}
    
    }
  if(MuDiLepControlSample==0)
    {

      // 1D
      MuDiLepContributionBTagFail_->Fill(BTags,Weight);
      MuDiLepContributionNJetsFail_->Fill(NJets,Weight);
      MuDiLepContributionHTFail_->Fill(HT,Weight);
      MuDiLepContributionMHTFail_->Fill(MHT,Weight);
      if(MTW <mtwCut_)
	{
	  // 1D
	  MuDiLepContributionMTWBTagFail_->Fill(BTags,Weight);
	  MuDiLepContributionMTWNJetsFail_->Fill(NJets,Weight);
	  MuDiLepContributionMTWHTFail_->Fill(HT,Weight);
	  MuDiLepContributionMTWMHTFail_->Fill(MHT,Weight);
	}
    }
  if(MuDiLepControlSample==0)
    {
      // 1D
      MuDiLepBTagFail_->Fill(BTags,Weight);
      MuDiLepNJetsFail_->Fill(NJets,Weight);
      MuDiLepHTFail_->Fill(HT,Weight);
      MuDiLepMHTFail_->Fill(MHT,Weight);
    
      // 1D
      MuDiLepMTWBTagFail_->Fill(BTags,Weight);
      MuDiLepMTWNJetsFail_->Fill(NJets,Weight);
      MuDiLepMTWHTFail_->Fill(HT,Weight);
      MuDiLepMTWMHTFail_->Fill(MHT,Weight);
    }
  
  if(ElecDiLepControlSample==2)
    {
      // 1D
      ElecDiLepBTag_->Fill(BTags,Weight);
      ElecDiLepNJets_->Fill(NJets,Weight);
      ElecDiLepHT_->Fill(HT,Weight);
      ElecDiLepMHT_->Fill(MHT,Weight);
      if(MTW <mtwCut_)
	{
	  // 1D
	  ElecDiLepMTWBTag_->Fill(BTags,Weight);
	  ElecDiLepMTWNJets_->Fill(NJets,Weight);
	  ElecDiLepMTWHT_->Fill(HT,Weight);
	  ElecDiLepMTWMHT_->Fill(MHT,Weight);
	}
      // 1D
      ElecDiLepContributionBTag_->Fill(BTags,Weight);
      ElecDiLepContributionNJets_->Fill(NJets,Weight);
      ElecDiLepContributionHT_->Fill(HT,Weight);
      ElecDiLepContributionMHT_->Fill(MHT,Weight);
      if(MTW <mtwCut_)
	{
	  // 1D
	  ElecDiLepContributionMTWBTag_->Fill(BTags,Weight);
	  ElecDiLepContributionMTWNJets_->Fill(NJets,Weight);
	  ElecDiLepContributionMTWHT_->Fill(HT,Weight);
	  ElecDiLepContributionMTWMHT_->Fill(MHT,Weight);
	}
    
    }
  if(ElecDiLepControlSample==0)
    {
    
      // 1D
      ElecDiLepContributionBTagFail_->Fill(BTags,Weight);
      ElecDiLepContributionNJetsFail_->Fill(NJets,Weight);
      ElecDiLepContributionHTFail_->Fill(HT,Weight);
      ElecDiLepContributionMHTFail_->Fill(MHT,Weight);
      if(MTW <mtwCut_)
	{
	  // 1D
	  ElecDiLepContributionMTWBTagFail_->Fill(BTags,Weight);
	  ElecDiLepContributionMTWNJetsFail_->Fill(NJets,Weight);
	  ElecDiLepContributionMTWHTFail_->Fill(HT,Weight);
	  ElecDiLepContributionMTWMHTFail_->Fill(MHT,Weight);
	}
    }
  
  if(ElecDiLepControlSample==0)
    {
      // 1D
      ElecDiLepBTagFail_->Fill(BTags,Weight);
      ElecDiLepNJetsFail_->Fill(NJets,Weight);
      ElecDiLepHTFail_->Fill(HT,Weight);
      ElecDiLepMHTFail_->Fill(MHT,Weight);
    
      // 1D
      ElecDiLepMTWBTagFail_->Fill(BTags,Weight);
      ElecDiLepMTWNJetsFail_->Fill(NJets,Weight);
      ElecDiLepMTWHTFail_->Fill(HT,Weight);
      ElecDiLepMTWMHTFail_->Fill(MHT,Weight);
    }

  // di leptonic contribution
  if((GenMuNum+GenElecNum)==2)
    {
      // single muon control sample correction due to di lep contribution
      if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
	{
	  // fill here for events that enter in the single muon control sample from di lep failing: POINT1
	  // passing: point 4
	}
      // single el control sample correction due to di lep contribution
      if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
	{
	  // fill here for events that enter in the single muon control sample from di lep failing: POINT2
	  // passing: point 4
	}
      // single isotrack control sample correction due to di lep contribution
      // if(IsolatedTracksNum==1)
      // {
      //   // fill here for events that enter in the single muon control sample from di lep failing: POINT3
      //   // passing: point 4
      // }
      // for efficiency for di leptonic decays check if non are isolated
      if((selectedIDIsoMuonsNum+selectedIDIsoElectronsNum)==0)
	{
	  // failing: point 4
	}
    }
  if(GenMuNum==1 && GenElecNum==0)
    {
      if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
	{
	  // passing: POINT1
	}
    }
  if(GenMuNum==0 && GenElecNum==1)
    {
      if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
	{
	  //passing: POINT2
	}
    }
  // if((GenMuNum+GenElecNum)==1)
  //   {
  //     if( IsolatedTracksNum==1)
  // 	{
  // 	  // passing: POINT3
  // 	}
  //   }

  //////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////////
  
  // TEfficiencies
  
  
  // purity
  // single muon control sample
  if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
    {
      if(mu_tm->at(0)==1)
	{
	  //1D
	  MuPurityBTagEff_->Fill(BTags,Weight,true);
	  MuPurityNJetsEff_->Fill(NJets,Weight,true);
	  MuPurityHTEff_->Fill(HT,Weight,true);
	  MuPurityMHTEff_->Fill(MHT,Weight,true);
	  MuPurityPTEff_->Fill(mu_pt->at(0),Weight,true);
	  MuPurityActivityEff_->Fill(mu_activity_mu->at(0),Weight,true);
	  //2D
	  MuonPurityMHTNJetEff_->Fill(MHT,NJets,Weight,true);
	  // search bin efficiencies
	  MuPuritySearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
	  // 			if(MHT>750)std::cout<<"push_back("<<EvtNum<<"); // passing"<<std::endl;
	}
      if(mu_tm->at(0)==0)
	{
	  //1D
	  MuPurityBTagEff_->Fill(BTags,Weight,false);
	  MuPurityNJetsEff_->Fill(NJets,Weight,false);
	  MuPurityHTEff_->Fill(HT,Weight,false);
	  MuPurityMHTEff_->Fill(MHT,Weight,false);
	  MuPurityPTEff_->Fill(mu_pt->at(0),Weight,false);
	  MuPurityActivityEff_->Fill(mu_activity_mu->at(0),Weight,false);
	  //2D
	  MuonPurityMHTNJetEff_->Fill(MHT,NJets,Weight,false);
	  // search bin efficiencies
	  MuPuritySearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
	  // 			if(MHT>750)std::cout<<"push_back("<<EvtNum<<"); // failing"<<std::endl;
	}
    }
  // single el control sample
  if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
    {
      if(el_tm->at(0)==1)
	{
	  //1D
	  ElecPurityBTagEff_->Fill(BTags,Weight,true);
	  ElecPurityNJetsEff_->Fill(NJets,Weight,true);
	  ElecPurityHTEff_->Fill(HT,Weight,true);
	  ElecPurityMHTEff_->Fill(MHT,Weight,true);
	  ElecPurityPTEff_->Fill(el_pt->at(0),Weight,true);
	  ElecPurityActivityEff_->Fill(el_activity_el->at(0),Weight,true);
	  //2D
	  ElecPurityMHTNJetEff_->Fill(MHT,NJets,Weight,true);
	  // search bin efficiencies
	  ElecPuritySearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
	}
      if(el_tm->at(0)==0)
	{
	  //1D
	  ElecPurityBTagEff_->Fill(BTags,Weight,false);
	  ElecPurityNJetsEff_->Fill(NJets,Weight,false);
	  ElecPurityHTEff_->Fill(HT,Weight,false);
	  ElecPurityMHTEff_->Fill(MHT,Weight,false);
	  ElecPurityPTEff_->Fill(el_pt->at(0),Weight,false);
	  ElecPurityActivityEff_->Fill(el_activity_el->at(0),Weight,false);
	  //2D
	  ElecPurityMHTNJetEff_->Fill(MHT,NJets,Weight,false);
	  // search bin efficiencies
	  ElecPuritySearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
	}
    }
  // acceptance
  // single muon control sample
  if(muAcc==2)
    {
      //1D
      MuAccBTagEff_->Fill(BTags,Weight,true);
      MuAccNJetsEff_->Fill(NJets,Weight,true);
      MuAccHTEff_->Fill(HT,Weight,true);
      MuAccMHTEff_->Fill(MHT,Weight,true);
      MuAccPTEff_->Fill(true_mu_pt->at(0),Weight,true);
      MuAccActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,true);
      //2D
      MuAccHTNJetsEff_->Fill(HT,NJets,Weight,true);
      MuAccMHTNJetsEff_->Fill(MHT,NJets,Weight,true);
      MuAccBTagNJetsEff_->Fill(BTags,NJets,Weight,true);
      // search bin efficiencies
      MuAccSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(muAcc==0)
    {
      //1D
      MuAccBTagEff_->Fill(BTags,Weight,false);
      MuAccNJetsEff_->Fill(NJets,Weight,false);
      MuAccHTEff_->Fill(HT,Weight,false);
      MuAccMHTEff_->Fill(MHT,Weight,false);
      MuAccPTEff_->Fill(true_mu_pt->at(0),Weight,false);
      MuAccActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,false);
      //2D
      MuAccHTNJetsEff_->Fill(HT,NJets,Weight,false);
      MuAccMHTNJetsEff_->Fill(MHT,NJets,Weight,false);
      MuAccBTagNJetsEff_->Fill(BTags,NJets,Weight,false);
      // search bin efficiencies
      MuAccSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
	
  // single elon control sample
  if(elecAcc==2)
    {
      //1D
      ElecAccBTagEff_->Fill(BTags,Weight,true);
      ElecAccNJetsEff_->Fill(NJets,Weight,true);
      ElecAccHTEff_->Fill(HT,Weight,true);
      ElecAccMHTEff_->Fill(MHT,Weight,true);
      ElecAccPTEff_->Fill(true_el_pt->at(0),Weight,true);
      ElecAccActivityEff_->Fill(true_el_activity_el->at(0),Weight,true);
      //2D
      ElecAccHTNJetsEff_->Fill(HT,NJets,Weight,true);
      ElecAccMHTNJetsEff_->Fill(MHT,NJets,Weight,true);
      ElecAccBTagNJetsEff_->Fill(BTags,NJets,Weight,true);
      // search bin efficiencies
      ElecAccSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(elecAcc==0)
    {
      //1D
      ElecAccBTagEff_->Fill(BTags,Weight,false);
      ElecAccNJetsEff_->Fill(NJets,Weight,false);
      ElecAccHTEff_->Fill(HT,Weight,false);
      ElecAccMHTEff_->Fill(MHT,Weight,false);
      ElecAccPTEff_->Fill(true_el_pt->at(0),Weight,false);
      ElecAccActivityEff_->Fill(true_el_activity_el->at(0),Weight,false);
      //2D
      ElecAccHTNJetsEff_->Fill(HT,NJets,Weight,false);
      ElecAccMHTNJetsEff_->Fill(MHT,NJets,Weight,false);
      ElecAccBTagNJetsEff_->Fill(BTags,NJets,Weight,false);
      // search bin efficiencies
      ElecAccSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
	
  // reconstruction
  // single muon control sample
  if(muReco==2)
    {
      //1D
      MuRecoBTagEff_->Fill(BTags,Weight,true);
      MuRecoNJetsEff_->Fill(NJets,Weight,true);
      MuRecoHTEff_->Fill(HT,Weight,true);
      MuRecoMHTEff_->Fill(MHT,Weight,true);
      MuRecoPTEff_->Fill(true_mu_pt->at(0),Weight,true);
      MuRecoActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,true);
      //2D
      MuRecoPTActivityEff_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight,true);
      // search bin efficiencies
      MuRecoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(muReco==0)
    {
      //1D
      MuRecoBTagEff_->Fill(BTags,Weight,false);
      MuRecoNJetsEff_->Fill(NJets,Weight,false);
      MuRecoHTEff_->Fill(HT,Weight,false);
      MuRecoMHTEff_->Fill(MHT,Weight,false);
      MuRecoPTEff_->Fill(true_mu_pt->at(0),Weight,false);
      MuRecoActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,false);
      //2D
      MuRecoPTActivityEff_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight,false);
      // search bin efficiencies
      MuRecoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
	
  // single elon control sample
  if(elecReco==2)
    {
      //1D
      ElecRecoBTagEff_->Fill(BTags,Weight,true);
      ElecRecoNJetsEff_->Fill(NJets,Weight,true);
      ElecRecoHTEff_->Fill(HT,Weight,true);
      ElecRecoMHTEff_->Fill(MHT,Weight,true);
      ElecRecoPTEff_->Fill(true_el_pt->at(0),Weight,true);
      ElecRecoActivityEff_->Fill(true_el_activity_el->at(0),Weight,true);
      //2D
      ElecRecoPTActivityEff_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight,true);
      // search bin efficiencies
      ElecRecoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(elecReco==0)
    {
      //1D
      ElecRecoBTagEff_->Fill(BTags,Weight,false);
      ElecRecoNJetsEff_->Fill(NJets,Weight,false);
      ElecRecoHTEff_->Fill(HT,Weight,false);
      ElecRecoMHTEff_->Fill(MHT,Weight,false);
      ElecRecoPTEff_->Fill(true_el_pt->at(0),Weight,false);
      ElecRecoActivityEff_->Fill(true_el_activity_el->at(0),Weight,false);
      //2D
      ElecRecoPTActivityEff_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight,false);
      // search bin efficiencies
      ElecRecoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
	
  // isolation
  // single muon control sample
  if(muIso==2)
    {
      //1D
      MuIsoBTagEff_->Fill(BTags,Weight,true);
      MuIsoNJetsEff_->Fill(NJets,Weight,true);
      MuIsoHTEff_->Fill(HT,Weight,true);
      MuIsoMHTEff_->Fill(MHT,Weight,true);
      MuIsoPTEff_->Fill(true_mu_pt->at(0),Weight,true);
      MuIsoActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,true);
      //2D
      MuIsoPTActivityEff_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight,true);
      // search bin efficiencies
      MuIsoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(muIso==0)
    {
      //1D
      MuIsoBTagEff_->Fill(BTags,Weight,false);
      MuIsoNJetsEff_->Fill(NJets,Weight,false);
      MuIsoHTEff_->Fill(HT,Weight,false);
      MuIsoMHTEff_->Fill(MHT,Weight,false);
      MuIsoPTEff_->Fill(true_mu_pt->at(0),Weight,false);
      MuIsoActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,false);
      //2D
      MuIsoPTActivityEff_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight,false);
      // search bin efficiencies
      MuIsoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
	
  // single elon control sample
  if(elecIso==2)
    {
      //1D
      ElecIsoBTagEff_->Fill(BTags,Weight,true);
      ElecIsoNJetsEff_->Fill(NJets,Weight,true);
      ElecIsoHTEff_->Fill(HT,Weight,true);
      ElecIsoMHTEff_->Fill(MHT,Weight,true);
      ElecIsoPTEff_->Fill(true_el_pt->at(0),Weight,true);
      ElecIsoActivityEff_->Fill(true_el_activity_el->at(0),Weight,true);
      //2D
      ElecIsoPTActivityEff_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight,true);
      // search bin efficiencies
      ElecIsoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(elecIso==0)
    {
      //1D
      ElecIsoBTagEff_->Fill(BTags,Weight,false);
      ElecIsoNJetsEff_->Fill(NJets,Weight,false);
      ElecIsoHTEff_->Fill(HT,Weight,false);
      ElecIsoMHTEff_->Fill(MHT,Weight,false);
      ElecIsoPTEff_->Fill(true_el_pt->at(0),Weight,false);
      ElecIsoActivityEff_->Fill(true_el_activity_el->at(0),Weight,false);
      //2D
      ElecIsoPTActivityEff_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight,false);
      // search bin efficiencies
      ElecIsoSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
  // mtw
  // single muon control sample
  if(muIso==2 && MTW < mtwCut_ && MTW>0)
    {
      // 1D
      MuMTWBTagEff_->Fill(BTags,Weight,true);
      MuMTWNJetsEff_->Fill(NJets,Weight,true);
      MuMTWHTEff_->Fill(HT,Weight,true);
      MuMTWMHTEff_->Fill(MHT,Weight,true);
      MuMTWPTEff_->Fill(true_mu_pt->at(0),Weight,true);
      MuMTWActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,true);
      MuMTWPTActivityEff_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight,true);
      // search bin efficiencies
      MuMTWSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(muIso==2 && MTW > mtwCut_)
    {
      // 1D
      MuMTWBTagEff_->Fill(BTags,Weight,false);
      MuMTWNJetsEff_->Fill(NJets,Weight,false);
      MuMTWHTEff_->Fill(HT,Weight,false);
      MuMTWMHTEff_->Fill(MHT,Weight,false);
      MuMTWPTEff_->Fill(true_mu_pt->at(0),Weight,false);
      MuMTWActivityEff_->Fill(true_mu_activity_mu->at(0),Weight,false);
      MuMTWPTActivityEff_->Fill(true_mu_pt->at(0),true_mu_activity_mu->at(0),Weight,false);
      // search bin efficiencies
      MuMTWSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
	
  // single el control sample
  if(elecIso==2 && MTW < mtwCut_ && MTW>0)
    {
      // 1D
      ElecMTWBTagEff_->Fill(BTags,Weight,true);
      ElecMTWNJetsEff_->Fill(NJets,Weight,true);
      ElecMTWHTEff_->Fill(HT,Weight,true);
      ElecMTWMHTEff_->Fill(MHT,Weight,true);
      ElecMTWPTEff_->Fill(true_el_pt->at(0),Weight,true);
      ElecMTWActivityEff_->Fill(true_el_activity_el->at(0),Weight,true);
      ElecMTWPTActivityEff_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight,true);
      // search bin efficiencies
      ElecMTWSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
    }
  if(elecIso==2 && MTW > mtwCut_)
    {
      // 1D
      ElecMTWBTagEff_->Fill(BTags,Weight,false);
      ElecMTWNJetsEff_->Fill(NJets,Weight,false);
      ElecMTWHTEff_->Fill(HT,Weight,false);
      ElecMTWMHTEff_->Fill(MHT,Weight,false);
      ElecMTWPTEff_->Fill(true_el_pt->at(0),Weight,false);
      ElecMTWActivityEff_->Fill(true_el_activity_el->at(0),Weight,false);
      ElecMTWPTActivityEff_->Fill(true_el_pt->at(0),true_el_activity_el->at(0),Weight,false);
      // search bin efficiencies
      ElecMTWSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
  // di lep contribution
  if(MuDiLepControlSample==2)
    {
      // 1D
      MuDiLepBTagEff_->Fill(BTags,Weight,true);
      MuDiLepNJetsEff_->Fill(NJets,Weight,true);
      MuDiLepHTEff_->Fill(HT,Weight,true);
      MuDiLepMHTEff_->Fill(MHT,Weight,true);
      // search bin efficiencies
      MuDiLepEffMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
      if(MTW <mtwCut_)
	{
	  // 1D
	  MuDiLepMTWBTagEff_->Fill(BTags,Weight,true);
	  MuDiLepMTWNJetsEff_->Fill(NJets,Weight,true);
	  MuDiLepMTWHTEff_->Fill(HT,Weight,true);
	  MuDiLepMTWMHTEff_->Fill(MHT,Weight,true);
	}
      // 1D
      MuDiLepContributionBTagEff_->Fill(BTags,Weight,true);
      MuDiLepContributionNJetsEff_->Fill(NJets,Weight,true);
      MuDiLepContributionHTEff_->Fill(HT,Weight,true);
      MuDiLepContributionMHTEff_->Fill(MHT,Weight,true);
      if(MTW <mtwCut_)
	{
	  // 1D
	  MuDiLepContributionMTWBTagEff_->Fill(BTags,Weight,true);
	  MuDiLepContributionMTWNJetsEff_->Fill(NJets,Weight,true);
	  MuDiLepContributionMTWHTEff_->Fill(HT,Weight,true);
	  MuDiLepContributionMTWMHTEff_->Fill(MHT,Weight,true);
	  // search bin efficiencies
	  MuDiLepContributionMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
	}
		
    }
  if(MuDiLepControlSample==0)
    {
		
      // 1D
      MuDiLepContributionBTagEff_->Fill(BTags,Weight,false);
      MuDiLepContributionNJetsEff_->Fill(NJets,Weight,false);
      MuDiLepContributionHTEff_->Fill(HT,Weight,false);
      MuDiLepContributionMHTEff_->Fill(MHT,Weight,false);
      if(MTW <mtwCut_)
	{
	  // 1D
	  MuDiLepContributionMTWBTagEff_->Fill(BTags,Weight,false);
	  MuDiLepContributionMTWNJetsEff_->Fill(NJets,Weight,false);
	  MuDiLepContributionMTWHTEff_->Fill(HT,Weight,false);
	  MuDiLepContributionMTWMHTEff_->Fill(MHT,Weight,false);
	  // search bin efficiencies
	  MuDiLepContributionMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
	}
    }
  if(MuDiLepControlSample==0)
    {
      // 1D
      MuDiLepBTagEff_->Fill(BTags,Weight,false);
      MuDiLepNJetsEff_->Fill(NJets,Weight,false);
      MuDiLepHTEff_->Fill(HT,Weight,false);
      MuDiLepMHTEff_->Fill(MHT,Weight,false);
		
      // 1D
      MuDiLepMTWBTagEff_->Fill(BTags,Weight,false);
      MuDiLepMTWNJetsEff_->Fill(NJets,Weight,false);
      MuDiLepMTWHTEff_->Fill(HT,Weight,false);
      MuDiLepMTWMHTEff_->Fill(MHT,Weight,false);
      // search bin efficiencies
      MuDiLepEffMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
  if(ElecDiLepControlSample==2)
    {
      // 1D
      ElecDiLepBTagEff_->Fill(BTags,Weight,true);
      ElecDiLepNJetsEff_->Fill(NJets,Weight,true);
      ElecDiLepHTEff_->Fill(HT,Weight,true);
      ElecDiLepMHTEff_->Fill(MHT,Weight,true);
      // search bin efficiencies
      ElecDiLepEffMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
      if(MTW <mtwCut_)
	{
	  // 1D
	  ElecDiLepMTWBTagEff_->Fill(BTags,Weight,true);
	  ElecDiLepMTWNJetsEff_->Fill(NJets,Weight,true);
	  ElecDiLepMTWHTEff_->Fill(HT,Weight,true);
	  ElecDiLepMTWMHTEff_->Fill(MHT,Weight,true);
	}
      // 1D
      ElecDiLepContributionBTagEff_->Fill(BTags,Weight,true);
      ElecDiLepContributionNJetsEff_->Fill(NJets,Weight,true);
      ElecDiLepContributionHTEff_->Fill(HT,Weight,true);
      ElecDiLepContributionMHTEff_->Fill(MHT,Weight,true);
      if(MTW <mtwCut_)
	{
	  // 1D
	  ElecDiLepContributionMTWBTagEff_->Fill(BTags,Weight,true);
	  ElecDiLepContributionMTWNJetsEff_->Fill(NJets,Weight,true);
	  ElecDiLepContributionMTWHTEff_->Fill(HT,Weight,true);
	  ElecDiLepContributionMTWMHTEff_->Fill(MHT,Weight,true);
	  // search bin efficiencies
	  ElecDiLepContributionMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,true);
	}
		
    }
  if(ElecDiLepControlSample==0)
    {
		
      // 1D
      ElecDiLepContributionBTagEff_->Fill(BTags,Weight,false);
      ElecDiLepContributionNJetsEff_->Fill(NJets,Weight,false);
      ElecDiLepContributionHTEff_->Fill(HT,Weight,false);
      ElecDiLepContributionMHTEff_->Fill(MHT,Weight,false);
      if(MTW <mtwCut_)
	{
	  // 1D
	  ElecDiLepContributionMTWBTagEff_->Fill(BTags,Weight,false);
	  ElecDiLepContributionMTWNJetsEff_->Fill(NJets,Weight,false);
	  ElecDiLepContributionMTWHTEff_->Fill(HT,Weight,false);
	  ElecDiLepContributionMTWMHTEff_->Fill(MHT,Weight,false);
	  // search bin efficiencies
	  ElecDiLepContributionMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
	}
    }
	
  if(ElecDiLepControlSample==0)
    {
      // 1D
      ElecDiLepBTagEff_->Fill(BTags,Weight,false);
      ElecDiLepNJetsEff_->Fill(NJets,Weight,false);
      ElecDiLepHTEff_->Fill(HT,Weight,false);
      ElecDiLepMHTEff_->Fill(MHT,Weight,false);
		
      // 1D
      ElecDiLepMTWBTagEff_->Fill(BTags,Weight,false);
      ElecDiLepMTWNJetsEff_->Fill(NJets,Weight,false);
      ElecDiLepMTWHTEff_->Fill(HT,Weight,false);
      ElecDiLepMTWMHTEff_->Fill(MHT,Weight,false);
      // search bin efficiencies
      ElecDiLepEffMTWAppliedSearchBinEff_->Fill(HT,MHT,NJets,BTags,Weight,false);
    }
  // di leptonic contribution
  if((GenMuNum+GenElecNum)==2)
    {
      // single muon control sample correction due to di lep contribution
      if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
	{
	  // fill here for events that enter in the single muon control sample from di lep failing: POINT1
	  // passing: point 4
	}
      // single el control sample correction due to di lep contribution
      if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
	{
	  // fill here for events that enter in the single muon control sample from di lep failing: POINT2
	  // passing: point 4
	}
      // single isotrack control sample correction due to di lep contribution
      // if(IsolatedTracksNum==1)
      // {
      // 	// fill here for events that enter in the single muon control sample from di lep failing: POINT3
      // 	// passing: point 4
      // }
      // for efficiency for di leptonic decays check if non are isolated
      if((selectedIDIsoMuonsNum+selectedIDIsoElectronsNum)==0)
	{
	  // failing: point 4
	}
    }
  if(GenMuNum==1 && GenElecNum==0)
    {
      if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
	{
	  // passing: POINT1
	}
    }
  if(GenMuNum==0 && GenElecNum==1)
    {
      if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
	{
	  //passing: POINT2
	}
    }
  // if((GenMuNum+GenElecNum)==1)
  //   {
  //     if( IsolatedTracksNum==1)
  // 	{
  // 	  // passing: POINT3
  // 	}
  //   }

  return kTRUE;
}

void RTree::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.

}

void RTree::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  TString outdir="macros/junews/lostleptons/arne/Efficiencies/";
  TFile *outPutFile = new TFile(outdir+"Efficiencies.root","RECREATE"); 
  outPutFile->cd();
  outPutFile->mkdir("Efficiencies");
  TDirectory *dEfficiencies = (TDirectory*)outPutFile->Get("Efficiencies");
  dEfficiencies->cd();
  gStyle->SetPaintTextFormat("5.2f");
  gStyle->SetStatW(0.1);
  gStyle->SetStatH(0.1);
  gStyle->SetStatY(202);
  gStyle->SetTitleYOffset(1.3);

  // compute and save efficiencies
  // purity
  //muon
  //1D
  MuPurityBTag_ = ratioCalculator(MuPurityBTag_,MuPurityBTagFail_);   
  MuPurityBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; B_{Tags}");
  MuPurityBTag_->SetMarkerSize(2.0);
  MuPurityBTag_->UseCurrentStyle();
  MuPurityBTag_->Write();
  SaveEfficiency(MuPurityBTag_);
  
  MuPurityNJets_ = ratioCalculator(MuPurityNJets_,MuPurityNJetsFail_);   
  MuPurityNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; N_{Jets}");
  MuPurityNJets_->SetMarkerSize(2.0);
  MuPurityNJets_->UseCurrentStyle();
  MuPurityNJets_->Write();
  SaveEfficiency(MuPurityNJets_);
  
  MuPurityHT_ = ratioCalculator(MuPurityHT_,MuPurityHTFail_);   
  MuPurityHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; H_{T} [GeV]");
  MuPurityHT_->SetMarkerSize(2.0);
  MuPurityHT_->UseCurrentStyle();
  MuPurityHT_->Write();
  SaveEfficiency(MuPurityHT_);
  
  MuPurityMHT_ = ratioCalculator(MuPurityMHT_,MuPurityMHTFail_);   
  MuPurityMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; #slash{H}_{T} [GeV]");
  MuPurityMHT_->SetMarkerSize(2.0);
  MuPurityMHT_->UseCurrentStyle();
  MuPurityMHT_->Write();
  SaveEfficiency(MuPurityMHT_);
  //2D
  MuonPurityMHTNJet = ratioCalculator(MuonPurityMHTNJet,MuonPurityMHTNJetFail);   
  MuonPurityMHTNJet->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; #slash{H}_{T} [GeV]; N_{Jets}");
  MuonPurityMHTNJet->SetMarkerSize(2.0);
  MuonPurityMHTNJet->UseCurrentStyle();
  MuonPurityMHTNJet->Write();
  SaveEfficiency(MuonPurityMHTNJet);
  
  //el
  //1D
  ElecPurityBTag_ = ratioCalculator(ElecPurityBTag_,ElecPurityBTagFail_);   
  ElecPurityBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; B_{Tags}");
  ElecPurityBTag_->SetMarkerSize(2.0);
  ElecPurityBTag_->UseCurrentStyle();
  ElecPurityBTag_->Write();
  SaveEfficiency(ElecPurityBTag_);
  
  ElecPurityNJets_ = ratioCalculator(ElecPurityNJets_,ElecPurityNJetsFail_);   
  ElecPurityNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; N_{Jets}");
  ElecPurityNJets_->SetMarkerSize(2.0);
  ElecPurityNJets_->UseCurrentStyle();
  ElecPurityNJets_->Write();
  SaveEfficiency(ElecPurityNJets_);
  
  ElecPurityHT_ = ratioCalculator(ElecPurityHT_,ElecPurityHTFail_);   
  ElecPurityHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; H_{T} [GeV]");
  ElecPurityHT_->SetMarkerSize(2.0);
  ElecPurityHT_->UseCurrentStyle();
  ElecPurityHT_->Write();
  SaveEfficiency(ElecPurityHT_);
  
  ElecPurityMHT_ = ratioCalculator(ElecPurityMHT_,ElecPurityMHTFail_);   
  ElecPurityMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; #slash{H}_{T} [GeV]");
  ElecPurityMHT_->SetMarkerSize(2.0);
  ElecPurityMHT_->UseCurrentStyle();
  ElecPurityMHT_->Write();
  SaveEfficiency(ElecPurityMHT_);
  //2D
  ElecPurityMHTNJet = ratioCalculator(ElecPurityMHTNJet,ElecPurityMHTNJetFail);   
  ElecPurityMHTNJet->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; #slash{H}_{T} [GeV]; N_{Jets}");
  ElecPurityMHTNJet->SetMarkerSize(2.0);
  ElecPurityMHTNJet->UseCurrentStyle();
  ElecPurityMHTNJet->Write();
  SaveEfficiency(ElecPurityMHTNJet);
  
  MuPurityBTag_ = ratioCalculator(MuPurityBTag_,MuPurityBTagFail_);   
  MuPurityBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; B_{Tags}");
  MuPurityBTag_->SetMarkerSize(2.0);
  MuPurityBTag_->UseCurrentStyle();
  MuPurityBTag_->Write();
  SaveEfficiency(MuPurityBTag_);
  
  MuPurityCheckNJets_ = ratioCalculator(MuPurityCheckNJets_,MuPurityCheckNJetsFail_);   
  MuPurityCheckNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; N_{Jets}");
  MuPurityCheckNJets_->SetMarkerSize(2.0);
  MuPurityCheckNJets_->UseCurrentStyle();
  MuPurityCheckNJets_->Write();
  SaveEfficiency(MuPurityCheckNJets_);
  
  MuPurityCheckHT_ = ratioCalculator(MuPurityCheckHT_,MuPurityCheckHTFail_);   
  MuPurityCheckHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; H_{T} [GeV]");
  MuPurityCheckHT_->SetMarkerSize(2.0);
  MuPurityCheckHT_->UseCurrentStyle();
  MuPurityCheckHT_->Write();
  SaveEfficiency(MuPurityCheckHT_);
  
  MuPurityCheckMHT_ = ratioCalculator(MuPurityCheckMHT_,MuPurityCheckMHTFail_);   
  MuPurityCheckMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; #slash{H}_{T} [GeV]");
  MuPurityCheckMHT_->SetMarkerSize(2.0);
  MuPurityCheckMHT_->UseCurrentStyle();
  MuPurityCheckMHT_->Write();
  SaveEfficiency(MuPurityCheckMHT_);
  //2D
  MuonPurityCheckMHTNJet = ratioCalculator(MuonPurityCheckMHTNJet,MuonPurityCheckMHTNJetFail);   
  MuonPurityCheckMHTNJet->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; #slash{H}_{T} [GeV]; N_{Jets}");
  MuonPurityCheckMHTNJet->SetMarkerSize(2.0);
  MuonPurityCheckMHTNJet->UseCurrentStyle();
  MuonPurityCheckMHTNJet->Write();
  SaveEfficiency(MuonPurityCheckMHTNJet);
  
  //el
  //1D
  ElecPurityCheckBTag_ = ratioCalculator(ElecPurityCheckBTag_,ElecPurityCheckBTagFail_);   
  ElecPurityCheckBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; B_{Tags}");
  ElecPurityCheckBTag_->SetMarkerSize(2.0);
  ElecPurityCheckBTag_->UseCurrentStyle();
  ElecPurityCheckBTag_->Write();
  SaveEfficiency(ElecPurityCheckBTag_);
  
  ElecPurityCheckNJets_ = ratioCalculator(ElecPurityCheckNJets_,ElecPurityCheckNJetsFail_);   
  ElecPurityCheckNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; N_{Jets}");
  ElecPurityCheckNJets_->SetMarkerSize(2.0);
  ElecPurityCheckNJets_->UseCurrentStyle();
  ElecPurityCheckNJets_->Write();
  SaveEfficiency(ElecPurityCheckNJets_);
  
  ElecPurityCheckHT_ = ratioCalculator(ElecPurityCheckHT_,ElecPurityCheckHTFail_);   
  ElecPurityCheckHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; H_{T} [GeV]");
  ElecPurityCheckHT_->SetMarkerSize(2.0);
  ElecPurityCheckHT_->UseCurrentStyle();
  ElecPurityCheckHT_->Write();
  SaveEfficiency(ElecPurityCheckHT_);
  
  ElecPurityCheckMHT_ = ratioCalculator(ElecPurityCheckMHT_,ElecPurityCheckMHTFail_);   
  ElecPurityCheckMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; #slash{H}_{T} [GeV]");
  ElecPurityCheckMHT_->SetMarkerSize(2.0);
  ElecPurityCheckMHT_->UseCurrentStyle();
  ElecPurityCheckMHT_->Write();
  SaveEfficiency(ElecPurityCheckMHT_);
  //2D
  ElecPurityCheckMHTNJet = ratioCalculator(ElecPurityCheckMHTNJet,ElecPurityCheckMHTNJetFail);   
  ElecPurityCheckMHTNJet->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; #slash{H}_{T} [GeV]; N_{Jets}");
  ElecPurityCheckMHTNJet->SetMarkerSize(2.0);
  ElecPurityCheckMHTNJet->UseCurrentStyle();
  ElecPurityCheckMHTNJet->Write();
  SaveEfficiency(ElecPurityCheckMHTNJet);
  
  
  
  // acc
  //muon
  //1D
  MuAccBTag_ = ratioCalculator(MuAccBTag_,MuAccBTagFail_);   
  MuAccBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; B_{Tags}");
  MuAccBTag_->SetMarkerSize(2.0);
  MuAccBTag_->UseCurrentStyle();
  MuAccBTag_->Write();
  SaveEfficiency(MuAccBTag_);
  
  MuAccNJets_ = ratioCalculator(MuAccNJets_,MuAccNJetsFail_);   
  MuAccNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; N_{Jets}");
  MuAccNJets_->SetMarkerSize(2.0);
  MuAccNJets_->UseCurrentStyle();
  MuAccNJets_->Write();
  SaveEfficiency(MuAccNJets_);
  
  MuAccHT_ = ratioCalculator(MuAccHT_,MuAccHTFail_);   
  MuAccHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; H_{T} [GeV]");
  MuAccHT_->SetMarkerSize(2.0);
  MuAccHT_->UseCurrentStyle();
  MuAccHT_->Write();
  SaveEfficiency(MuAccHT_);
  
  MuAccMHT_ = ratioCalculator(MuAccMHT_,MuAccMHTFail_);   
  MuAccMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; #slash{H}_{T} [GeV]");
  MuAccMHT_->SetMarkerSize(2.0);
  MuAccMHT_->UseCurrentStyle();
  MuAccMHT_->Write();
  SaveEfficiency(MuAccMHT_);
  
  // 2D 
  MuAccHTNJets_ = ratioCalculator(MuAccHTNJets_,MuAccHTNJetsFail_);   
  MuAccHTNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; H_{T} [GeV]; N_{Jets}");
  MuAccHTNJets_->SetMarkerSize(2.0);
  MuAccHTNJets_->UseCurrentStyle();
  MuAccHTNJets_->Write();
  SaveEfficiency(MuAccHTNJets_);
	
  MuAccMHTNJets_ = ratioCalculator(MuAccMHTNJets_,MuAccMHTNJetsFail_);   
  MuAccMHTNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; #slash{H}_{T} [GeV]; N_{Jets}");
  MuAccMHTNJets_->SetMarkerSize(2.0);
  MuAccMHTNJets_->UseCurrentStyle();
  MuAccMHTNJets_->Write();
  SaveEfficiency(MuAccMHTNJets_);
	
  MuAccBTagNJets_ = ratioCalculator(MuAccBTagNJets_,MuAccBTagNJetsFail_);   
  MuAccBTagNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; B_{Tags}; N_{Jets}");
  MuAccBTagNJets_->SetMarkerSize(2.0);
  MuAccBTagNJets_->UseCurrentStyle();
  MuAccBTagNJets_->Write();
  SaveEfficiency(MuAccBTagNJets_);
	
  MuAccMHTNJetsB0_ = ratioCalculator(MuAccMHTNJetsB0_,MuAccMHTNJetsB0Fail_);   
  MuAccMHTNJetsB0_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc B_{Tags}=0; #slash{H}_{T} [GeV]; N_{Jets}");
  MuAccMHTNJetsB0_->SetMarkerSize(2.0);
  MuAccMHTNJetsB0_->UseCurrentStyle();
  MuAccMHTNJetsB0_->Write();
  SaveEfficiency(MuAccMHTNJetsB0_);
	
  MuAccMHTNJetsB1_Inf_ = ratioCalculator(MuAccMHTNJetsB1_Inf_,MuAccMHTNJetsB1_InfFail_);   
  MuAccMHTNJetsB1_Inf_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc B_{Tags}#geq1; #slash{H}_{T} [GeV]; N_{Jets}");
  MuAccMHTNJetsB1_Inf_->SetMarkerSize(2.0);
  MuAccMHTNJetsB1_Inf_->UseCurrentStyle();
  MuAccMHTNJetsB1_Inf_->Write();
  SaveEfficiency(MuAccMHTNJetsB1_Inf_);
  
  //el
  //1D
  ElecAccBTag_ = ratioCalculator(ElecAccBTag_,ElecAccBTagFail_);   
  ElecAccBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; B_{Tags}");
  ElecAccBTag_->SetMarkerSize(2.0);
  ElecAccBTag_->UseCurrentStyle();
  ElecAccBTag_->Write();
  SaveEfficiency(ElecAccBTag_);
  
  ElecAccNJets_ = ratioCalculator(ElecAccNJets_,ElecAccNJetsFail_);   
  ElecAccNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; N_{Jets}");
  ElecAccNJets_->SetMarkerSize(2.0);
  ElecAccNJets_->UseCurrentStyle();
  ElecAccNJets_->Write();
  SaveEfficiency(ElecAccNJets_);
  
  ElecAccHT_ = ratioCalculator(ElecAccHT_,ElecAccHTFail_);   
  ElecAccHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; H_{T} [GeV]");
  ElecAccHT_->SetMarkerSize(2.0);
  ElecAccHT_->UseCurrentStyle();
  ElecAccHT_->Write();
  SaveEfficiency(ElecAccHT_);
  
  ElecAccMHT_ = ratioCalculator(ElecAccMHT_,ElecAccMHTFail_);   
  ElecAccMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; #slash{H}_{T} [GeV]");
  ElecAccMHT_->SetMarkerSize(2.0);
  ElecAccMHT_->UseCurrentStyle();
  ElecAccMHT_->Write();
  SaveEfficiency(ElecAccMHT_);
  
  // 2D 
  ElecAccHTNJets_ = ratioCalculator(ElecAccHTNJets_,ElecAccHTNJetsFail_);   
  ElecAccHTNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; H_{T} [GeV]; N_{Jets}");
  ElecAccHTNJets_->SetMarkerSize(2.0);
  ElecAccHTNJets_->UseCurrentStyle();
  ElecAccHTNJets_->Write();
  SaveEfficiency(ElecAccHTNJets_);
	
  ElecAccMHTNJets_ = ratioCalculator(ElecAccMHTNJets_,ElecAccMHTNJetsFail_);   
  ElecAccMHTNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; #slash{H}_{T} [GeV]; N_{Jets}");
  ElecAccMHTNJets_->SetMarkerSize(2.0);
  ElecAccMHTNJets_->UseCurrentStyle();
  ElecAccMHTNJets_->Write();
  SaveEfficiency(ElecAccMHTNJets_);
	
  ElecAccBTagNJets_ = ratioCalculator(ElecAccBTagNJets_,ElecAccBTagNJetsFail_);   
  ElecAccBTagNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; B_{Tags}; N_{Jets}");
  ElecAccBTagNJets_->SetMarkerSize(2.0);
  ElecAccBTagNJets_->UseCurrentStyle();
  ElecAccBTagNJets_->Write();
  SaveEfficiency(ElecAccBTagNJets_);
	
  ElecAccMHTNJetsB0_ = ratioCalculator(ElecAccMHTNJetsB0_,ElecAccMHTNJetsB0Fail_);   
  ElecAccMHTNJetsB0_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc B_{Tags}=0; #slash{H}_{T} [GeV]; N_{Jets}");
  ElecAccMHTNJetsB0_->SetMarkerSize(2.0);
  ElecAccMHTNJetsB0_->UseCurrentStyle();
  ElecAccMHTNJetsB0_->Write();
  SaveEfficiency(ElecAccMHTNJetsB0_);
	
  ElecAccMHTNJetsB1_Inf_ = ratioCalculator(ElecAccMHTNJetsB1_Inf_,ElecAccMHTNJetsB1_InfFail_);   
  ElecAccMHTNJetsB1_Inf_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc B_{Tags}#geq1; #slash{H}_{T} [GeV]; N_{Jets}");
  ElecAccMHTNJetsB1_Inf_->SetMarkerSize(2.0);
  ElecAccMHTNJetsB1_Inf_->UseCurrentStyle();
  ElecAccMHTNJetsB1_Inf_->Write();
  SaveEfficiency(ElecAccMHTNJetsB1_Inf_);
  
  
  
  // reco
  //muon
  //1D
  MuRecoBTag_ = ratioCalculator(MuRecoBTag_,MuRecoBTagFail_);   
  MuRecoBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; B_{Tags}");
  MuRecoBTag_->SetMarkerSize(2.0);
  MuRecoBTag_->UseCurrentStyle();
  MuRecoBTag_->Write();
  SaveEfficiency(MuRecoBTag_);
  
  MuRecoNJets_ = ratioCalculator(MuRecoNJets_,MuRecoNJetsFail_);   
  MuRecoNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; N_{Jets}");
  MuRecoNJets_->SetMarkerSize(2.0);
  MuRecoNJets_->UseCurrentStyle();
  MuRecoNJets_->Write();
  SaveEfficiency(MuRecoNJets_);
  
  MuRecoHT_ = ratioCalculator(MuRecoHT_,MuRecoHTFail_);   
  MuRecoHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; H_{T} [GeV]");
  MuRecoHT_->SetMarkerSize(2.0);
  MuRecoHT_->UseCurrentStyle();
  MuRecoHT_->Write();
  SaveEfficiency(MuRecoHT_);
  
  MuRecoMHT_ = ratioCalculator(MuRecoMHT_,MuRecoMHTFail_);   
  MuRecoMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; #slash{H}_{T} [GeV]");
  MuRecoMHT_->SetMarkerSize(2.0);
  MuRecoMHT_->UseCurrentStyle();
  MuRecoMHT_->Write();
  SaveEfficiency(MuRecoMHT_);
  
  //el
  //1D
  ElecRecoBTag_ = ratioCalculator(ElecRecoBTag_,ElecRecoBTagFail_);   
  ElecRecoBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; B_{Tags}");
  ElecRecoBTag_->SetMarkerSize(2.0);
  ElecRecoBTag_->UseCurrentStyle();
  ElecRecoBTag_->Write();
  SaveEfficiency(ElecRecoBTag_);
  
  ElecRecoNJets_ = ratioCalculator(ElecRecoNJets_,ElecRecoNJetsFail_);   
  ElecRecoNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; N_{Jets}");
  ElecRecoNJets_->SetMarkerSize(2.0);
  ElecRecoNJets_->UseCurrentStyle();
  ElecRecoNJets_->Write();
  SaveEfficiency(ElecRecoNJets_);
  
  ElecRecoHT_ = ratioCalculator(ElecRecoHT_,ElecRecoHTFail_);   
  ElecRecoHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; H_{T} [GeV]");
  ElecRecoHT_->SetMarkerSize(2.0);
  ElecRecoHT_->UseCurrentStyle();
  ElecRecoHT_->Write();
  SaveEfficiency(ElecRecoHT_);
  
  ElecRecoMHT_ = ratioCalculator(ElecRecoMHT_,ElecRecoMHTFail_);   
  ElecRecoMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; #slash{H}_{T} [GeV]");
  ElecRecoMHT_->SetMarkerSize(2.0);
  ElecRecoMHT_->UseCurrentStyle();
  ElecRecoMHT_->Write();
  SaveEfficiency(ElecRecoMHT_);
  
  
  
  // iso
  //muon
  //1D
  MuIsoBTag_ = ratioCalculator(MuIsoBTag_,MuIsoBTagFail_);   
  MuIsoBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu iso; B_{Tags}");
  MuIsoBTag_->SetMarkerSize(2.0);
  MuIsoBTag_->UseCurrentStyle();
  MuIsoBTag_->Write();
  SaveEfficiency(MuIsoBTag_);
  
  MuIsoNJets_ = ratioCalculator(MuIsoNJets_,MuIsoNJetsFail_);   
  MuIsoNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu iso; N_{Jets}");
  MuIsoNJets_->SetMarkerSize(2.0);
  MuIsoNJets_->UseCurrentStyle();
  MuIsoNJets_->Write();
  SaveEfficiency(MuIsoNJets_);
  
  MuIsoHT_ = ratioCalculator(MuIsoHT_,MuIsoHTFail_);   
  MuIsoHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu iso; H_{T} [GeV]");
  MuIsoHT_->SetMarkerSize(2.0);
  MuIsoHT_->UseCurrentStyle();
  MuIsoHT_->Write();
  SaveEfficiency(MuIsoHT_);
  
  MuIsoMHT_ = ratioCalculator(MuIsoMHT_,MuIsoMHTFail_);   
  MuIsoMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu iso; #slash{H}_{T} [GeV]");
  MuIsoMHT_->SetMarkerSize(2.0);
  MuIsoMHT_->UseCurrentStyle();
  MuIsoMHT_->Write();
  SaveEfficiency(MuIsoMHT_);
  
  //el
  //1D
  ElecIsoBTag_ = ratioCalculator(ElecIsoBTag_,ElecIsoBTagFail_);   
  ElecIsoBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e iso; B_{Tags}");
  ElecIsoBTag_->SetMarkerSize(2.0);
  ElecIsoBTag_->UseCurrentStyle();
  ElecIsoBTag_->Write();
  SaveEfficiency(ElecIsoBTag_);
  
  ElecIsoNJets_ = ratioCalculator(ElecIsoNJets_,ElecIsoNJetsFail_);   
  ElecIsoNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e iso; N_{Jets}");
  ElecIsoNJets_->SetMarkerSize(2.0);
  ElecIsoNJets_->UseCurrentStyle();
  ElecIsoNJets_->Write();
  SaveEfficiency(ElecIsoNJets_);
  
  ElecIsoHT_ = ratioCalculator(ElecIsoHT_,ElecIsoHTFail_);   
  ElecIsoHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e iso; H_{T} [GeV]");
  ElecIsoHT_->SetMarkerSize(2.0);
  ElecIsoHT_->UseCurrentStyle();
  ElecIsoHT_->Write();
  SaveEfficiency(ElecIsoHT_);
  
  ElecIsoMHT_ = ratioCalculator(ElecIsoMHT_,ElecIsoMHTFail_);   
  ElecIsoMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e iso; #slash{H}_{T} [GeV]");
  ElecIsoMHT_->SetMarkerSize(2.0);
  ElecIsoMHT_->UseCurrentStyle();
  ElecIsoMHT_->Write();
  SaveEfficiency(ElecIsoMHT_);
  
  
  // m_{T}(w)
  //muon
  //1D
  MuMTWBTag_ = ratioCalculator(MuMTWBTag_,MuMTWBTagFail_);   
  MuMTWBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); B_{Tags}");
  MuMTWBTag_->SetMarkerSize(2.0);
  MuMTWBTag_->UseCurrentStyle();
  MuMTWBTag_->Write();
  SaveEfficiency(MuMTWBTag_);
  
  MuMTWNJets_ = ratioCalculator(MuMTWNJets_,MuMTWNJetsFail_);   
  MuMTWNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); N_{Jets}");
  MuMTWNJets_->SetMarkerSize(2.0);
  MuMTWNJets_->UseCurrentStyle();
  MuMTWNJets_->Write();
  SaveEfficiency(MuMTWNJets_);
  
  MuMTWHT_ = ratioCalculator(MuMTWHT_,MuMTWHTFail_);   
  MuMTWHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); H_{T} [GeV]");
  MuMTWHT_->SetMarkerSize(2.0);
  MuMTWHT_->UseCurrentStyle();
  MuMTWHT_->Write();
  SaveEfficiency(MuMTWHT_);
  
  MuMTWMHT_ = ratioCalculator(MuMTWMHT_,MuMTWMHTFail_);   
  MuMTWMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); #slash{H}_{T} [GeV]");
  MuMTWMHT_->SetMarkerSize(2.0);
  MuMTWMHT_->UseCurrentStyle();
  MuMTWMHT_->Write();
  SaveEfficiency(MuMTWMHT_);
	
  MuMTWPTActivity_ = ratioCalculator(MuMTWPTActivity_,MuMTWPTActivityFail_);   
  MuMTWPTActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); #mu p_{T} [GeV]; Activity [GeV]");
  MuMTWPTActivity_->SetMarkerSize(2.0);
  MuMTWPTActivity_->UseCurrentStyle();
  MuMTWPTActivity_->Write();
  SaveEfficiency(MuMTWPTActivity_);
  
  //muon
  //1D
  MuDiLepBTag_ = ratioCalculator(MuDiLepBTag_,MuDiLepBTagFail_);   
  MuDiLepBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; B_{Tags}");
  MuDiLepBTag_->SetMarkerSize(2.0);
  MuDiLepBTag_->UseCurrentStyle();
  MuDiLepBTag_->Write();
  SaveEfficiency(MuDiLepBTag_);
  
  MuDiLepNJets_ = ratioCalculator(MuDiLepNJets_,MuDiLepNJetsFail_);   
  MuDiLepNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; N_{Jets}");
  MuDiLepNJets_->SetMarkerSize(2.0);
  MuDiLepNJets_->UseCurrentStyle();
  MuDiLepNJets_->Write();
  SaveEfficiency(MuDiLepNJets_);
  
  MuDiLepHT_ = ratioCalculator(MuDiLepHT_,MuDiLepHTFail_);   
  MuDiLepHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; H_{T} [GeV]");
  MuDiLepHT_->SetMarkerSize(2.0);
  MuDiLepHT_->UseCurrentStyle();
  MuDiLepHT_->Write();
  SaveEfficiency(MuDiLepHT_);
  
  MuDiLepMHT_ = ratioCalculator(MuDiLepMHT_,MuDiLepMHTFail_);   
  MuDiLepMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; #slash{H}_{T} [GeV]");
  MuDiLepMHT_->SetMarkerSize(2.0);
  MuDiLepMHT_->UseCurrentStyle();
  MuDiLepMHT_->Write();
  SaveEfficiency(MuDiLepMHT_);
  
  MuDiLepMTWBTag_ = ratioCalculator(MuDiLepMTWBTag_,MuDiLepMTWBTagFail_);   
  MuDiLepMTWBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; B_{Tags}");
  MuDiLepMTWBTag_->SetMarkerSize(2.0);
  MuDiLepMTWBTag_->UseCurrentStyle();
  MuDiLepMTWBTag_->Write();
  SaveEfficiency(MuDiLepMTWBTag_);
  
  MuDiLepMTWNJets_ = ratioCalculator(MuDiLepMTWNJets_,MuDiLepMTWNJetsFail_);   
  MuDiLepMTWNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; N_{Jets}");
  MuDiLepMTWNJets_->SetMarkerSize(2.0);
  MuDiLepMTWNJets_->UseCurrentStyle();
  MuDiLepMTWNJets_->Write();
  SaveEfficiency(MuDiLepMTWNJets_);
  
  MuDiLepMTWHT_ = ratioCalculator(MuDiLepMTWHT_,MuDiLepMTWHTFail_);   
  MuDiLepMTWHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; H_{T} [GeV]");
  MuDiLepMTWHT_->SetMarkerSize(2.0);
  MuDiLepMTWHT_->UseCurrentStyle();
  MuDiLepMTWHT_->Write();
  SaveEfficiency(MuDiLepMTWHT_);
  
  MuDiLepMTWMHT_ = ratioCalculator(MuDiLepMTWMHT_,MuDiLepMTWMHTFail_);   
  MuDiLepMTWMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep; #slash{H}_{T} [GeV]");
  MuDiLepMTWMHT_->SetMarkerSize(2.0);
  MuDiLepMTWMHT_->UseCurrentStyle();
  MuDiLepMTWMHT_->Write();
  SaveEfficiency(MuDiLepMTWMHT_);
  
  //muon
  //1D
  MuDiLepContributionBTag_ = ratioCalculator(MuDiLepContributionBTag_,MuDiLepContributionBTagFail_);   
  MuDiLepContributionBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; B_{Tags}");
  MuDiLepContributionBTag_->SetMarkerSize(2.0);
  MuDiLepContributionBTag_->UseCurrentStyle();
  MuDiLepContributionBTag_->Write();
  SaveEfficiency(MuDiLepContributionBTag_);
  
  MuDiLepContributionNJets_ = ratioCalculator(MuDiLepContributionNJets_,MuDiLepContributionNJetsFail_);   
  MuDiLepContributionNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; N_{Jets}");
  MuDiLepContributionNJets_->SetMarkerSize(2.0);
  MuDiLepContributionNJets_->UseCurrentStyle();
  MuDiLepContributionNJets_->Write();
  SaveEfficiency(MuDiLepContributionNJets_);
  
  MuDiLepContributionHT_ = ratioCalculator(MuDiLepContributionHT_,MuDiLepContributionHTFail_);   
  MuDiLepContributionHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; H_{T} [GeV]");
  MuDiLepContributionHT_->SetMarkerSize(2.0);
  MuDiLepContributionHT_->UseCurrentStyle();
  MuDiLepContributionHT_->Write();
  SaveEfficiency(MuDiLepContributionHT_);
  
  MuDiLepContributionMHT_ = ratioCalculator(MuDiLepContributionMHT_,MuDiLepContributionMHTFail_);   
  MuDiLepContributionMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; #slash{H}_{T} [GeV]");
  MuDiLepContributionMHT_->SetMarkerSize(2.0);
  MuDiLepContributionMHT_->UseCurrentStyle();
  MuDiLepContributionMHT_->Write();
  SaveEfficiency(MuDiLepContributionMHT_);
  
  MuDiLepContributionMTWBTag_ = ratioCalculator(MuDiLepContributionMTWBTag_,MuDiLepContributionMTWBTagFail_);   
  MuDiLepContributionMTWBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; B_{Tags}");
  MuDiLepContributionMTWBTag_->SetMarkerSize(2.0);
  MuDiLepContributionMTWBTag_->UseCurrentStyle();
  MuDiLepContributionMTWBTag_->Write();
  SaveEfficiency(MuDiLepContributionMTWBTag_);
  
  MuDiLepContributionMTWNJets_ = ratioCalculator(MuDiLepContributionMTWNJets_,MuDiLepContributionMTWNJetsFail_);   
  MuDiLepContributionMTWNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; N_{Jets}");
  MuDiLepContributionMTWNJets_->SetMarkerSize(2.0);
  MuDiLepContributionMTWNJets_->UseCurrentStyle();
  MuDiLepContributionMTWNJets_->Write();
  SaveEfficiency(MuDiLepContributionMTWNJets_);
  
  MuDiLepContributionMTWHT_ = ratioCalculator(MuDiLepContributionMTWHT_,MuDiLepContributionMTWHTFail_);   
  MuDiLepContributionMTWHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; H_{T} [GeV]");
  MuDiLepContributionMTWHT_->SetMarkerSize(2.0);
  MuDiLepContributionMTWHT_->UseCurrentStyle();
  MuDiLepContributionMTWHT_->Write();
  SaveEfficiency(MuDiLepContributionMTWHT_);
  
  MuDiLepContributionMTWMHT_ = ratioCalculator(MuDiLepContributionMTWMHT_,MuDiLepContributionMTWMHTFail_);   
  MuDiLepContributionMTWMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu di lep contri contri; #slash{H}_{T} [GeV]");
  MuDiLepContributionMTWMHT_->SetMarkerSize(2.0);
  MuDiLepContributionMTWMHT_->UseCurrentStyle();
  MuDiLepContributionMTWMHT_->Write();
  SaveEfficiency(MuDiLepContributionMTWMHT_);
  
  
  
  //el
  //1D
  ElecMTWBTag_ = ratioCalculator(ElecMTWBTag_,ElecMTWBTagFail_);   
  ElecMTWBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); B_{Tags}");
  ElecMTWBTag_->SetMarkerSize(2.0);
  ElecMTWBTag_->UseCurrentStyle();
  ElecMTWBTag_->Write();
  SaveEfficiency(ElecMTWBTag_);
  
  ElecMTWNJets_ = ratioCalculator(ElecMTWNJets_,ElecMTWNJetsFail_);   
  ElecMTWNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); N_{Jets}");
  ElecMTWNJets_->SetMarkerSize(2.0);
  ElecMTWNJets_->UseCurrentStyle();
  ElecMTWNJets_->Write();
  SaveEfficiency(ElecMTWNJets_);
  
  ElecMTWHT_ = ratioCalculator(ElecMTWHT_,ElecMTWHTFail_);   
  ElecMTWHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); H_{T} [GeV]");
  ElecMTWHT_->SetMarkerSize(2.0);
  ElecMTWHT_->UseCurrentStyle();
  ElecMTWHT_->Write();
  SaveEfficiency(ElecMTWHT_);
  
  ElecMTWMHT_ = ratioCalculator(ElecMTWMHT_,ElecMTWMHTFail_);   
  ElecMTWMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); #slash{H}_{T} [GeV]");
  ElecMTWMHT_->SetMarkerSize(2.0);
  ElecMTWMHT_->UseCurrentStyle();
  ElecMTWMHT_->Write();
  SaveEfficiency(ElecMTWMHT_);
  
  ElecDiLepBTag_ = ratioCalculator(ElecDiLepBTag_,ElecDiLepBTagFail_);   
  ElecDiLepBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; B_{Tags}");
  ElecDiLepBTag_->SetMarkerSize(2.0);
  ElecDiLepBTag_->UseCurrentStyle();
  ElecDiLepBTag_->Write();
  SaveEfficiency(ElecDiLepBTag_);
  
  ElecDiLepNJets_ = ratioCalculator(ElecDiLepNJets_,ElecDiLepNJetsFail_);   
  ElecDiLepNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; N_{Jets}");
  ElecDiLepNJets_->SetMarkerSize(2.0);
  ElecDiLepNJets_->UseCurrentStyle();
  ElecDiLepNJets_->Write();
  SaveEfficiency(ElecDiLepNJets_);
  
  ElecDiLepHT_ = ratioCalculator(ElecDiLepHT_,ElecDiLepHTFail_);   
  ElecDiLepHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; H_{T} [GeV]");
  ElecDiLepHT_->SetMarkerSize(2.0);
  ElecDiLepHT_->UseCurrentStyle();
  ElecDiLepHT_->Write();
  SaveEfficiency(ElecDiLepHT_);
  
  ElecDiLepMHT_ = ratioCalculator(ElecDiLepMHT_,ElecDiLepMHTFail_);   
  ElecDiLepMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; #slash{H}_{T} [GeV]");
  ElecDiLepMHT_->SetMarkerSize(2.0);
  ElecDiLepMHT_->UseCurrentStyle();
  ElecDiLepMHT_->Write();
  SaveEfficiency(ElecDiLepMHT_);
  
  ElecDiLepMTWBTag_ = ratioCalculator(ElecDiLepMTWBTag_,ElecDiLepMTWBTagFail_);   
  ElecDiLepMTWBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; B_{Tags}");
  ElecDiLepMTWBTag_->SetMarkerSize(2.0);
  ElecDiLepMTWBTag_->UseCurrentStyle();
  ElecDiLepMTWBTag_->Write();
  SaveEfficiency(ElecDiLepMTWBTag_);
  
  ElecDiLepMTWNJets_ = ratioCalculator(ElecDiLepMTWNJets_,ElecDiLepMTWNJetsFail_);   
  ElecDiLepMTWNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; N_{Jets}");
  ElecDiLepMTWNJets_->SetMarkerSize(2.0);
  ElecDiLepMTWNJets_->UseCurrentStyle();
  ElecDiLepMTWNJets_->Write();
  SaveEfficiency(ElecDiLepMTWNJets_);
  
  ElecDiLepMTWHT_ = ratioCalculator(ElecDiLepMTWHT_,ElecDiLepMTWHTFail_);   
  ElecDiLepMTWHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; H_{T} [GeV]");
  ElecDiLepMTWHT_->SetMarkerSize(2.0);
  ElecDiLepMTWHT_->UseCurrentStyle();
  ElecDiLepMTWHT_->Write();
  SaveEfficiency(ElecDiLepMTWHT_);
  
  ElecDiLepMTWMHT_ = ratioCalculator(ElecDiLepMTWMHT_,ElecDiLepMTWMHTFail_);   
  ElecDiLepMTWMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep; #slash{H}_{T} [GeV]");
  ElecDiLepMTWMHT_->SetMarkerSize(2.0);
  ElecDiLepMTWMHT_->UseCurrentStyle();
  ElecDiLepMTWMHT_->Write();
  SaveEfficiency(ElecDiLepMTWMHT_);
	
  ElecMTWPTActivity_ = ratioCalculator(ElecMTWPTActivity_,ElecMTWPTActivityFail_);   
  ElecMTWPTActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); e p_{T} [GeV]; Activity [GeV]");
  ElecMTWPTActivity_->SetMarkerSize(2.0);
  ElecMTWPTActivity_->UseCurrentStyle();
  ElecMTWPTActivity_->Write();
  SaveEfficiency(ElecMTWPTActivity_);
  
  //muon
  //1D
  ElecDiLepContributionBTag_ = ratioCalculator(ElecDiLepContributionBTag_,ElecDiLepContributionBTagFail_);   
  ElecDiLepContributionBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; B_{Tags}");
  ElecDiLepContributionBTag_->SetMarkerSize(2.0);
  ElecDiLepContributionBTag_->UseCurrentStyle();
  ElecDiLepContributionBTag_->Write();
  SaveEfficiency(ElecDiLepContributionBTag_);
  
  ElecDiLepContributionNJets_ = ratioCalculator(ElecDiLepContributionNJets_,ElecDiLepContributionNJetsFail_);   
  ElecDiLepContributionNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; N_{Jets}");
  ElecDiLepContributionNJets_->SetMarkerSize(2.0);
  ElecDiLepContributionNJets_->UseCurrentStyle();
  ElecDiLepContributionNJets_->Write();
  SaveEfficiency(ElecDiLepContributionNJets_);
  
  ElecDiLepContributionHT_ = ratioCalculator(ElecDiLepContributionHT_,ElecDiLepContributionHTFail_);   
  ElecDiLepContributionHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; H_{T} [GeV]");
  ElecDiLepContributionHT_->SetMarkerSize(2.0);
  ElecDiLepContributionHT_->UseCurrentStyle();
  ElecDiLepContributionHT_->Write();
  SaveEfficiency(ElecDiLepContributionHT_);
  
  ElecDiLepContributionMHT_ = ratioCalculator(ElecDiLepContributionMHT_,ElecDiLepContributionMHTFail_);   
  ElecDiLepContributionMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; #slash{H}_{T} [GeV]");
  ElecDiLepContributionMHT_->SetMarkerSize(2.0);
  ElecDiLepContributionMHT_->UseCurrentStyle();
  ElecDiLepContributionMHT_->Write();
  SaveEfficiency(ElecDiLepContributionMHT_);
  
  ElecDiLepContributionMTWBTag_ = ratioCalculator(ElecDiLepContributionMTWBTag_,ElecDiLepContributionMTWBTagFail_);   
  ElecDiLepContributionMTWBTag_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; B_{Tags}");
  ElecDiLepContributionMTWBTag_->SetMarkerSize(2.0);
  ElecDiLepContributionMTWBTag_->UseCurrentStyle();
  ElecDiLepContributionMTWBTag_->Write();
  SaveEfficiency(ElecDiLepContributionMTWBTag_);
  
  ElecDiLepContributionMTWNJets_ = ratioCalculator(ElecDiLepContributionMTWNJets_,ElecDiLepContributionMTWNJetsFail_);   
  ElecDiLepContributionMTWNJets_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; N_{Jets}");
  ElecDiLepContributionMTWNJets_->SetMarkerSize(2.0);
  ElecDiLepContributionMTWNJets_->UseCurrentStyle();
  ElecDiLepContributionMTWNJets_->Write();
  SaveEfficiency(ElecDiLepContributionMTWNJets_);
  
  ElecDiLepContributionMTWHT_ = ratioCalculator(ElecDiLepContributionMTWHT_,ElecDiLepContributionMTWHTFail_);   
  ElecDiLepContributionMTWHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; H_{T} [GeV]");
  ElecDiLepContributionMTWHT_->SetMarkerSize(2.0);
  ElecDiLepContributionMTWHT_->UseCurrentStyle();
  ElecDiLepContributionMTWHT_->Write();
  SaveEfficiency(ElecDiLepContributionMTWHT_);
  
  ElecDiLepContributionMTWMHT_ = ratioCalculator(ElecDiLepContributionMTWMHT_,ElecDiLepContributionMTWMHTFail_);   
  ElecDiLepContributionMTWMHT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e di lep contri contri; #slash{H}_{T} [GeV]");
  ElecDiLepContributionMTWMHT_->SetMarkerSize(2.0);
  ElecDiLepContributionMTWMHT_->UseCurrentStyle();
  ElecDiLepContributionMTWMHT_->Write();
  SaveEfficiency(ElecDiLepContributionMTWMHT_);

  // pt and activity
  
  MuPurityPT_ = ratioCalculator(MuPurityPT_,MuPurityPTFail_);   
  MuPurityPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; #mu p_{T} [GeV]");
  MuPurityPT_->SetMarkerSize(2.0);
  MuPurityPT_->UseCurrentStyle();
  MuPurityPT_->Write();
  SaveEfficiency(MuPurityPT_);
  
  MuPurityActivity_ = ratioCalculator(MuPurityActivity_,MuPurityActivityFail_);   
  MuPurityActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu purity; Activity [GeV]");
  MuPurityActivity_->SetMarkerSize(2.0);
  MuPurityActivity_->UseCurrentStyle();
  MuPurityActivity_->Write();
  SaveEfficiency(MuPurityActivity_);
  
  
  MuAccPT_ = ratioCalculator(MuAccPT_,MuAccPTFail_);   
  MuAccPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; #mu p_{T} [GeV]");
  MuAccPT_->SetMarkerSize(2.0);
  MuAccPT_->UseCurrentStyle();
  MuAccPT_->Write();
  SaveEfficiency(MuAccPT_);
  
  MuAccActivity_ = ratioCalculator(MuAccActivity_,MuAccActivityFail_);   
  MuAccActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu acc; Activity [GeV]");
  MuAccActivity_->SetMarkerSize(2.0);
  MuAccActivity_->UseCurrentStyle();
  MuAccActivity_->Write();
  SaveEfficiency(MuAccActivity_);
  
  
  MuRecoPT_ = ratioCalculator(MuRecoPT_,MuRecoPTFail_);   
  MuRecoPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; #mu p_{T} [GeV]");
  MuRecoPT_->SetMarkerSize(2.0);
  MuRecoPT_->UseCurrentStyle();
  MuRecoPT_->Write();
  SaveEfficiency(MuRecoPT_);
  
  MuRecoActivity_ = ratioCalculator(MuRecoActivity_,MuRecoActivityFail_);   
  MuRecoActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; Activity [GeV]");
  MuRecoActivity_->SetMarkerSize(2.0);
  MuRecoActivity_->UseCurrentStyle();
  MuRecoActivity_->Write();
  SaveEfficiency(MuRecoActivity_);
	
  MuRecoPTActivity_ = ratioCalculator(MuRecoPTActivity_,MuRecoPTActivityFail_);   
  MuRecoPTActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu reco; #mu p_{T} [GeV];Activity [GeV]");
  MuRecoPTActivity_->SetMarkerSize(2.0);
  MuRecoPTActivity_->UseCurrentStyle();
  MuRecoPTActivity_->Write();
  SaveEfficiency(MuRecoPTActivity_);
  
  
  MuIsoPT_ = ratioCalculator(MuIsoPT_,MuIsoPTFail_);   
  MuIsoPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu iso; #mu p_{T} [GeV]");
  MuIsoPT_->SetMarkerSize(2.0);
  MuIsoPT_->UseCurrentStyle();
  MuIsoPT_->Write();
  SaveEfficiency(MuIsoPT_);
  
  MuIsoActivity_ = ratioCalculator(MuIsoActivity_,MuIsoActivityFail_);   
  MuIsoActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu iso; Activity [GeV]");
  MuIsoActivity_->SetMarkerSize(2.0);
  MuIsoActivity_->UseCurrentStyle();
  MuIsoActivity_->Write();
  SaveEfficiency(MuIsoActivity_);
  
  
  MuMTWPT_ = ratioCalculator(MuMTWPT_,MuMTWPTFail_);   
  MuMTWPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); #mu p_{T} [GeV]");
  MuMTWPT_->SetMarkerSize(2.0);
  MuMTWPT_->UseCurrentStyle();
  MuMTWPT_->Write();
  SaveEfficiency(MuMTWPT_);
  
  MuMTWActivity_ = ratioCalculator(MuMTWActivity_,MuMTWActivityFail_);   
  MuMTWActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu m_{T}(w); Activity [GeV]");
  MuMTWActivity_->SetMarkerSize(2.0);
  MuMTWActivity_->UseCurrentStyle();
  MuMTWActivity_->Write();
  SaveEfficiency(MuMTWActivity_);
  
  MuIsoPTActivity_ = ratioCalculator(MuIsoPTActivity_,MuIsoPTActivityFail_);   
  MuIsoPTActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV #mu Iso; #mu p_{T}[GeV]; Activity [GeV]");
  MuIsoPTActivity_->SetMarkerSize(2.0);
  MuIsoPTActivity_->UseCurrentStyle();
  MuIsoPTActivity_->Write();
  SaveEfficiency(MuIsoPTActivity_);
  
  
  
  ElecPurityPT_ = ratioCalculator(ElecPurityPT_,ElecPurityPTFail_);   
  ElecPurityPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; e p_{T} [GeV]");
  ElecPurityPT_->SetMarkerSize(2.0);
  ElecPurityPT_->UseCurrentStyle();
  ElecPurityPT_->Write();
  SaveEfficiency(ElecPurityPT_);
  
  ElecPurityActivity_ = ratioCalculator(ElecPurityActivity_,ElecPurityActivityFail_);   
  ElecPurityActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e purity; Activity [GeV]");
  ElecPurityActivity_->SetMarkerSize(2.0);
  ElecPurityActivity_->UseCurrentStyle();
  ElecPurityActivity_->Write();
  SaveEfficiency(ElecPurityActivity_);
  
  
  ElecAccPT_ = ratioCalculator(ElecAccPT_,ElecAccPTFail_);   
  ElecAccPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; e p_{T} [GeV]");
  ElecAccPT_->SetMarkerSize(2.0);
  ElecAccPT_->UseCurrentStyle();
  ElecAccPT_->Write();
  SaveEfficiency(ElecAccPT_);
  
  ElecAccActivity_ = ratioCalculator(ElecAccActivity_,ElecAccActivityFail_);   
  ElecAccActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e acc; Activity [GeV]");
  ElecAccActivity_->SetMarkerSize(2.0);
  ElecAccActivity_->UseCurrentStyle();
  ElecAccActivity_->Write();
  SaveEfficiency(ElecAccActivity_);
  
  
  ElecRecoPT_ = ratioCalculator(ElecRecoPT_,ElecRecoPTFail_);   
  ElecRecoPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; e p_{T} [GeV]");
  ElecRecoPT_->SetMarkerSize(2.0);
  ElecRecoPT_->UseCurrentStyle();
  ElecRecoPT_->Write();
  SaveEfficiency(ElecRecoPT_);
  
  ElecRecoActivity_ = ratioCalculator(ElecRecoActivity_,ElecRecoActivityFail_);   
  ElecRecoActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; Activity [GeV]");
  ElecRecoActivity_->SetMarkerSize(2.0);
  ElecRecoActivity_->UseCurrentStyle();
  ElecRecoActivity_->Write();
  SaveEfficiency(ElecRecoActivity_);
	
  ElecRecoPTActivity_ = ratioCalculator(ElecRecoPTActivity_,ElecRecoPTActivityFail_);   
  ElecRecoPTActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e reco; e p_{T} [GeV];Activity [GeV]");
  ElecRecoPTActivity_->SetMarkerSize(2.0);
  ElecRecoPTActivity_->UseCurrentStyle();
  ElecRecoPTActivity_->Write();
  SaveEfficiency(ElecRecoPTActivity_);
  
  
  ElecIsoPT_ = ratioCalculator(ElecIsoPT_,ElecIsoPTFail_);   
  ElecIsoPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e iso; e p_{T} [GeV]");
  ElecIsoPT_->SetMarkerSize(2.0);
  ElecIsoPT_->UseCurrentStyle();
  ElecIsoPT_->Write();
  SaveEfficiency(ElecIsoPT_);
  TEfficiency* tempTeff =  ratioCalculatorTEff(ElecIsoPT_,ElecIsoPTFail_);
  tempTeff->SetName("ElecIsoPTTEff");
  tempTeff->Write();
  
  ElecIsoActivity_ = ratioCalculator(ElecIsoActivity_,ElecIsoActivityFail_);   
  ElecIsoActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e iso; Activity [GeV]");
  ElecIsoActivity_->SetMarkerSize(2.0);
  ElecIsoActivity_->UseCurrentStyle();
  ElecIsoActivity_->Write();
  SaveEfficiency(ElecIsoActivity_);
  
  
  ElecMTWPT_ = ratioCalculator(ElecMTWPT_,ElecMTWPTFail_);   
  ElecMTWPT_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); e p_{T} [GeV]");
  ElecMTWPT_->SetMarkerSize(2.0);
  ElecMTWPT_->UseCurrentStyle();
  ElecMTWPT_->Write();
  SaveEfficiency(ElecMTWPT_);
  
  ElecMTWActivity_ = ratioCalculator(ElecMTWActivity_,ElecMTWActivityFail_);   
  ElecMTWActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e m_{T}(w); Activity [GeV]");
  ElecMTWActivity_->SetMarkerSize(2.0);
  ElecMTWActivity_->UseCurrentStyle();
  ElecMTWActivity_->Write();
  SaveEfficiency(ElecMTWActivity_);
  
  ElecIsoPTActivity_ = ratioCalculator(ElecIsoPTActivity_,ElecIsoPTActivityFail_);   
  ElecIsoPTActivity_->SetTitle("Simulation, #int Ldt = 10^{-1}, #sqrt{s}=13 TeV e Iso; e p_{T}[GeV]; Activity [GeV]");
  ElecIsoPTActivity_->SetMarkerSize(2.0);
  ElecIsoPTActivity_->UseCurrentStyle();
  ElecIsoPTActivity_->Write();
  SaveEfficiency(ElecIsoPTActivity_);

  // storing TEfficiency objectes in the file these ones should compute the efficiencies and also the errors correctly
  cout << "Storing TEfficiencies in " << outPutFile->GetName() << endl;	
  outPutFile->cd();
  outPutFile->mkdir("TEfficiencies");
  TDirectory *dTEfficiencies = (TDirectory*)outPutFile->Get("TEfficiencies");
  dTEfficiencies->cd();
	
  // TEfficiencies used efficiencies
  MuPurityBTagEff_->saveResults(dTEfficiencies);
  MuPurityNJetsEff_->saveResults(dTEfficiencies);
  MuPurityHTEff_->saveResults(dTEfficiencies);
  MuPurityMHTEff_->saveResults(dTEfficiencies);
  MuPurityPTEff_->saveResults(dTEfficiencies);
  MuPurityActivityEff_->saveResults(dTEfficiencies);
	
  MuonPurityMHTNJetEff_->saveResults(dTEfficiencies);
	
	
  ElecPurityBTagEff_->saveResults(dTEfficiencies);
  ElecPurityNJetsEff_->saveResults(dTEfficiencies);
  ElecPurityHTEff_->saveResults(dTEfficiencies);
  ElecPurityMHTEff_->saveResults(dTEfficiencies);
  ElecPurityPTEff_->saveResults(dTEfficiencies);
  ElecPurityActivityEff_->saveResults(dTEfficiencies);
	
  ElecPurityMHTNJetEff_->saveResults(dTEfficiencies);
	
  MuAccBTagEff_->saveResults(dTEfficiencies);
  MuAccNJetsEff_->saveResults(dTEfficiencies);
  MuAccHTEff_->saveResults(dTEfficiencies);
  MuAccMHTEff_->saveResults(dTEfficiencies);
  MuAccPTEff_->saveResults(dTEfficiencies);
  MuAccActivityEff_->saveResults(dTEfficiencies);
	
  MuAccHTNJetsEff_->saveResults(dTEfficiencies);
  MuAccMHTNJetsEff_->saveResults(dTEfficiencies);
  MuAccBTagNJetsEff_->saveResults(dTEfficiencies);
	
	
  ElecAccBTagEff_->saveResults(dTEfficiencies);
  ElecAccNJetsEff_->saveResults(dTEfficiencies);
  ElecAccHTEff_->saveResults(dTEfficiencies);
  ElecAccMHTEff_->saveResults(dTEfficiencies);
  ElecAccPTEff_->saveResults(dTEfficiencies);
  ElecAccActivityEff_->saveResults(dTEfficiencies);
	
  ElecAccHTNJetsEff_->saveResults(dTEfficiencies);
  ElecAccMHTNJetsEff_->saveResults(dTEfficiencies);
  ElecAccBTagNJetsEff_->saveResults(dTEfficiencies);
	
	
  MuRecoBTagEff_->saveResults(dTEfficiencies);
  MuRecoNJetsEff_->saveResults(dTEfficiencies);
  MuRecoHTEff_->saveResults(dTEfficiencies);
  MuRecoMHTEff_->saveResults(dTEfficiencies);
  MuRecoPTEff_->saveResults(dTEfficiencies);
  MuRecoActivityEff_->saveResults(dTEfficiencies);
	
  MuRecoPTActivityEff_->saveResults(dTEfficiencies);
	
	
  ElecRecoBTagEff_->saveResults(dTEfficiencies);
  ElecRecoNJetsEff_->saveResults(dTEfficiencies);
  ElecRecoHTEff_->saveResults(dTEfficiencies);
  ElecRecoMHTEff_->saveResults(dTEfficiencies);
  ElecRecoPTEff_->saveResults(dTEfficiencies);
  ElecRecoActivityEff_->saveResults(dTEfficiencies);
	
  ElecRecoPTActivityEff_->saveResults(dTEfficiencies);
	
	
  MuIsoBTagEff_->saveResults(dTEfficiencies);
  MuIsoNJetsEff_->saveResults(dTEfficiencies);
  MuIsoHTEff_->saveResults(dTEfficiencies);
  MuIsoMHTEff_->saveResults(dTEfficiencies);
  MuIsoPTEff_->saveResults(dTEfficiencies);
  MuIsoActivityEff_->saveResults(dTEfficiencies);
	
  MuIsoPTActivityEff_->saveResults(dTEfficiencies);
	
	
  ElecIsoBTagEff_->saveResults(dTEfficiencies);
  ElecIsoNJetsEff_->saveResults(dTEfficiencies);
  ElecIsoHTEff_->saveResults(dTEfficiencies);
  ElecIsoMHTEff_->saveResults(dTEfficiencies);
  ElecIsoPTEff_->saveResults(dTEfficiencies);
  ElecIsoActivityEff_->saveResults(dTEfficiencies);
	
  ElecIsoPTActivityEff_->saveResults(dTEfficiencies);
	
	
  MuMTWBTagEff_->saveResults(dTEfficiencies);
  MuMTWNJetsEff_->saveResults(dTEfficiencies);
  MuMTWHTEff_->saveResults(dTEfficiencies);
  MuMTWMHTEff_->saveResults(dTEfficiencies);
  MuMTWPTEff_->saveResults(dTEfficiencies);
  MuMTWActivityEff_->saveResults(dTEfficiencies);
	
  MuMTWPTActivityEff_->saveResults(dTEfficiencies);
	
	
	
  ElecMTWBTagEff_->saveResults(dTEfficiencies);
  ElecMTWNJetsEff_->saveResults(dTEfficiencies);
  ElecMTWHTEff_->saveResults(dTEfficiencies);
  ElecMTWMHTEff_->saveResults(dTEfficiencies);
  ElecMTWPTEff_->saveResults(dTEfficiencies);
  ElecMTWActivityEff_->saveResults(dTEfficiencies);
	
  ElecMTWPTActivityEff_->saveResults(dTEfficiencies);
	
	
	
  MuDiLepBTagEff_->saveResults(dTEfficiencies);
  MuDiLepNJetsEff_->saveResults(dTEfficiencies);
  MuDiLepHTEff_->saveResults(dTEfficiencies);
  MuDiLepMHTEff_->saveResults(dTEfficiencies);
  // 	MuDiLepPTEff_->saveResults(dTEfficiencies);
  // 	MuDiLepActivityEff_->saveResults(dTEfficiencies);
	
	
  ElecDiLepBTagEff_->saveResults(dTEfficiencies);
  ElecDiLepNJetsEff_->saveResults(dTEfficiencies);
  ElecDiLepHTEff_->saveResults(dTEfficiencies);
  ElecDiLepMHTEff_->saveResults(dTEfficiencies);
  // 	ElecDiLepPTEff_->saveResults(dTEfficiencies);
  // 	ElecDiLepActivityEff_->saveResults(dTEfficiencies);
	
	
  MuDiLepMTWBTagEff_->saveResults(dTEfficiencies);
  MuDiLepMTWNJetsEff_->saveResults(dTEfficiencies);
  MuDiLepMTWHTEff_->saveResults(dTEfficiencies);
  MuDiLepMTWMHTEff_->saveResults(dTEfficiencies);
  // 	MuDiLepMTWPTEff_->saveResults(dTEfficiencies);
  // 	MuDiLepMTWActivityEff_->saveResults(dTEfficiencies);
	
	
  ElecDiLepMTWBTagEff_->saveResults(dTEfficiencies);
  ElecDiLepMTWNJetsEff_->saveResults(dTEfficiencies);
  ElecDiLepMTWHTEff_->saveResults(dTEfficiencies);
  ElecDiLepMTWMHTEff_->saveResults(dTEfficiencies);
  // 	ElecDiLepMTWPTEff_->saveResults(dTEfficiencies);
  // 	ElecDiLepMTWActivityEff_->saveResults(dTEfficiencies);
	
	
  MuDiLepContributionBTagEff_->saveResults(dTEfficiencies);
  MuDiLepContributionNJetsEff_->saveResults(dTEfficiencies);
  MuDiLepContributionHTEff_->saveResults(dTEfficiencies);
  MuDiLepContributionMHTEff_->saveResults(dTEfficiencies);
	
	
  ElecDiLepContributionBTagEff_->saveResults(dTEfficiencies);
  ElecDiLepContributionNJetsEff_->saveResults(dTEfficiencies);
  ElecDiLepContributionHTEff_->saveResults(dTEfficiencies);
  ElecDiLepContributionMHTEff_->saveResults(dTEfficiencies);
	
	
  MuDiLepContributionMTWBTagEff_->saveResults(dTEfficiencies);
  MuDiLepContributionMTWNJetsEff_->saveResults(dTEfficiencies);
  MuDiLepContributionMTWHTEff_->saveResults(dTEfficiencies);
  MuDiLepContributionMTWMHTEff_->saveResults(dTEfficiencies);
	
	
  ElecDiLepContributionMTWBTagEff_->saveResults(dTEfficiencies);
  ElecDiLepContributionMTWNJetsEff_->saveResults(dTEfficiencies);
  ElecDiLepContributionMTWHTEff_->saveResults(dTEfficiencies);
  ElecDiLepContributionMTWMHTEff_->saveResults(dTEfficiencies);

  outPutFile->cd();
  outPutFile->mkdir("TEfficienciesSearchBins");
  TDirectory *dTEfficienciesSearchBins = (TDirectory*)outPutFile->Get("TEfficienciesSearchBins");
  dTEfficienciesSearchBins->cd();
  MuAccSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  cout << "JERR" << endl;
  MuRecoSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  MuIsoSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  MuMTWSearchBinEff_->saveResults(dTEfficienciesSearchBins); 
  MuDiLepContributionMTWAppliedSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  MuDiLepEffMTWAppliedSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  MuPuritySearchBinEff_->saveResults(dTEfficienciesSearchBins);
	
  ElecAccSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  ElecRecoSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  ElecIsoSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  ElecMTWSearchBinEff_->saveResults(dTEfficienciesSearchBins); 
  ElecDiLepContributionMTWAppliedSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  ElecDiLepEffMTWAppliedSearchBinEff_->saveResults(dTEfficienciesSearchBins);
  ElecPuritySearchBinEff_->saveResults(dTEfficienciesSearchBins);
	
  //	ExpectationReductionIsoTrack_->saveResults(dTEfficienciesSearchBins);
	
	
	
  // 	MuonPurityMHTNJetEff_->GetEfficiency()->Write();
  outPutFile->cd();
  outPutFile->mkdir("Expectation");
  TDirectory *dExpectation = (TDirectory*)outPutFile->Get("Expectation");
  dExpectation->cd();
  totalExpectation_->saveResults(dExpectation);
  
  outPutFile->Close();
}

TH2D* RTree::ratioCalculator(TH2D* passTH2, TH2D* failTH2)
{
  // passTH2->Sumw2();
  TH2D *sum = (TH2D*)passTH2->Clone();
  // failTH2->Sumw2();
  
  sum->Add(failTH2);
  passTH2->Divide(passTH2,sum,1,1,"B");
  return passTH2;
}
TH1D* RTree::ratioCalculator(TH1D* passTH1, TH1D* failTH1)
{
  passTH1->Sumw2();
  TH1D *sum = (TH1D*)passTH1->Clone();
  failTH1->Sumw2();
	
  sum->Add(failTH1);
  passTH1->Divide(passTH1,sum,1,1,"B");
  return passTH1;
}
TEfficiency* RTree::ratioCalculatorTEff(TH1D* passTH1, TH1D* failTH1)
{
  TEfficiency* pEff = 0;
  passTH1->Sumw2();
  TH1D *sum = (TH1D*)passTH1->Clone();
  failTH1->Sumw2();
	
  sum->Add(failTH1);
  if(TEfficiency::CheckConsistency(*passTH1,*sum, "w"))
    {
      pEff = new TEfficiency(*passTH1,*sum);
    }
  else std::cout<<"Error in ratioCalculatorTEff TH1D passing and sum not consistend!"<<std::endl;
  return pEff;
}

TEfficiency* RTree::ratioCalculatorTEff(TH2D* passTH2, TH2D* failTH2)
{
  TEfficiency* pEff = 0;
  // passTH2->Sumw2();
  TH2D *sum = (TH2D*)passTH2->Clone();
  // failTH2->Sumw2();
	
  sum->Add(failTH2);
  passTH2->Divide(passTH2,sum,1,1,"B");
  return pEff;
}

void RTree::SaveEfficiency(TH2D *input, TString outdir)
{
  gROOT->SetBatch(true);
  const TString th2Name = input->GetName();
  const TString th2Title = input->GetTitle();
  TCanvas *c1 = new TCanvas(th2Name,th2Title,1);
  c1->cd();
  c1->SetLogx();
  c1->SetLogy();
  input->SetMarkerSize(2.0);
  input->UseCurrentStyle();
  input->SetMinimum(0.);
  input->SetMaximum(1.);
  input->GetZaxis()->SetLimits(0.,100.);
  input->Draw("ColZ,Text,E");
  c1->Update();

  // c1->SaveAs(s+"MuonAccEff3"+".png");
  if(saveEffToPDF_) c1->SaveAs(outdir+th2Name+".pdf");
  if(saveEffToPNG_) c1->SaveAs(outdir+th2Name+".png");
  delete c1;
  gROOT->SetBatch(false);
  
}

void RTree::SaveEfficiency(TH1D *input, TString outdir)
{
  gROOT->SetBatch(true);
  const TString th2Name = input->GetName();
  const TString th2Title = (TString)input->GetTitle();
  
  TCanvas *c1 = new TCanvas(th2Name,th2Title,1);
  c1->cd();
  c1->SetLogx();
  //c1->SetLogy();
  input->SetMarkerSize(2.0);
  input->UseCurrentStyle();
  input->Draw("ColZ,Text,E");
  // c1->SaveAs(s+"MuonAccEff3"+".png");
  if(saveEffToPDF_) c1->SaveAs(outdir+th2Name+".pdf");
  if(saveEffToPNG_) c1->SaveAs(outdir+th2Name+".png");
  delete c1;
  gROOT->SetBatch(false);
  
}

void TH1Deff::Fill(Double_t x,Double_t Weight, bool passOrFail)
{
  int matched=-1;
  for(unsigned int i=0; i < weights_.size();i++)
    {
      if(std::abs(weights_[i]-Weight)/Weight<0.0001 ) matched=i;
    }
  if(matched==-1)
    {
      TH1D* tempPass = (TH1D*) RefTH1D_->Clone();
      TH1D* tempFail = (TH1D*) RefTH1D_->Clone();
      TH1DPass_.push_back(tempPass );
      TH1DFail_.push_back(tempFail );

      if(passOrFail)
	{
	  TH1DPass_[weights_.size()]->Fill(x);
	}
      else TH1DFail_[weights_.size()]->Fill(x);
      weights_.push_back(Weight);
      // 		std::cout<<"Weight: "<<Weight<<"added"<<std::endl;
    }
  else
    {
      if(passOrFail) TH1DPass_[matched]->Fill(x);
      else TH1DFail_[matched]->Fill(x);
    }
}
TGraphAsymmErrors* TH1Deff::GetEfficiency()
{
  TList* myList = new TList(); 
  // compute th1 for sanity check
  TH1D* sumRef = (TH1D*) RefTH1D_->Clone();
  sumRef->Sumw2();
  for(unsigned int i=0; i< TH1DFail_.size();i++)
    {
      sumRef->Add(TH1DPass_[i],weights_[i]);
      RefTH1D_->Add(TH1DPass_[i],weights_[i]);
      sumRef->Add(TH1DFail_[i],weights_[i]);
    }
  RefTH1D_->Divide(RefTH1D_,sumRef,1,1,"B");
  for(unsigned int i=0; i<weights_.size();i++)
    {
      TH1DFail_[i]->Sumw2();
      TH1D *sum = (TH1D*)TH1DFail_[i]->Clone();
      TH1DPass_[i]->Sumw2();
		
      sum->Add(TH1DPass_[i]);
      sum->Print("all");
      TEfficiency* myEff = new TEfficiency(*TH1DPass_[i],*sum);
      //std::cout<<"Eff["<<i<<"]: "<<myEff->GetEfficiency(14)<<" +"<<myEff->GetEfficiencyErrorUp(14)<<" - "<<myEff->GetEfficiencyErrorLow(14)<<" passed: "<<myEff->GetPassedHistogram()->GetBinContent(14)<<" totalN: "<<myEff->GetTotalHistogram()->GetBinContent(14)<<" Weight: "<<weights_[i]<<" xCenter: "<<myEff->GetPassedHistogram()->GetBinCenter(14)<<"\n";
      // 		myEff->SetWeight(weights_[i]);
      myList->Add(myEff);
    }
  const Int_t size=weights_.size();
  Double_t weightArray[size];
  for(int i=0; i<size;i++)weightArray[i]=weights_[i];
	
  TGraphAsymmErrors* result= TEfficiency::Combine ((TCollection*)&*myList,(Option_t*)"kBUniform",(Int_t)weights_.size(),weightArray);
  // 	std::cout<<"Total Eff: "<<result->GetY()[13]<<" + "<<result->GetEYhigh()[13]<<" - "<<result->GetErrorYlow(13)<<" x value: "<<result->GetX()[13]<<"\n";
  // 	std::cout<<"-----------------------------------------------------------------Name: "<<name_ <<"\n";
  result->SetName(name_);
  result->SetTitle(title_);
  return result;
}

void TH1Deff::saveResults(TDirectory* MainDirectory)
{
  // 	MainDirectory->mkdir(name_);
  // 	std::cout<<"name: "<<name_<<std::endl;
  // 	TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
  // 	dir->cd();
  MainDirectory->cd();
  cout << "GetEfficiency() for " << RefTH1D_->GetName() << endl;
  TGraphAsymmErrors* result = GetEfficiency();
  result->SetName(RefTH1D_->GetName());
  result->SetTitle(RefTH1D_->GetTitle());
  result->Write();
  TString tempName = RefTH1D_->GetName();
  tempName+="_SanityCheck";
  MainDirectory->mkdir(tempName);
  // std::cout<<"name: "<<name_<<std::endl;
  TDirectory *sanityDir = (TDirectory*)MainDirectory->Get(tempName);
  sanityDir->cd();
  RefTH1D_->Write();
	
}


void TH2Deff::Fill(Double_t x, Double_t y, Double_t Weight, bool passOrFail)
{
  int matched=-1;
  for(unsigned int i=0; i < weights_.size();i++)
    {
      if(std::abs(weights_[i]-Weight)/Weight<0.0001 ) matched=i;
    }
  if(matched==-1)
    {
      TH2D* tempPass = (TH2D*) RefTH2D_->Clone();
      tempPass->Sumw2();
      TH2D* tempFail = (TH2D*) RefTH2D_->Clone();
      tempFail->Sumw2();
      TH2DPass_.push_back(tempPass );
      TH2DFail_.push_back(tempFail );
		
      if(passOrFail)	TH2DPass_[weights_.size()]->Fill(x,y);
      else TH2DFail_[weights_.size()]->Fill(x,y);
      weights_.push_back(Weight);
      // 		std::cout<<"Weight: "<<Weight<<" added"<<std::endl;
    }
  else
    {
      if(passOrFail) TH2DPass_[matched]->Fill(x,y);
      else TH2DFail_[matched]->Fill(x,y);
    }
}
std::vector<TGraphAsymmErrors*> TH2Deff::GetEfficiency()
{ 
  const Int_t nBinsY = RefTH2D_->GetNbinsY();
  std::cout<<"Number of binsY: "<<nBinsY<<std::endl;
  std::vector<std::vector<TH1D*> > th1PassVec, th1FailVec;
  std::vector<TList*> myLists;
  // reference sum th2f
  TH2D* sumTH2ftemp = (TH2D*) RefTH2D_->Clone();
  sumTH2ftemp->Sumw2();

  for(unsigned int i=0; i < TH2DPass_.size();i++)
    {
      RefTH2D_->Add(TH2DPass_[i],weights_[i]);
      sumTH2ftemp->Add(TH2DPass_[i],weights_[i]);
      sumTH2ftemp->Add(TH2DFail_[i],weights_[i]);
    }
  RefTH2D_->Divide(RefTH2D_,sumTH2ftemp,1,1,"B");
  for(int i=0; i<nBinsY;i++)
    {
      myLists.push_back(new TList());
      TString temp1 ( Form ("%d", i));
      temp1= (TString)name_+"Bin"+temp1;
      TString temp2 (Form ("_%f.0-%f.0",ybins_[i],ybins_[i+1]));
      temp2= (TString)title_+temp2;
      std::vector<TH1D*> tt;
      th1PassVec.push_back(tt);
      th1FailVec.push_back(tt);
      for(unsigned int ii=0; ii<weights_.size();ii++)
	{
	  TString temp3 ( Form ("%d", ii));
	  temp1= temp1+"_"+temp3;
	  temp3=temp1+"pass";
	  th1PassVec[i].push_back(new TH1D(temp3,temp2,nbinsx_, xbins_));
	  temp3=temp1+"fail";
	  th1FailVec[i].push_back(new TH1D(temp3,temp2,nbinsx_, xbins_));
	  for(int iii=0; iii<nbinsx_+1;iii++)
	    {
	      th1PassVec[i][ii]->SetBinContent(iii,TH2DPass_[ii]->GetBinContent(iii,i+1) );
	      // 			std::cout<<"P:BinContent["<<i<<"]["<<ii<<"]["<<iii<<"]: "<<TH2DPass_[ii]->GetBinContent(iii,i+1)<<" +- "<<TH2DPass_[ii]->GetBinError(iii,i+1)<<std::endl;
	      th1PassVec[i][ii]->SetBinError(iii,TH2DPass_[ii]->GetBinError(iii,i+1) );
	      th1FailVec[i][ii]->SetBinContent(iii,TH2DFail_[ii]->GetBinContent(iii,i+1) );
	      // 			std::cout<<"F:BinContent["<<i<<"]["<<ii<<"]["<<iii<<"]: "<<TH2DFail_[ii]->GetBinContent(iii,i)<<" +- "<<TH2DFail_[ii]->GetBinError(iii,i+1)<<std::endl;
	      th1FailVec[i][ii]->SetBinError(iii,TH2DFail_[ii]->GetBinError(iii,i+1) );
	    }
	  TH1D *sum = (TH1D*)th1FailVec[i][ii]->Clone();
	  // 		sum->SetTitle()
	  sum->Add(th1PassVec[i][ii]);
	  TEfficiency* myEff = new TEfficiency(*th1PassVec[i][ii],*sum);
	  // 		myEff->SetWeight(weights_[i]);
	  myLists[i]->Add(myEff);
	}
    }
  const Int_t size=weights_.size();
  Double_t weightArray[size];
  for(int i=0; i<size;i++)weightArray[i]=weights_[i];
  std::vector<TGraphAsymmErrors*> result; //= TEfficiency::Combine ((TCollection*)&*myList,(Option_t*)"kFCP",(Int_t)weights_.size(),weightArray);
  for(int i=0; i<nBinsY;i++)
    {
      result.push_back(TEfficiency::Combine ((TCollection*)&*myLists[i],(Option_t*)"kBUniform",(Int_t)weights_.size(),weightArray) );
    }
	
  return result;
}

void TH2Deff::saveResults(TDirectory* MainDirectory)
{
  MainDirectory->mkdir(name_);
  // 	std::cout<<"name: "<<name_<<std::endl;
  TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
  dir->cd();
  RefTH2D_->SetName(name_);
  RefTH2D_->Write();
  std::vector<TGraphAsymmErrors*> result = GetEfficiency();
  for(unsigned int i=0; i<result.size();i++) 
    {
      TString temp2 (Form ("_%d-%d",(int)ybins_[i],(int)ybins_[i+1]));
      temp2= (TString)title_+temp2;
      TString temp3 ( Form ("%d", i));
      temp3= (TString)name_+temp3;
      result[i]->SetName(temp3);
      result[i]->SetTitle(temp2);
      result[i]->Write();
    }
  dir->mkdir("SanityCheck");
  // std::cout<<"name: "<<name_<<std::endl;
  TDirectory *sanityDir = (TDirectory*)dir->Get("SanityCheck");
  sanityDir->cd();
  RefTH2D_->Write();
}
SearchBins::SearchBins()
{
  /*
  // HTmin,HTmax,MHTmin,MHTmax,NJetsmin,NJetsmax,BTagsmin,BTagsmax
  // NJets 4,6 BTags=0
  // fixed ht Njets btags all MHT bins
  bins_.push_back( Bin(500,800,200,500,4,6,-1,0) );
  bins_.push_back( Bin(800,1200,200,500,4,6,-1,0) );
  bins_.push_back( Bin(1200,99999,200,500,4,6,-1,0) );
	
  bins_.push_back( Bin(500,1200,500,750,4,6,-1,0) );
  bins_.push_back( Bin(1200,99999,500,750,4,6,-1,0) );
	
  bins_.push_back( Bin(800,99999,750,9999,4,6,-1,0) );
	
  // NJewts 7,8 BTags=0
  bins_.push_back( Bin(500,800,200,500,7,8,-1,0) );
  bins_.push_back( Bin(800,1200,200,500,7,8,-1,0) );
  bins_.push_back( Bin(1200,99999,200,500,7,8,-1,0) );
	
  bins_.push_back( Bin(500,1200,500,750,7,8,-1,0) );
  bins_.push_back( Bin(1200,99999,500,750,7,8,-1,0) );
	
  bins_.push_back( Bin(800,99999,750,9999,7,8,-1,0) );
	
	
  // NJewts 9,9999 BTags=0
  bins_.push_back( Bin(500,800,200,500,9,9999,-1,0) );
  bins_.push_back( Bin(800,1200,200,500,9,9999,-1,0) );
  bins_.push_back( Bin(1200,99999,200,500,9,9999,-1,0) );
	
  bins_.push_back( Bin(500,1200,500,750,9,9999,-1,0) );
  bins_.push_back( Bin(1200,99999,500,750,9,9999,-1,0) );
	
  bins_.push_back( Bin(800,99999,750,9999,9,9999,-1,0) );
	
	
	
  // NJets 4,6 BTags=1
  // fixed ht Njets btags all MHT bins
  bins_.push_back( Bin(500,800,200,500,4,6,1,1) );
  bins_.push_back( Bin(800,1200,200,500,4,6,1,1) );
  bins_.push_back( Bin(1200,99999,200,500,4,6,1,1) );
	
  bins_.push_back( Bin(500,1200,500,750,4,6,1,1) );
  bins_.push_back( Bin(1200,99999,500,750,4,6,1,1) );
	
  bins_.push_back( Bin(800,99999,750,9999,4,6,1,1) );
	
  // NJewts 7,8 BTags=0
  bins_.push_back( Bin(500,800,200,500,7,8,1,1) );
  bins_.push_back( Bin(800,1200,200,500,7,8,1,1) );
  bins_.push_back( Bin(1200,99999,200,500,7,8,1,1) );
	
  bins_.push_back( Bin(500,1200,500,750,7,8,1,1) );
  bins_.push_back( Bin(1200,99999,500,750,7,8,1,1) );
	
  bins_.push_back( Bin(800,99999,750,9999,7,8,1,1) );
	
	
  // NJewts 9,9999 BTags=1
  bins_.push_back( Bin(500,800,200,500,9,9999,1,1) );
  bins_.push_back( Bin(800,1200,200,500,9,9999,1,1) );
  bins_.push_back( Bin(1200,99999,200,500,9,9999,1,1) );
	
  bins_.push_back( Bin(500,1200,500,750,9,9999,1,1) );
  bins_.push_back( Bin(1200,99999,500,750,9,9999,1,1) );
	
  bins_.push_back( Bin(800,99999,750,9999,9,9999,1,1) );
  */
	
	
  // NJets 4,6 BTags=2
  // fixed ht Njets btags all MHT bins
  bins_.push_back( Bin(500,800,200,500,4,6,2,2) );
  bins_.push_back( Bin(800,1200,200,500,4,6,2,2) );
  bins_.push_back( Bin(1200,99999,200,500,4,6,2,2) );
	
  bins_.push_back( Bin(500,1200,500,750,4,6,2,2) );
  bins_.push_back( Bin(1200,99999,500,750,4,6,2,2) );
	
  bins_.push_back( Bin(800,99999,750,9999,4,6,2,2) );
	
  // NJewts 7,8 BTags=2
  bins_.push_back( Bin(500,800,200,500,7,8,2,2) );
  bins_.push_back( Bin(800,1200,200,500,7,8,2,2) );
  bins_.push_back( Bin(1200,99999,200,500,7,8,2,2) );
	
  bins_.push_back( Bin(500,1200,500,750,7,8,2,2) );
  bins_.push_back( Bin(1200,99999,500,750,7,8,2,2) );
	
  bins_.push_back( Bin(800,99999,750,9999,7,8,2,2) );
	
	
  // NJewts 9,9999 BTags=2
  bins_.push_back( Bin(500,800,200,500,9,9999,2,2) );
  bins_.push_back( Bin(800,1200,200,500,9,9999,2,2) );
  bins_.push_back( Bin(1200,99999,200,500,9,9999,2,2) );
	
  bins_.push_back( Bin(500,1200,500,750,9,9999,2,2) );
  bins_.push_back( Bin(1200,99999,500,750,9,9999,2,2) );
	
  bins_.push_back( Bin(800,99999,750,9999,9,9999,2,2) );
	
	
  // NJets 4,6 BTags=>3
  // fixed ht Njets btags all MHT bins
  bins_.push_back( Bin(500,800,200,500,4,6,3,9999) );
  bins_.push_back( Bin(800,1200,200,500,4,6,3,9999) );
  bins_.push_back( Bin(1200,99999,200,500,4,6,3,9999) );
	
  bins_.push_back( Bin(500,1200,500,750,4,6,3,9999) );
  bins_.push_back( Bin(1200,99999,500,750,4,6,3,9999) );
	
  bins_.push_back( Bin(800,99999,750,9999,4,6,3,9999) );
	
  // NJewts 7,8 BTags=>3
  bins_.push_back( Bin(500,800,200,500,7,8,3,9999) );
  bins_.push_back( Bin(800,1200,200,500,7,8,3,9999) );
  bins_.push_back( Bin(1200,99999,200,500,7,8,3,9999) );
	
  bins_.push_back( Bin(500,1200,500,750,7,8,3,9999) );
  bins_.push_back( Bin(1200,99999,500,750,7,8,3,9999) );
	
  bins_.push_back( Bin(800,99999,750,9999,7,8,3,9999) );
	
	
  // NJewts 9,9999 BTags=>3
  bins_.push_back( Bin(500,800,200,500,9,9999,3,9999) );
  bins_.push_back( Bin(800,1200,200,500,9,9999,3,9999) );
  bins_.push_back( Bin(1200,99999,200,500,9,9999,3,9999) );
	
  bins_.push_back( Bin(500,1200,500,750,9,9999,3,9999) );
  bins_.push_back( Bin(1200,99999,500,750,9,9999,3,9999) );
	
  bins_.push_back( Bin(800,99999,750,9999,9,9999,3,9999) );
}

unsigned int SearchBins::GetBinNumber(double HT, double MHT, int NJets, int BTags)
{
  unsigned int result =999;
  int match =-1;
  for(unsigned int i=0; i<bins_.size();i++)
    {
      // 		std::cout<<"Bin["<<i<<"]: HT["<<bins_[i].HTmin_<<","<<bins_[i].HTmax_<<"] MHT["<<bins_[i].MHTmin_<<","<<bins_[i].MHTmax_<<"] NJets["<<bins_[i].NJetsmin_<<","<<bins_[i].NJetsmax_<<"] BTags["<<bins_[i].BTagsmin_<<","<<bins_[i].BTagsmax_<<"]\n";
      if(HT>bins_[i].HTmin_ && 
	 HT<bins_[i].HTmax_ &&
	 MHT>bins_[i].MHTmin_ && 
	 MHT<bins_[i].MHTmax_ &&
	 NJets+0.1>bins_[i].NJetsmin_ && 
	 NJets-0.1<bins_[i].NJetsmax_ &&
	 BTags+0.1>bins_[i].BTagsmin_ && 
	 BTags-0.1<bins_[i].BTagsmax_
	 )
	{
	  result=i;
	  match++;
	}
    }
  if(match==-1)
    {
      //  		std::cout<<"Error event fits in no bin!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
      result=999;
    }
  if(match>0)
    {
      // std::cout<<"Error event fits in more than one bin!!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
    }
  return result+1; // to not use the 0 bin but start counting at 1
}

Efficiency::Efficiency(const char* name, const char* title)
{
  TH1DSearchBins_= new TH1Deff (name,title,bins_.size(),0,bins_.size()+1);
  name_=name;
  title_=title;
  splitAfter_=18;
  unsigned int plotsNumber= bins_.size()/splitAfter_;
  // 	std::cout<<"Efficiency::Efficiency plotsNumber="<<plotsNumber<<" number of bins: "<<bins_.size()<<std::endl;
  if(plotsNumber * splitAfter_<bins_.size() )
    {

      plotsNumber++;
      for(unsigned int i=0; i < plotsNumber;i++)
	{
			
	  TString temp2 (Form ("_%d",(int)i+1));
	  TString temp1 = name;
	  temp1+=temp2;
	  temp2 = title + temp2;
	  const char* name1=temp1;
	  const char* title1=temp2;
			
	  if(i+1==plotsNumber)
	    {
	      // 				std::cout<<"titlelast["<<i<<"]: "<<temp1<<std::endl;
	      unsigned int tempBins = bins_.size() - plotsNumber * splitAfter_;
	      TH1DSearchBinsSplit_.push_back( new TH1Deff (name1,title1,tempBins,0,tempBins+1) );
	      continue;
	    }
	  // 			std::cout<<"title["<<i<<"]: "<<temp1<<std::endl;
	  TH1DSearchBinsSplit_.push_back( new TH1Deff (name1,title1,splitAfter_,0,splitAfter_+1) );
	}
    }
  else
    {
      for(unsigned int i=0; i < plotsNumber;i++)
	{

	  TString temp2 (Form ("_%d",(int)i+1));
	  TString temp1 = name;
	  temp1+=temp2;
	  temp2 = title + temp2;
	  const char* name1=temp1;
	  const char* title1=temp2;
	  TH1Deff* tempeff2 = new  TH1Deff (name1,title1,splitAfter_,0,splitAfter_+1);
	  TH1DSearchBinsSplit_.push_back( tempeff2 );
	}
    }
}

void Efficiency::saveResults(TDirectory* MainDirectory)
{
  MainDirectory->mkdir(name_);
  // std::cout<<"name: "<<name_<<std::endl;
  TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
  dir->cd();
  TH1DSearchBins_->saveResults(dir);
  // std::cout<<"TH1DSearchBinsSplit_.size(): "<<TH1DSearchBinsSplit_.size()<<std::endl;
  for(unsigned int i=0; i<TH1DSearchBinsSplit_.size();i++) 
    {
      TString temp2 (Form ("_%d",(int)i+1));
      TString temp1 = name_;
      temp1+=temp2;
      temp2 = title_ + temp2;
      const char* name1=temp1;
      const char* title1=temp2;
      TH1DSearchBinsSplit_[i]->SetTitle(title1);
      TH1DSearchBinsSplit_[i]->SetName(name1);
      // std::cout << "Saving TH1DSearchBinsSplit_[i]->GetName()" << std::endl;
      TH1DSearchBinsSplit_[i]->saveResults(dir);
    }
  // std::cout << "Exit Efficiency::saveResults()" << std::endl;
}


void Efficiency::Fill(double HT, double MHT, int NJets, int BTags, double Weight, bool passOrFail)
{
  double bin = GetBinNumber(HT,MHT,NJets,BTags);
	
  if(bin<bins_.size()+2) 
    {
      TH1DSearchBins_->Fill(bin+0.01, Weight,passOrFail);
      unsigned int splitHist=0;
      // 	std::cout<<"bin before split: "<<bin<<std::endl;
      for(int ii=0;bin>splitAfter_;ii++)
	{
	  splitHist++;
	  bin = bin-splitAfter_;
	}
      // 		if(splitHist==3)std::cout<<"BinForSplit: "<<bin<<" with splitHistNumber "<<splitHist<<" and TH1DSearchBinsSplit_.size(): "<<TH1DSearchBinsSplit_.size()<<std::endl;
		
      TH1DSearchBinsSplit_[splitHist]->Fill(bin+0.01, Weight,passOrFail);
    }
}
SearchBinEventCount::SearchBinEventCount(const char* name)
{
  fullTH1D_ = new 	TH1D(name, name, bins_.size(),0,bins_.size()+1);
  fullTH1D_->Sumw2();
  name_=name;
  splitAfter_=18;
  unsigned int plotsNumber= bins_.size()/splitAfter_;
  // 	std::cout<<"Efficiency::Efficiency plotsNumber="<<plotsNumber<<" number of bins: "<<bins_.size()<<std::endl;
  if(plotsNumber * splitAfter_<bins_.size() )
    {
		
      plotsNumber++;
      for(unsigned int i=0; i < plotsNumber;i++)
	{
			
	  TString temp2 (Form ("_%d",(int)i+1));
	  TString temp1 = name;
	  temp1+=temp2;
	  temp2 = name + temp2;
	  const char* name1=temp1;
	  const char* title1=temp2;
			
	  if(i+1==plotsNumber)
	    {
	      // 				std::cout<<"titlelast["<<i<<"]: "<<temp1<<std::endl;
	      unsigned int tempBins = bins_.size() - plotsNumber * splitAfter_;
	      splitTH1D_.push_back( new TH1D (name1,title1,tempBins,0,tempBins+1) );
	      splitTH1D_[i]->Sumw2();
	      continue;
	    }
	  // 			std::cout<<"title["<<i<<"]: "<<temp1<<std::endl;
	  splitTH1D_.push_back( new TH1D (name1,title1,splitAfter_,0,splitAfter_+1) );
	  splitTH1D_[i]->Sumw2();
	}
    }
  else
    {
      for(unsigned int i=0; i < plotsNumber;i++)
	{
			
	  TString temp2 (Form ("_%d",(int)i+1));
	  TString temp1 = name;
	  temp1+=temp2;

	  const char* name1=temp1;
	  TH1D* tempeff2 = new  TH1D (name1,name1,splitAfter_,0,splitAfter_+1);
	  splitTH1D_.push_back( tempeff2 );
	  splitTH1D_[i]->Sumw2();
	}
    }
}


void SearchBinEventCount::Fill(double HT, double MHT, int NJets, int BTags, double Weight)
{
  double bin = GetBinNumber(HT,MHT,NJets,BTags);
	
  if(bin<bins_.size()+2) 
    {
      fullTH1D_->Fill(bin-0.01, Weight);
      unsigned int splitHist=0;
      // 	std::cout<<"bin before split: "<<bin<<std::endl;
      for(int ii=0;bin>splitAfter_;ii++)
	{
	  splitHist++;
	  bin = bin-splitAfter_;
	}
      // 		if(splitHist==3)std::cout<<"BinForSplit: "<<bin<<" with splitHistNumber "<<splitHist<<" and TH1DSearchBinsSplit_.size(): "<<TH1DSearchBinsSplit_.size()<<std::endl;
		
      splitTH1D_[splitHist]->Fill(bin-0.1, Weight);
    }
}
void SearchBinEventCount::saveResults(TDirectory* MainDirectory)
{
  MainDirectory->mkdir(name_);
  // std::cout<<"name: "<<name_<<std::endl;
  TDirectory *dir = (TDirectory*)MainDirectory->Get(name_);
  dir->cd();
  fullTH1D_->Write();
  for(unsigned int i=0; i<splitTH1D_.size();i++) 
    {
      TString temp2 (Form ("_%d",(int)i+1));
      TString temp1 = name_;
      temp1+=temp2;
      temp2 = name_ + temp2;
      const char* name1=temp1;
      const char* title1=temp2;
      splitTH1D_[i]->SetTitle(title1);
      splitTH1D_[i]->SetName(name1);
      splitTH1D_[i]->Write();
    }
}
