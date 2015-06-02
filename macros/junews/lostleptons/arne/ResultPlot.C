#include <TChain.h>
#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TTree.h>
#include <TH2D.h>
#include <TH1D.h>
#include "TVector2.h"
#include <cmath>
#include "TCanvas.h"
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include <iostream>
#include "Bin.h"

TH1D* ratioCalculator(TH1D* passTH1, TH1D* failTH1)
{
  passTH1->Sumw2();
  TH1D *sum = (TH1D*)passTH1->Clone();
  failTH1->Sumw2();
	
  sum->Add(failTH1);
  passTH1->Divide(passTH1,sum,1,1,"B");
  return passTH1;
}

void SaveClosure(TH1D* prediction, TH1D* expectation, TDirectory* Folder) // prediction durch expectation
{
  TH1D* Closure = (TH1D*) prediction->Clone();
  Closure->Divide(prediction,expectation,1,1,"B");
  TString title = prediction->GetTitle();
  title +="_closure";
  // 	title = "#mu & e Control-Sample Ratio in Search Bins; Search bins; #mu / e CS";
  Closure->SetTitle(title);
  title = prediction->GetName();
  title+="_closure";
  Closure->SetName(title);
  Folder->cd();
  Closure->Write();
}


class SearchBinEventCount : public SearchBins
{
public:
  SearchBinEventCount(){}
  SearchBinEventCount(const char*);
  void Fill(double HT, double MHT, int NJets, int BTags, double Weight);
  void saveResults(TDirectory* MainDirectory);
  TH1D* getFullTH1D(){return fullTH1D_;}
  ~SearchBinEventCount(){}
private:
  TH1D* fullTH1D_;
  std::vector<TH1D*> splitTH1D_;
  unsigned int splitAfter_;
  const char* name_;
};

void ResultPlot(TString expectation="skims/13TeV/reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78.root",
		TString prediction="macros/junews/lostleptons/arne/Prediction/Prediction.root")
{
	

  //Reset ROOT and connect tree file
  gROOT->Reset();
  TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject(expectation.Data());
  if (!f) {
    f = new TFile(expectation.Data());
  }
  // output tree
  TFile *outPutFile = new TFile("macros/junews/lostleptons/arne/Closure/Closure.root","RECREATE"); 
  outPutFile->cd();
  TTree* OutPutTreeExp_ = new TTree("ResultExp","Result Lost-Lepton Tree for plotting tool");
  TTree* OutPutTreePre_ = new TTree("ResultPre","Result Lost-Lepton Tree for plotting tool");
  UShort_t Bin=0;
  Float_t Weight_=0, PredictionWeight_=0; 
	
  OutPutTreeExp_->Branch("Weight", &Weight_, "Weight/F");
  OutPutTreePre_->Branch("totalWeightDiLep", &PredictionWeight_, "totalWeightDiLep/F");
  OutPutTreeExp_->Branch("Bin",&Bin,"Bin/s");
  //
  UShort_t muAcc_=0, muIso_=0, muReco_=0;
  OutPutTreeExp_->Branch("muAcc",&muAcc_,"muAcc/s");
  OutPutTreeExp_->Branch("muReco",&muReco_,"muReco/s");
  OutPutTreeExp_->Branch("muIso",&muIso_,"muIso/s");
	
  UShort_t elAcc_=0, elIso_=0, elReco_=0;
  OutPutTreeExp_->Branch("elAcc",&elAcc_,"elAcc/s");
  OutPutTreeExp_->Branch("elReco",&elReco_,"elReco/s");
  OutPutTreeExp_->Branch("elIso",&elIso_,"elIso/s");
  //
  Float_t muIsoWeight_=0, muRecoWeight_=0, muAccWeight_=0;
  OutPutTreePre_->Branch("muIsoWeight", &muIsoWeight_, "muIsoWeight/F");
  OutPutTreePre_->Branch("muRecoWeight", &muRecoWeight_, "muRecoWeight/F");
  OutPutTreePre_->Branch("muAccWeight", &muAccWeight_, "muAccWeight/F");
	
  Float_t elIsoWeight_=0, elRecoWeight_=0, elAccWeight_=0;
  OutPutTreePre_->Branch("elIsoWeight", &elIsoWeight_, "elIsoWeight/F");
  OutPutTreePre_->Branch("elRecoWeight", &elRecoWeight_, "elRecoWeight/F");
  OutPutTreePre_->Branch("elAccWeight", &elAccWeight_, "elAccWeight/F");
	
  UShort_t num_veto_mus_=0, num_veto_els_=0;
  OutPutTreePre_->Branch("num_veto_mus",&num_veto_mus_,"num_veto_mus/s");
  OutPutTreePre_->Branch("num_veto_els",&num_veto_els_,"num_veto_els/s");
	
  TTree* LostLeptonExpectation = (TTree*) f->Get("reduced_tree");

	
  SearchBinEventCount * ControlSampleMu_ = new SearchBinEventCount("ControlSampleMu");
  SearchBinEventCount * ControlSampleEl_ = new SearchBinEventCount("ControlSampleEl");
	
  SearchBinEventCount * totalExpectation_ = new SearchBinEventCount("TotalLostLeptonExpecation");
  SearchBinEventCount * totalPrediction_ = new SearchBinEventCount("TotalLostLeptonPrediction");
  SearchBinEventCount * totalPredictionMu_ = new SearchBinEventCount("TotalLostLeptonPredictionMu");
  SearchBinEventCount * totalPredictionEl_ = new SearchBinEventCount("TotalLostLeptonPredictionEl");
	
  // SearchBinEventCount * totalExpectationIsoTrackReduction_ = new SearchBinEventCount("TotalLostLeptonExpecationIsoTrackReduction");
  // SearchBinEventCount * totalPredictionIsoTrackReduction_ = new SearchBinEventCount("TotalLostLeptonPredictionIsoTrackReduction");
  // SearchBinEventCount * totalPredictionMuIsoTrackReduction_ = new SearchBinEventCount("TotalLostLeptonPredictionMuIsoTrackReduction");
  // SearchBinEventCount * totalPredictionElIsoTrackReduction_ = new SearchBinEventCount("TotalLostLeptonPredictionElIsoTrackReduction");
	
  // separted closure
  SearchBinEventCount * totalExpectationMuAcc_ = new SearchBinEventCount("TotalLostLeptonExpecationMuAcc");
  SearchBinEventCount * totalPredictionMuCSMuAcc_ = new SearchBinEventCount("TotalLostLeptonPredictionMuCSMuAcc");
  SearchBinEventCount * totalPredictionElCSMuAcc_ = new SearchBinEventCount("TotalLostLeptonPredictionElCSMuAcc");
	
  SearchBinEventCount * totalExpectationMuReco_ = new SearchBinEventCount("TotalLostLeptonExpecationMuReco");
  SearchBinEventCount * totalPredictionMuCSMuReco_ = new SearchBinEventCount("TotalLostLeptonPredictionMuCSMuReco");
  SearchBinEventCount * totalPredictionElCSMuReco_ = new SearchBinEventCount("TotalLostLeptonPredictionElCSMuReco");
	
  SearchBinEventCount * totalExpectationMuIso_ = new SearchBinEventCount("TotalLostLeptonExpecationMuIso");
  SearchBinEventCount * totalPredictionMuCSMuIso_ = new SearchBinEventCount("TotalLostLeptonPredictionMuCSMuIso");
  SearchBinEventCount * totalPredictionElCSMuIso_ = new SearchBinEventCount("TotalLostLeptonPredictionElCSMuIso");
	
	
	
  SearchBinEventCount * totalExpectationElAcc_ = new SearchBinEventCount("TotalLostLeptonExpecationElAcc");
  SearchBinEventCount * totalPredictionMuCSElAcc_ = new SearchBinEventCount("TotalLostLeptonPredictionMuCSElAcc");
  SearchBinEventCount * totalPredictionElCSElAcc_ = new SearchBinEventCount("TotalLostLeptonPredictionElCSElAcc");
	
  SearchBinEventCount * totalExpectationElReco_ = new SearchBinEventCount("TotalLostLeptonExpecationElReco");
  SearchBinEventCount * totalPredictionMuCSElReco_ = new SearchBinEventCount("TotalLostLeptonPredictionMuCSElReco");
  SearchBinEventCount * totalPredictionElCSElReco_ = new SearchBinEventCount("TotalLostLeptonPredictionElCSElReco");
	
  SearchBinEventCount * totalExpectationElIso_ = new SearchBinEventCount("TotalLostLeptonExpecationElIso");
  SearchBinEventCount * totalPredictionMuCSElIso_ = new SearchBinEventCount("TotalLostLeptonPredictionMuCSElIso");
  SearchBinEventCount * totalPredictionElCSElIso_ = new SearchBinEventCount("TotalLostLeptonPredictionElCSElIso");
	
  double totalExp=0, totalPre=0;
  double totalExpError=0, totalPreError=0;
  double totalPreMu=0, totalPreMuError=0;
  double totalPreEl=0, totalPreElError=0;
	
  double totalExpMuAcc=0, totalExpMuReco=0, totalExpMuIso=0;
	
  double totalPreMuAcc=0, totalPreMuReco=0, totalPreMuIso=0;
	
	
  double totalExpElAcc=0, totalExpElReco=0, totalExpElIso=0;
	
  double totalPreElAcc=0, totalPreElReco=0, totalPreElIso=0;
	
  //Declaration of leaves types
  Float_t         HT;
  Float_t         MHT;
  UShort_t        NJets;
  UShort_t        BTags;

  Float_t         min_delta_phi_met_N;
  Float_t         Weight;
	
  UShort_t        Expectation;
  UShort_t        muAcc;
  UShort_t        muReco;
  UShort_t        muIso;
  UShort_t        elAcc;
  UShort_t        elReco;
  UShort_t        elIso;
  UShort_t        ExpectationDiLep;

  bool passesBadJetFilter;
  Int_t PBNRcode;
  //	UShort_t        ExpectationReductionIsoTrack;
	
  // Set branch addresses.
  LostLeptonExpectation->SetBranchAddress("ht30",&HT);
  LostLeptonExpectation->SetBranchAddress("mht30",&MHT);
  LostLeptonExpectation->SetBranchAddress("num_jets_pt30",&NJets);
  LostLeptonExpectation->SetBranchAddress("num_csvm_jets30",&BTags);
  LostLeptonExpectation->SetBranchAddress("min_delta_phi_met_N",&min_delta_phi_met_N);
  LostLeptonExpectation->SetBranchAddress("weightppb",&Weight);
	
  LostLeptonExpectation->SetBranchAddress("Expectation",&Expectation);
  //	LostLeptonExpectation->SetBranchAddress("ExpectationReductionIsoTrack",&ExpectationReductionIsoTrack);
  LostLeptonExpectation->SetBranchAddress("muAcc",&muAcc);
  LostLeptonExpectation->SetBranchAddress("muReco",&muReco);
  LostLeptonExpectation->SetBranchAddress("muIso",&muIso);
	
  LostLeptonExpectation->SetBranchAddress("elAcc",&elAcc);
  LostLeptonExpectation->SetBranchAddress("elReco",&elReco);
  LostLeptonExpectation->SetBranchAddress("elIso",&elIso);
	
  //LostLeptonExpectation->SetBranchAddress("IsolatedTracksNum",&IsolatedTracksNum);
  LostLeptonExpectation->SetBranchAddress("ExpectationDiLep",&ExpectationDiLep);

  LostLeptonExpectation->SetBranchAddress("passesBadJetFilter",&passesBadJetFilter);
  LostLeptonExpectation->SetBranchAddress("PBNRcode",&PBNRcode);

  LostLeptonExpectation->SetBranchStatus("jet_*", 0);
  LostLeptonExpectation->SetBranchStatus("fat*", 0);
  LostLeptonExpectation->SetBranchStatus("*track*", 0);
	
	
  Long64_t nentries = LostLeptonExpectation->GetEntries();
	
  SearchBins *SearchBins_ = new SearchBins();

	
  Long64_t nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += LostLeptonExpectation->GetEntry(i);
    Weight*=10000;
    //cout << "Expectation weight: " << Weight << endl;
    // total expectation
    if(NJets<4) continue;
    if(BTags<2) continue;
    if(HT<500) continue;
    if(MHT<200) continue;
    if(min_delta_phi_met_N<6) continue;
    // if (!passesBadJetFilter || PBNRcode<1) continue;
    //if (num_true_mus+num_true_els==1&&num_true_had_taus==0)
     if(Expectation==1 && NJets>3.1)
      {
	totalExpectation_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExp+=Weight;
	totalExpError+= Weight*Weight;
	Bin = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);
	Weight_=Weight;
	muAcc_=muAcc; muReco_=muReco; muIso_=muIso; elAcc_=elAcc; elReco_=elReco; elIso_=elIso;
	OutPutTreeExp_->Fill();
      }
    // if(Expectation==1 && ExpectationReductionIsoTrack==0)
    // {
    // 	totalExpectationIsoTrackReduction_->Fill(HT,MHT,NJets,BTags,Weight);
    // }
    if(muAcc==0)
      {
	totalExpectationMuAcc_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExpMuAcc+=Weight;
      }
    if(muReco==0)
      {
	totalExpectationMuReco_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExpMuReco+=Weight;
      }
    if(muIso==0)
      {
	totalExpectationMuIso_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExpMuIso+=Weight;
      }
					
    if(elAcc==0)
      {
	totalExpectationElAcc_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExpElAcc+=Weight;
      }
    if(elReco==0)
      {
	totalExpectationElReco_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExpElReco+=Weight;
      }
    if(elIso==0)
      {
	totalExpectationElIso_->Fill(HT,MHT,NJets,BTags,Weight);
	totalExpElIso+=Weight;
      }
  }
	   
  // loop on prediction
  gROOT->Reset();
  TFile *f1 = (TFile*)gROOT->GetListOfFiles()->FindObject(prediction.Data());
  if (!f1) {
    f1 = new TFile(prediction.Data());
  }
  TTree* LostLeptonPrediction = (TTree*) f1->Get("LostLeptonPrediction");
	
  Float_t         MTW;
  UShort_t        num_veto_mus;
  UShort_t        num_veto_els;
	
  Float_t         totalWeightDiLep;
  //	Float_t         totalWeightDiLepIsoTrackReduced;
	
  Float_t         muIsoWeight;
  Float_t         muRecoWeight;
  Float_t         muAccWeight;

  Float_t         elAccWeight;
  Float_t         elRecoWeight;
  Float_t         elIsoWeight;
	
	
	
  LostLeptonPrediction->SetBranchAddress("HT",&HT);
  LostLeptonPrediction->SetBranchAddress("MHT",&MHT);
  LostLeptonPrediction->SetBranchAddress("NJets",&NJets);
  LostLeptonPrediction->SetBranchAddress("BTags",&BTags);
  LostLeptonPrediction->SetBranchAddress("min_delta_phi_met_N",&min_delta_phi_met_N);
  LostLeptonPrediction->SetBranchAddress("Weight",&Weight);
	
  LostLeptonPrediction->SetBranchAddress("MTW",&MTW);
  LostLeptonPrediction->SetBranchAddress("num_veto_mus",&num_veto_mus);
  LostLeptonPrediction->SetBranchAddress("num_veto_els",&num_veto_els);
  LostLeptonPrediction->SetBranchAddress("totalWeightDiLep",&totalWeightDiLep);
  //	LostLeptonPrediction->SetBranchAddress("totalWeightDiLepIsoTrackReduced",&totalWeightDiLepIsoTrackReduced);
	
  LostLeptonPrediction->SetBranchAddress("muIsoWeight",&muIsoWeight);
  LostLeptonPrediction->SetBranchAddress("muRecoWeight",&muRecoWeight);
  LostLeptonPrediction->SetBranchAddress("muAccWeight",&muAccWeight);
	
  LostLeptonPrediction->SetBranchAddress("elAccWeight",&elAccWeight);
  LostLeptonPrediction->SetBranchAddress("elRecoWeight",&elRecoWeight);
  LostLeptonPrediction->SetBranchAddress("elIsoWeight",&elIsoWeight);

	
  nentries = LostLeptonPrediction->GetEntries();
  nbytes = 0;
  for (Long64_t i=0; i<nentries;i++) {
    nbytes += LostLeptonPrediction->GetEntry(i);
    // total expectation
    Weight*=10000;
    //    // cout << "Prediction weight: " << Weight << endl;
    if(MTW>100 || NJets<3.7 || BTags<1.6)continue;
    // cout << "NJets = " << NJets << endl;
    if(NJets<4) continue;
    // cout << "BTags = " << BTags << endl;
    if(BTags<2) continue;
    // cout << "HT = " << HT << endl;
    if(HT<500) continue;
    // cout << "MHT = " << MHT << endl;
    if(MHT<200) continue;
    // cout << "min_delta_phi_met_N = " << min_delta_phi_met_N << endl;
    if(min_delta_phi_met_N<6) continue;
    Bin = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);
    Weight_=Weight;
    num_veto_mus_=num_veto_mus;
    num_veto_els_=num_veto_els;
    if(num_veto_mus==1 && num_veto_els==0)
      {
	// cout << "Single muon event..." << endl;
	ControlSampleMu_->Fill(HT,MHT,NJets,BTags,Weight);
			
	totalPrediction_->Fill(HT,MHT,NJets,BTags,totalWeightDiLep/2);
	totalPredictionMu_->Fill(HT,MHT,NJets,BTags,totalWeightDiLep);
	totalPre+=totalWeightDiLep;
	totalPreError+= totalWeightDiLep*totalWeightDiLep;
	totalPreMu+=totalWeightDiLep;
	totalPreMuError+= totalWeightDiLep*totalWeightDiLep;
	// isotrackveto applied
	// totalPredictionIsoTrackReduction_->Fill(HT,MHT,NJets,BTags,totalWeightDiLepIsoTrackReduced/2);
	// totalPredictionMuIsoTrackReduction_->Fill(HT,MHT,NJets,BTags,totalWeightDiLepIsoTrackReduced);
	// separted closure
	totalPredictionMuCSMuAcc_->Fill(HT,MHT,NJets,BTags,muAccWeight);
	totalPreMuAcc+=muAccWeight;
	totalPredictionMuCSMuReco_->Fill(HT,MHT,NJets,BTags,muRecoWeight);
	totalPreMuReco+=muRecoWeight;
	totalPredictionMuCSMuIso_->Fill(HT,MHT,NJets,BTags,muIsoWeight);
	totalPreMuIso+=muIsoWeight;
			
	totalPredictionMuCSElAcc_->Fill(HT,MHT,NJets,BTags,elAccWeight);
	totalPreElAcc+=elAccWeight;
	totalPredictionMuCSElReco_->Fill(HT,MHT,NJets,BTags,elRecoWeight);
	totalPreElReco+=elRecoWeight;
	totalPredictionMuCSElIso_->Fill(HT,MHT,NJets,BTags,elIsoWeight);
	totalPreElIso+=elIsoWeight;
	muIsoWeight_=muIsoWeight; muRecoWeight_=muRecoWeight; muAccWeight_=muAccWeight;  elIsoWeight_=elIsoWeight; elRecoWeight_=elRecoWeight; elAccWeight_=elAccWeight; 
	PredictionWeight_= totalWeightDiLep;
	OutPutTreePre_->Fill();
      }
    if(num_veto_mus==0 && num_veto_els==1)
      {
	ControlSampleEl_->Fill(HT,MHT,NJets,BTags,Weight);
			
	totalPrediction_->Fill(HT,MHT,NJets,BTags,totalWeightDiLep/2);
	totalPredictionEl_->Fill(HT,MHT,NJets,BTags,totalWeightDiLep);
	totalPre+=totalWeightDiLep;
	totalPreError+= totalWeightDiLep*totalWeightDiLep;
	totalPreEl+=totalWeightDiLep;
	totalPreElError+= totalWeightDiLep*totalWeightDiLep;
	// isotrackveto applied
	// totalPredictionIsoTrackReduction_->Fill(HT,MHT,NJets,BTags,totalWeightDiLepIsoTrackReduced/2);
	// totalPredictionElIsoTrackReduction_->Fill(HT,MHT,NJets,BTags,totalWeightDiLepIsoTrackReduced);
	// separted closure
	totalPredictionElCSMuAcc_->Fill(HT,MHT,NJets,BTags,muAccWeight);
	totalPredictionElCSMuReco_->Fill(HT,MHT,NJets,BTags,muRecoWeight);
	totalPredictionElCSMuIso_->Fill(HT,MHT,NJets,BTags,muIsoWeight);
			
	totalPredictionElCSElAcc_->Fill(HT,MHT,NJets,BTags,elAccWeight);
	totalPredictionElCSElReco_->Fill(HT,MHT,NJets,BTags,elRecoWeight);
	totalPredictionElCSElIso_->Fill(HT,MHT,NJets,BTags,elIsoWeight);
	PredictionWeight_= totalWeightDiLep;
	OutPutTreePre_->Fill();
      }
  }
	
  TH1D* expectationTH1 = (TH1D*) totalExpectation_->getFullTH1D();
  TH1D* predictionTH1 = (TH1D*) totalPrediction_->getFullTH1D();
  TH1D* FullClosure = (TH1D*) expectationTH1->Clone();
  FullClosure->Divide(predictionTH1,expectationTH1,1,1,"B");
  FullClosure->SetName("LostLeptonClosure");
  FullClosure->SetTitle("Closure of lost-lepton method");
	
  TH1D* predictionTH1Mu = (TH1D*) totalPredictionMu_->getFullTH1D();
  TH1D* FullClosureMu = (TH1D*) expectationTH1->Clone();
  FullClosureMu->Divide(predictionTH1Mu,expectationTH1,1,1,"B");
  FullClosureMu->SetName("LostLeptonClosureMuCS");
  FullClosureMu->SetTitle("Closure of lost-lepton method #mu control-sample");
	
  TH1D* predictionTH1El = (TH1D*) totalPredictionEl_->getFullTH1D();
  TH1D* FullClosureEl = (TH1D*) expectationTH1->Clone();
  FullClosureEl->Divide(predictionTH1El,expectationTH1,1,1,"B");
  FullClosureEl->SetName("LostLeptonClosureElCS");
  FullClosureEl->SetTitle("Closure of lost-lepton method el control-sample");

  std::cout<<"Result:\n TotalExpectation: "<<totalExp<<" +- " << sqrt(totalExpError)<<"\n TotalPrediction: "<<totalPre/2<<" +- "<<sqrt(totalPreError)/2<<std::endl;
  std::cout<<"TotalExpectation: "<<totalExp<<" +- " << sqrt(totalExpError)<<"\n TotalPredictionMu: "<<totalPreMu<<" +- "<<sqrt(totalPreMuError)<<std::endl;
  std::cout<<"TotalExpectation: "<<totalExp<<" +- " << sqrt(totalExpError)<<"\n TotalPredictionEl: "<<totalPreEl<<" +- "<<sqrt(totalPreElError)<<std::endl;
  std::cout<<"--------------------------------------------------------------------------------------------------------------------------\n";
  std::cout<<"MuAccExp: "<<totalExpMuAcc<<"\n MuAccPre: "<<totalPreMuAcc<<std::endl;
  std::cout<<"MuRecoExp: "<<totalExpMuReco<<"\n MuRecoPre: "<<totalPreMuReco<<std::endl;
  std::cout<<"MuIsoExp: "<<totalExpMuIso<<"\n MuIsoPre: "<<totalPreMuIso<<std::endl;
  std::cout<<"ElAccExp: "<<totalExpElAcc<<"\n ElAccPre: "<<totalPreElAcc<<std::endl;
  std::cout<<"ElRecoExp: "<<totalExpElReco<<"\n ElRecoPre: "<<totalPreElReco<<std::endl;
  std::cout<<"ElIsoExp: "<<totalExpElIso<<"\n ElIsoPre: "<<totalPreElIso<<std::endl;
	
  outPutFile->cd();
  OutPutTreeExp_->Write();
  OutPutTreePre_->Write();
  FullClosure->Write();
  FullClosureMu->Write();
  FullClosureEl->Write();
  outPutFile->mkdir("Closures");
  TDirectory *dClosures = (TDirectory*)outPutFile->Get("Closures");
  SaveClosure(totalPredictionMuCSMuAcc_->getFullTH1D(), totalExpectationMuAcc_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionMuCSMuReco_->getFullTH1D(), totalExpectationMuReco_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionMuCSMuIso_->getFullTH1D(), totalExpectationMuIso_->getFullTH1D(), dClosures);
	
  SaveClosure(totalPredictionMuCSElAcc_->getFullTH1D(), totalExpectationElAcc_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionMuCSElReco_->getFullTH1D(), totalExpectationElReco_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionMuCSElIso_->getFullTH1D(), totalExpectationElIso_->getFullTH1D(), dClosures);
	
  SaveClosure(totalPredictionElCSMuAcc_->getFullTH1D(), totalExpectationMuAcc_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionElCSMuReco_->getFullTH1D(), totalExpectationMuReco_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionElCSMuIso_->getFullTH1D(), totalExpectationMuIso_->getFullTH1D(), dClosures);
	
  SaveClosure(totalPredictionElCSElAcc_->getFullTH1D(), totalExpectationElAcc_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionElCSElReco_->getFullTH1D(), totalExpectationElReco_->getFullTH1D(), dClosures);
  SaveClosure(totalPredictionElCSElIso_->getFullTH1D(), totalExpectationElIso_->getFullTH1D(), dClosures);
	
  // SaveClosure(totalPredictionIsoTrackReduction_->getFullTH1D(), totalExpectationIsoTrackReduction_->getFullTH1D(), dClosures);
  // SaveClosure(totalPredictionMuIsoTrackReduction_->getFullTH1D(), totalExpectationIsoTrackReduction_->getFullTH1D(), dClosures);
  // SaveClosure(totalPredictionElIsoTrackReduction_->getFullTH1D(), totalExpectationIsoTrackReduction_->getFullTH1D(), dClosures);
	
	
  outPutFile->mkdir("Expectation");
  TDirectory *dExpectation = (TDirectory*)outPutFile->Get("Expectation");
  dExpectation->cd();
  totalExpectation_->saveResults(dExpectation);
  outPutFile->mkdir("Prediction");
  TDirectory *dPrediction = (TDirectory*)outPutFile->Get("Prediction");
  dPrediction->cd();
  totalPrediction_->saveResults(dPrediction);
  totalPredictionMu_->saveResults(dPrediction);
  totalPredictionEl_->saveResults(dPrediction);
	
  outPutFile->mkdir("ControlSample");
  TDirectory *dControlSample = (TDirectory*)outPutFile->Get("ControlSample");
  SaveClosure(ControlSampleEl_->getFullTH1D(), ControlSampleMu_->getFullTH1D(), dControlSample);
  dControlSample->cd();
  ControlSampleMu_->saveResults(dControlSample);
  ControlSampleEl_->saveResults(dControlSample);
	
}


SearchBins::SearchBins()
{
	
  // HTmin,HTmax,MHTmin,MHTmax,NJetsmin,NJetsmax,BTagsmin,BTagsmax
  // NJets 4,6 BTags=0
  // fixed ht Njets btags all MHT bins
  // bins_.push_back( Bin(500,800,200,500,4,6,-1,0) );
  // bins_.push_back( Bin(800,1200,200,500,4,6,-1,0) );
  // bins_.push_back( Bin(1200,99999,200,500,4,6,-1,0) );
	
  // bins_.push_back( Bin(500,1200,500,750,4,6,-1,0) );
  // bins_.push_back( Bin(1200,99999,500,750,4,6,-1,0) );
	
  // bins_.push_back( Bin(800,99999,750,9999,4,6,-1,0) );
	
  // // NJewts 7,8 BTags=0
  // bins_.push_back( Bin(500,800,200,500,7,8,-1,0) );
  // bins_.push_back( Bin(800,1200,200,500,7,8,-1,0) );
  // bins_.push_back( Bin(1200,99999,200,500,7,8,-1,0) );
	
  // bins_.push_back( Bin(500,1200,500,750,7,8,-1,0) );
  // bins_.push_back( Bin(1200,99999,500,750,7,8,-1,0) );
	
  // bins_.push_back( Bin(800,99999,750,9999,7,8,-1,0) );
	
	
  // // NJewts 9,9999 BTags=0
  // bins_.push_back( Bin(500,800,200,500,9,9999,-1,0) );
  // bins_.push_back( Bin(800,1200,200,500,9,9999,-1,0) );
  // bins_.push_back( Bin(1200,99999,200,500,9,9999,-1,0) );
	
  // bins_.push_back( Bin(500,1200,500,750,9,9999,-1,0) );
  // bins_.push_back( Bin(1200,99999,500,750,9,9999,-1,0) );
	
  // bins_.push_back( Bin(800,99999,750,9999,9,9999,-1,0) );
	
	
	
  // // NJets 4,6 BTags=1
  // // fixed ht Njets btags all MHT bins
  // bins_.push_back( Bin(500,800,200,500,4,6,1,1) );
  // bins_.push_back( Bin(800,1200,200,500,4,6,1,1) );
  // bins_.push_back( Bin(1200,99999,200,500,4,6,1,1) );
	
  // bins_.push_back( Bin(500,1200,500,750,4,6,1,1) );
  // bins_.push_back( Bin(1200,99999,500,750,4,6,1,1) );
	
  // bins_.push_back( Bin(800,99999,750,9999,4,6,1,1) );
	
  // // NJewts 7,8 BTags=0
  // bins_.push_back( Bin(500,800,200,500,7,8,1,1) );
  // bins_.push_back( Bin(800,1200,200,500,7,8,1,1) );
  // bins_.push_back( Bin(1200,99999,200,500,7,8,1,1) );
	
  // bins_.push_back( Bin(500,1200,500,750,7,8,1,1) );
  // bins_.push_back( Bin(1200,99999,500,750,7,8,1,1) );
	
  // bins_.push_back( Bin(800,99999,750,9999,7,8,1,1) );
	
	
  // // NJewts 9,9999 BTags=1
  // bins_.push_back( Bin(500,800,200,500,9,9999,1,1) );
  // bins_.push_back( Bin(800,1200,200,500,9,9999,1,1) );
  // bins_.push_back( Bin(1200,99999,200,500,9,9999,1,1) );
	
  // bins_.push_back( Bin(500,1200,500,750,9,9999,1,1) );
  // bins_.push_back( Bin(1200,99999,500,750,9,9999,1,1) );
	
  // bins_.push_back( Bin(800,99999,750,9999,9,9999,1,1) );
	
	
	
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
      std::cout<<"Error event fits in no bin!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
      result=999;
    }
  // 	if(result<18 && result>16)
  // 	{
  // 		std::cout<<"ResultBin: "<<result+1<<" HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
  // 	}
  if(match>0)
    {
      std::cout<<"Error event fits in more than one bin!!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
    }
  return result+1; // to not use the 0 bin but start counting at 1
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
  // 	std::cout<<"name: "<<name_<<std::endl;
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
