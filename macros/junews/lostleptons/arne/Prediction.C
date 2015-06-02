#define Prediction_cxx
// The class definition in Prediction.h has been generated automatically
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
// Root > T->Process("Prediction.C")
// Root > T->Process("Prediction.C","some options")
// Root > T->Process("Prediction.C+")
//

#include "Prediction.h"
#include <TH2.h>
#include <TStyle.h>


void Prediction::Begin(TTree * /*tree*/)
{
  // The Begin() function is called at the start of the query.
  // When running with PROOF Begin() is only called on the client.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TString option = GetOption();

}

void Prediction::SlaveBegin(TTree * /*tree*/)
{
  // The SlaveBegin() function is called after the Begin() function.
  // When running with PROOF SlaveBegin() is called on each slave server.
  // The tree argument is deprecated (on PROOF 0 is passed).

  TFile *effInput = new TFile("macros/junews/lostleptons/arne/Efficiencies/Efficiencies.root","UPDATE");
  cout << "Open efficiency file: " << effInput->GetName() << endl;
  TDirectory *EffInputFolder =   (TDirectory*)effInput->Get("Efficiencies");
  // if(UseTagAndProbeEff_)
  //   {
  //     TDirectory *EffTagAndProbeInputFolder =   (TDirectory*)effInput->Get("TagAndProbe");
  //     MuIsoPTActivityTAPMC_ = (TH2D*)EffTagAndProbeInputFolder->Get("MuIsoTagAndProbeMC");
  //     MuRecoPTActivityTAPMC_ = (TH2D*)EffTagAndProbeInputFolder->Get("MuRecoTagAndProbeMC");
  //     ElecIsoPTActivityTAPMC_ = (TH2D*)EffTagAndProbeInputFolder->Get("ElecIsoTagAndProbeMC");
  //     ElecRecoPTActivityTAPMC_ = (TH2D*)EffTagAndProbeInputFolder->Get("ElecRecoTagAndProbeMC");
  //   }
  //    MuMTWPTActivity_ = (TH2D*) EffInputFolder->Get("MuMTWPTActivity");
  cout << "Get muon efficiencies" << endl;
  MuMTWNJets_ = (TH1D*) EffInputFolder->Get("MuMTWNJets1D");
  MuDiLepContributionMTWAppliedNJets_ = (TH1D*) EffInputFolder->Get("MuDiLepContributionMTWNJets1D");
  MuDiLepEffMTWAppliedNJets_ = (TH1D*) EffInputFolder->Get("MuDiLepMTWNJets1D");
  MuIsoPTActivity_ = (TH2D*)EffInputFolder->Get("MuIsoPTActivity");
  //     MuRecoActivitiy_ = (TH1D*)EffInputFolder->Get("MuRecoActivity");
  MuRecoPTActivity_= (TH2D*)EffInputFolder->Get("MuRecoPTActivity");
  //    MuAccHTNJets_ = (TH2D*)EffInputFolder->Get("MuAccHTNJets");
  // 	 MuAccBTagNJets_ = (TH2D*)EffInputFolder->Get("MuAccBTagNJets");
  MuAccMHTNJets_ = (TH2D*)EffInputFolder->Get("MuAccMHTNJets");
  MuPurityMHTNJets_ = (TH2D*)EffInputFolder->Get("MuonPurity");

  cout << "Get electron efficiencies" << endl;
  ElecIsoPTActivity_ = (TH2D*)EffInputFolder->Get("ElecIsoPTActivity");
  //     ElecRecoActivity_ = (TH1D*)EffInputFolder->Get("ElecRecoActivity");
  ElecRecoPTActivity_= (TH2D*)EffInputFolder->Get("ElecRecoPTActivity");
  //    ElecAccHTNJets_ = (TH2D*)EffInputFolder->Get("ElecAccHTNJets");
  // 	 ElecAccBTagNJets_ = (TH2D*)EffInputFolder->Get("ElecAccBTagNJets");
  ElecAccMHTNJets_ = (TH2D*)EffInputFolder->Get("ElecAccMHTNJets");
	 
  ElecPurityMHTNJets_ = (TH2D*)EffInputFolder->Get("ElecPurity");
  // 	 ElecMTWPTActivity_ = (TH2D*)EffInputFolder->Get("ElecMTWPTActivity");
  ElecMTWNJets_ = (TH1D*) EffInputFolder->Get("ElecMTWNJets1D");
  ElecDiLepContributionMTWAppliedNJets_ = (TH1D*) EffInputFolder->Get("ElecDiLepContributionMTWNJets1D");
  ElecDiLepEffMTWAppliedNJets_ = (TH1D*) EffInputFolder->Get("ElecDiLepMTWNJets1D");
	 
  // load TEfficiencies
  cout << "Load TEfficiencies" << endl;
  TDirectory *TEffInputFolder =   (TDirectory*)effInput->Get("TEfficiencies");
	 
  
  // 	 MuMTWPTActivityEff_ =  new THFeff(TEffInputFolder,"MuMTWPTActivity_Eff");
  MuMTWNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuMTWNJets1D_Eff"));
  MuDiLepContributionMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuDiLepContributionMTWNJets1D_Eff"));
  MuDiLepEffMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuDiLepMTWNJets1D_Eff"));
  MuIsoPTActivityEff_ =  new THFeff(TEffInputFolder,"MuIsoPTActivity_Eff");
  MuRecoPTActivityEff_=  new THFeff(TEffInputFolder,"MuRecoPTActivity_Eff");
  MuAccMHTNJetsEff_ =  new THFeff(TEffInputFolder,"MuAccMHTNJets_Eff");
  ElecIsoPTActivityEff_ =  new THFeff(TEffInputFolder,"ElecIsoPTActivity_Eff");

  ElecRecoPTActivityEff_=  new THFeff(TEffInputFolder,"ElecRecoPTActivity_Eff");
  ElecAccMHTNJetsEff_ =  new THFeff(TEffInputFolder,"ElecAccMHTNJets_Eff");
	 
  ElecPurityMHTNJetsEff_ =  new THFeff(TEffInputFolder,"ElecPurity_Eff");
  // 	 ElecMTWPTActivityEff_ =  new THFeff(TEffInputFolder,"ElecMTWPTActivity");
  ElecMTWNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ElecMTWNJets1D_Eff"));
  ElecDiLepContributionMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ElecDiLepContributionMTWNJets1D_Eff"));
  ElecDiLepEffMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ElecDiLepMTWNJets1D_Eff"));
	 
  // load search variables and binned efficiencies
   cout << "Load search variables and binned efficiencies" << endl;
   searchBinsRef_ = new SearchBins();
  TDirectory *TEffSearchBinsInputFolder =   (TDirectory*)effInput->Get("TEfficienciesSearchBins");
  MuMTWSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("MuMTWSearchBinEff") )->Get("MuMTWSearchBinEff") );
  MuDiLepContributionMTWAppliedSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("MuDiLepContributionMTWAppliedSearchBinEff") )->Get("MuDiLepContributionMTWAppliedSearchBinEff") );
  MuIsoSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("MuIsoSearchBinEff") )->Get("MuIsoSearchBinEff") );
  MuRecoSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("MuRecoSearchBinEff") )->Get("MuRecoSearchBinEff") );
  MuAccSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("MuAccSearchBinEff") )->Get("MuAccSearchBinEff") );
  MuDiLepEffMTWAppliedSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("MuDiLepEffMTWAppliedSearchBinEff") )->Get("MuDiLepEffMTWAppliedSearchBinEff") );
	 
  ElecIsoSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecIsoSearchBinEff") )->Get("ElecIsoSearchBinEff") );
  ElecRecoSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecRecoSearchBinEff") )->Get("ElecRecoSearchBinEff") );
  ElecAccSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecAccSearchBinEff") )->Get("ElecAccSearchBinEff") );
  ElecPuritySearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecPuritySearchBinEff") )->Get("ElecPuritySearchBinEff") );
  ElecMTWSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecMTWSearchBinEff") )->Get("ElecMTWSearchBinEff") );
  ElecDiLepContributionMTWAppliedSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecDiLepContributionMTWAppliedSearchBinEff") )->Get("ElecDiLepContributionMTWAppliedSearchBinEff") );
  ElecDiLepEffMTWAppliedSearchBinEff_ = new THFeff( (TGraphAsymmErrors*) ( (TDirectory*) TEffSearchBinsInputFolder->Get("ElecDiLepEffMTWAppliedSearchBinEff") )->Get("ElecDiLepEffMTWAppliedSearchBinEff") );


  
  TString option = GetOption();
  tPrediction_ = new TTree("LostLeptonPrediction","a simple Tree with simple variables");

  tPrediction_->Branch("isData", &isData_, "isData/s");
  tPrediction_->Branch("Weight", &weightppb, "Weight/F");
  tPrediction_->Branch("Bin",&Bin_,"Bin/s");
  tPrediction_->Branch("searchBin",&searchBin_,"searchBin/s");

  tPrediction_->Branch("HT",&ht30,"HT/F");
  tPrediction_->Branch("MHT",&mht30,"MHT/F");
  tPrediction_->Branch("NJets",&num_jets_pt30,"NJets/s");
  tPrediction_->Branch("BTags",&num_csvm_jets30,"BTags/s");
  tPrediction_->Branch("minDeltaPhiN",&min_delta_phi_met_N,"minDeltaPhiN/F");
  tPrediction_->Branch("MTW",&mtw,"MTW/F");
  tPrediction_->Branch("MET",&met,"MET/F");

  tPrediction_->Branch("Bin",&Bin_,"Bin/s");
  //tPrediction_->Branch("Leptons",&Leptons,"Leptons/s");

  tPrediction_->Branch("selectedIDIsoElectronsNum",&num_veto_els,"selectedIDIsoElectronsNum/s");
  tPrediction_->Branch("selectedIDIsoMuonsNum",&num_veto_mus,"selectedIDIsoMuonsNum/s");
  tPrediction_->Branch("GenElecNum",&num_true_els,"GenElecNum/s");
  tPrediction_->Branch("GenMuNum",&num_true_mus,"GenMuNum/s");
  // 
 
  tPrediction_->Branch("muMTWEff",&muMTWEff_,"muMTWEff/F");
  tPrediction_->Branch("mtwCorrectedWeight",&mtwCorrectedWeight_,"mtwCorrectedWeight/F");
  tPrediction_->Branch("muDiLepContributionMTWAppliedEff",&muDiLepContributionMTWAppliedEff_,"muDiLepContributionMTWAppliedEff/F");
  tPrediction_->Branch("mtwDiLepCorrectedWeight",&mtwDiLepCorrectedWeight_,"mtwDiLepCorrectedWeight/F");
  tPrediction_->Branch("muIsoWeight",&muIsoWeight_,"muIsoWeight/F");
  tPrediction_->Branch("muIsoEff",&muIsoEff_,"muIsoEff/F");
  tPrediction_->Branch("muRecoWeight",&muRecoWeight_,"muRecoWeight/F");
  tPrediction_->Branch("muRecoEff",&muRecoEff_,"muRecoEff/F");
  tPrediction_->Branch("muAccWeight",&muAccWeight_,"muAccWeight/F");
  tPrediction_->Branch("muAccEff",&muAccEff_,"muAccEff/F");
  tPrediction_->Branch("muTotalWeight",&muTotalWeight_,"muTotalWeight/F");
  tPrediction_->Branch("muTotalWeightData",&muTotalWeightData_,"muTotalWeightData/F");
  tPrediction_->Branch("totalMuons",&totalMuons_,"totalMuons/F");
  tPrediction_->Branch("elecAccWeight",&elecAccWeight_,"elecAccWeight/F");
  tPrediction_->Branch("elecAccEff",&elecAccEff_,"elecAccEff/F");
  tPrediction_->Branch("elecRecoWeight",&elecRecoWeight_,"elecRecoWeight/F");
  tPrediction_->Branch("elecRecoEff",&elecRecoEff_,"elecRecoEff/F");
  tPrediction_->Branch("elecIsoWeight",&elecIsoWeight_,"elecIsoWeight/F");
  tPrediction_->Branch("elecIsoEff",&elecIsoEff_,"elecIsoEff/F");
  tPrediction_->Branch("elecTotalWeight",&elecTotalWeight_,"elecTotalWeight/F");
  tPrediction_->Branch("elecTotalWeightData",&elecTotalWeightData_,"elecTotalWeightData/F");
  tPrediction_->Branch("totalElectrons",&totalElectrons_,"totalElectrons/F");
  tPrediction_->Branch("totalWeight",&totalWeight_,"totalWeight/F");
  tPrediction_->Branch("muDiLepEffMTWAppliedEff",&muDiLepEffMTWAppliedEff_,"muDiLepEffMTWAppliedEff/F");
  tPrediction_->Branch("elecDiLepEffMTWAppliedEff",&elecDiLepEffMTWAppliedEff_,"elecDiLepEffMTWAppliedEff/F");
  tPrediction_->Branch("totalWeightDiLep",&totalWeightDiLep_,"totalWeightDiLep/F");
  tPrediction_->Branch("totalWeightDiLepAvg",&totalWeightDiLepAvg_,"totalWeightDiLepAvg/F");

  
  GetOutputList()->Add(tPrediction_);
  SearchBins_ = new SearchBins();
  std::cout<<"Applying filters: "<<applyFilters_<<std::endl;
  std::cout<<"----------------"<<std::endl;
  std::cout<<"UseUpdatedTEfficiencies: "<<UseUpdatedTEfficiencies_<<std::endl;
  //  std::cout<<"UseTagAndProbeEff: "<<UseTagAndProbeEff_<<std::endl;
  
}

Bool_t Prediction::Process(Long64_t entry)
{

  resetValues();
  fChain->GetTree()->GetEntry(entry);
  double HT=ht30, MHT=mht30;
  int NJets=num_jets_pt30, BTags=num_csvm_jets30;
  double minDeltaPhiN=min_delta_phi_met_N;
  weightppb*=10000;
  double Weight=weightppb;
  //if (isData_) Weight=1;
  if(HT<minHT_ || MHT< minMHT_ || NJets < minNJets_  || BTags<2) return kTRUE;
  if (minDeltaPhiN<6) return kTRUE;
  // if (!passesBadJetFilter || PBNRcode<1) return kTRUE;
  // for tests, only looking at 2+ b-jet events

  Bin_ = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);
  // 	if((GenMuNum+GenElecNum)!=1) return kTRUE;
  searchBin_ = searchBinsRef_->GetBinNumber(HT,MHT,NJets,BTags);

  int selectedIDIsoElectronsNum=num_veto_els, selectedIDIsoMuonsNum=num_veto_mus;
  
  if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0) {
    // find the veto mu (we save non-isolated)
    int vetoMu(-1);
    for (unsigned int imu(0); imu<mu_pt->size(); imu++) {
      if (mu_veto->at(imu)) {
	vetoMu=imu;
	break;
      }
    }
    mtw =  mu_mT->at(vetoMu);
    muPurityCorrection_ =  getEff(MuPurityMHTNJets_,MHT,NJets);
    if(!UseUpdatedTEfficiencies_) muMTWEff_ = getEff(MuMTWNJets_,NJets);
    else muMTWEff_ = MuMTWNJetsEff_->GetEff(NJets);
    if(MuMTWSearchBinUse_)muMTWEff_=MuMTWSearchBinEff_->GetEff(searchBin_+0.01);
    purityCorrectedWeight_ = Weight * muPurityCorrection_;
    mtwCorrectedWeight_ = purityCorrectedWeight_ / muMTWEff_;
    if(!UseUpdatedTEfficiencies_) muDiLepContributionMTWAppliedEff_ = getEff(MuDiLepContributionMTWAppliedNJets_,NJets);
    else muDiLepContributionMTWAppliedEff_ = MuDiLepContributionMTWAppliedNJetsEff_->GetEff(NJets);
    if(MuDiLepContributionMTWAppliedEffSearchBinUse_)muDiLepContributionMTWAppliedEff_=MuDiLepContributionMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    mtwDiLepCorrectedWeight_ = mtwCorrectedWeight_ * muDiLepContributionMTWAppliedEff_;
    if(!UseUpdatedTEfficiencies_) muIsoEff_ = getEff(MuIsoPTActivity_, mu_pt->at(vetoMu),mu_activity_mu->at(vetoMu));
    else muIsoEff_ = MuIsoPTActivityEff_->GetEff( mu_pt->at(vetoMu),mu_activity_mu->at(vetoMu));
    if(MuIsoSearchBinUse_)muIsoEff_=MuIsoSearchBinEff_->GetEff(searchBin_+0.01);
    //    if(UseTagAndProbeEff_)muIsoEff_ = getEff(MuIsoPTActivityTAPMC_, mu_pt->at(vetoMu),mu_activity_mu->at(vetoMu)); 
    muIsoWeight_ = mtwDiLepCorrectedWeight_* (1 - muIsoEff_)/muIsoEff_; 
    if(!UseUpdatedTEfficiencies_) muRecoEff_ = getEff(MuRecoPTActivity_,mu_pt->at(vetoMu),mu_activity_mu->at(vetoMu));
    else muRecoEff_ = MuRecoPTActivityEff_->GetEff(mu_pt->at(vetoMu),mu_activity_mu->at(vetoMu));
    if(MuRecoSearchBinUse_)muRecoEff_=MuRecoSearchBinEff_->GetEff(searchBin_+0.01);
    //    if(UseTagAndProbeEff_)muRecoEff_ = getEff(MuRecoPTActivityTAPMC_, mu_pt->at(vetoMu),mu_activity_mu->at(vetoMu)); 
    muRecoWeight_ = mtwDiLepCorrectedWeight_* 1 / muIsoEff_ * (1-muRecoEff_)/muRecoEff_;
    if(!UseUpdatedTEfficiencies_) muAccEff_ = getEff(MuAccMHTNJets_,MHT,NJets);
    else muAccEff_ = MuAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(MuAccSearchBinUse_)muAccEff_=MuAccSearchBinEff_->GetEff(searchBin_+0.01);
    muAccWeight_ = mtwDiLepCorrectedWeight_* 1 / muIsoEff_ * 1 / muRecoEff_ * (1-muAccEff_)/muAccEff_;
    muTotalWeight_ = muIsoWeight_ + muRecoWeight_ + muAccWeight_;
    muTotalWeightData_ = muTotalWeight_/Weight;
    totalMuons_ = mtwDiLepCorrectedWeight_ / ( muIsoEff_ * muRecoEff_ * muAccEff_);
    if(!UseUpdatedTEfficiencies_) elecAccEff_ = getEff(ElecAccMHTNJets_,MHT,NJets);
    else elecAccEff_ = ElecAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(ElecAccSearchBinUse_)elecAccEff_=ElecAccSearchBinEff_->GetEff(searchBin_+0.01);
    elecAccWeight_ = totalMuons_ * (1 - elecAccEff_);
    // if(!UseUpdatedTEfficiencies_) elecRecoEff_ = getEff(ElecRecoPTActivity_,mu_pt->at(vetoMu),ElecActivity(mu_eta->at(vetoMu), mu_phi->at(vetoMu),elActivityMethod_));
    if(!UseUpdatedTEfficiencies_) elecRecoEff_ = getEff(ElecRecoPTActivity_,mu_pt->at(vetoMu), mu_activity_el->at(vetoMu));
    // else elecRecoEff_ = ElecRecoPTActivityEff_->GetEff(mu_pt->at(vetoMu),ElecActivity(mu_eta->at(vetoMu), mu_phi->at(vetoMu),elActivityMethod_));
    else elecRecoEff_ = ElecRecoPTActivityEff_->GetEff(mu_pt->at(vetoMu), mu_activity_el->at(vetoMu));
    if(ElecRecoSearchBinUse_)elecRecoEff_=ElecRecoSearchBinEff_->GetEff(searchBin_+0.01);
    // if(UseTagAndProbeEff_)elecRecoEff_ = getEff(ElecRecoPTActivityTAPMC_, mu_pt->at(vetoMu),ElecActivity(mu_eta->at(vetoMu), mu_phi->at(vetoMu),elActivityMethod_)); 
    //    if(UseTagAndProbeEff_)elecRecoEff_ = getEff(ElecRecoPTActivityTAPMC_, mu_pt->at(vetoMu), mu_activity_el->at(vetoMu)); 
    elecRecoWeight_ = totalMuons_ * (elecAccEff_) * (1-elecRecoEff_);
    //    if(!UseUpdatedTEfficiencies_) elecIsoEff_ = getEff(ElecIsoPTActivity_,mu_pt->at(vetoMu),ElecActivity(mu_eta->at(vetoMu), mu_phi->at(vetoMu),elActivityMethod_));
    if(!UseUpdatedTEfficiencies_) elecIsoEff_ = getEff(ElecIsoPTActivity_,mu_pt->at(vetoMu), mu_activity_el->at(vetoMu));
    //    else elecIsoEff_= ElecIsoPTActivityEff_->GetEff(mu_pt->at(vetoMu),ElecActivity(mu_eta->at(vetoMu), mu_phi->at(vetoMu),elActivityMethod_));
    else elecIsoEff_= ElecIsoPTActivityEff_->GetEff(mu_pt->at(vetoMu), mu_activity_el->at(vetoMu));
    if(ElecIsoSearchBinUse_)elecIsoEff_=ElecIsoSearchBinEff_->GetEff(searchBin_+0.01);
    // if(UseTagAndProbeEff_)elecIsoEff_ = getEff(ElecIsoPTActivityTAPMC_, mu_pt->at(vetoMu),ElecActivity(mu_eta->at(vetoMu), mu_phi->at(vetoMu),elActivityMethod_)); 
    elecIsoWeight_ = totalMuons_ * (elecAccEff_) * (elecRecoEff_) * (1-elecIsoEff_);
    elecTotalWeight_ = elecIsoWeight_ + elecRecoWeight_ + elecAccWeight_;
    elecTotalWeightData_ = elecTotalWeight_/Weight;
    totalWeight_ = elecTotalWeight_ + muTotalWeight_;
    if(!UseUpdatedTEfficiencies_) muDiLepEffMTWAppliedEff_ = getEff(MuDiLepEffMTWAppliedNJets_,NJets);
    else muDiLepEffMTWAppliedEff_= MuDiLepContributionMTWAppliedNJetsEff_->GetEff(NJets);
    if(MuDiLepContributionMTWAppliedEffSearchBinUse_)muDiLepEffMTWAppliedEff_=MuDiLepEffMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    totalWeightDiLep_ = totalWeight_ + (1-muDiLepContributionMTWAppliedEff_) * mtwCorrectedWeight_ * (1-muDiLepEffMTWAppliedEff_)/muDiLepEffMTWAppliedEff_;
    totalWeightDiLepAvg_ = totalWeightDiLep_/2.;
  } else if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1) {
    // find the veto el (we save non-isolated)
    int vetoElec(-1);
    for (unsigned int iel(0); iel<el_pt->size(); iel++) {
      if (el_veto->at(iel)) {
	vetoElec=iel;
	break;
      }
    }
    mtw = el_mT->at(vetoElec);
    if(!UseUpdatedTEfficiencies_) elecPurityCorrection_ =  getEff(ElecPurityMHTNJets_,MHT,NJets);
    else elecPurityCorrection_ = ElecRecoPTActivityEff_->GetEff(MHT,NJets);
    if(ElecPuritySearchBinEff_) elecPurityCorrection_ = ElecPuritySearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) elecMTWEff_ = getEff(ElecMTWNJets_,NJets);
    else elecMTWEff_ = ElecMTWNJetsEff_->GetEff(NJets);
    if(ElecMTWSearchBinUse_)elecMTWEff_=ElecMTWSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) elecIsoEff_ =  getEff(ElecIsoPTActivity_,el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    else elecIsoEff_ = ElecIsoPTActivityEff_->GetEff(el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    if(ElecIsoSearchBinUse_)elecIsoEff_=ElecIsoSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) elecRecoEff_ = getEff(ElecRecoPTActivity_,el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    else elecRecoEff_ = ElecRecoPTActivityEff_->GetEff(el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    if(ElecRecoSearchBinUse_)elecRecoEff_=ElecRecoSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) elecAccEff_ = getEff(ElecAccMHTNJets_,MHT,NJets);
    else elecAccEff_ = ElecAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(ElecAccSearchBinUse_)elecAccEff_=ElecAccSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) muAccEff_ = getEff(MuAccMHTNJets_,MHT,NJets);
    else muAccEff_ = MuAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(MuAccSearchBinUse_)muAccEff_=MuAccSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) muRecoEff_ = getEff(MuRecoPTActivity_, el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    else muRecoEff_ = MuRecoPTActivityEff_->GetEff(el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    if(MuRecoSearchBinUse_)muRecoEff_=MuRecoSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) muIsoEff_ = getEff(MuIsoPTActivity_, el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    else muIsoEff_ = MuIsoPTActivityEff_->GetEff(el_pt->at(vetoElec),el_activity_el->at(vetoElec));
    if(MuIsoSearchBinUse_)muIsoEff_=MuIsoSearchBinEff_->GetEff(searchBin_+0.01);
    purityCorrectedWeight_ = Weight * elecPurityCorrection_;
    mtwCorrectedWeight_ =  purityCorrectedWeight_ / elecMTWEff_;
    if(!UseUpdatedTEfficiencies_) elecDiLepContributionMTWAppliedEff_ = getEff(ElecDiLepContributionMTWAppliedNJets_,NJets);
    else elecDiLepContributionMTWAppliedEff_ = ElecDiLepContributionMTWAppliedNJetsEff_->GetEff(NJets);
    if(ElecDiLepContributionMTWAppliedEffSearchBinUse_)elecDiLepContributionMTWAppliedEff_=ElecDiLepContributionMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    mtwDiLepCorrectedWeight_ = mtwCorrectedWeight_ * elecDiLepContributionMTWAppliedEff_;
    elecIsoWeight_= mtwDiLepCorrectedWeight_ * (1 - elecIsoEff_)/elecIsoEff_;
    elecRecoWeight_ = mtwDiLepCorrectedWeight_* 1 / elecIsoEff_ * (1-elecRecoEff_)/elecRecoEff_;
    elecAccWeight_ = mtwDiLepCorrectedWeight_* 1 / elecIsoEff_ * 1 / elecRecoEff_ * (1-elecAccEff_)/elecAccEff_;
    elecTotalWeight_ = elecIsoWeight_ + elecRecoWeight_ + elecAccWeight_;
    elecTotalWeightData_ = elecTotalWeight_/Weight;
    totalElectrons_ = mtwDiLepCorrectedWeight_ / ( elecIsoEff_ * elecRecoEff_ * elecAccEff_);
    muAccWeight_ = totalElectrons_ * (1-muAccEff_);
    muRecoWeight_ = totalElectrons_ * (muAccEff_) * (1-muRecoEff_);
    muIsoWeight_ = totalElectrons_ * (muAccEff_) * (muRecoEff_) * (1-muIsoEff_);
    muTotalWeight_ = muIsoWeight_ + muRecoWeight_ + muAccWeight_;
    muTotalWeightData_ = muTotalWeight_/Weight;
    totalWeight_ = elecTotalWeight_ + muTotalWeight_;
    if(!UseUpdatedTEfficiencies_) elecDiLepEffMTWAppliedEff_ = getEff(ElecDiLepEffMTWAppliedNJets_,NJets);
    else elecDiLepEffMTWAppliedEff_ = ElecDiLepEffMTWAppliedNJetsEff_->GetEff(NJets);
    if(ElecDiLepContributionMTWAppliedEffSearchBinUse_)elecDiLepEffMTWAppliedEff_=ElecDiLepContributionMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    totalWeightDiLep_ = totalWeight_ + (1-elecDiLepContributionMTWAppliedEff_) * mtwCorrectedWeight_ * (1-elecDiLepEffMTWAppliedEff_)/elecDiLepEffMTWAppliedEff_;
    totalWeightDiLepAvg_ = totalWeightDiLep_/2.;
  }
  if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0) return kTRUE;
  tPrediction_->Fill();
  return kTRUE;
}

void Prediction::SlaveTerminate()
{
  SearchBins_->PrintUsed();
}

void Prediction::Terminate()
{
  TFile *outPutFile = new TFile("macros/junews/lostleptons/arne/Prediction/Prediction_data.root","RECREATE"); ;
  outPutFile->cd();
  tPrediction_->Write();
  outPutFile->Close();
}

void Prediction::resetValues()
{
  mtw=0.;
  muMTWEff_=0.;
  mtwCorrectedWeight_=0.;
  muDiLepContributionMTWAppliedEff_=0.;
  mtwDiLepCorrectedWeight_=0.;
  muIsoEff_=0;
  muIsoWeight_=0;
  muRecoEff_=0;
  muRecoWeight_=0;
  muAccEff_=0;
  muAccWeight_=0;
  muTotalWeight_=0.;
  muTotalWeightData_ =0;
  totalMuons_=0;
  elecIsoEff_=0;
  elecIsoWeight_=0;
  elecRecoEff_=0;
  elecRecoWeight_=0;
  elecAccEff_=0;
  elecAccWeight_=0;
  elecTotalWeight_=0;
  elecTotalWeightData_ = 0;
  totalElectrons_=0.;
  totalWeight_=0.;
  muDiLepEffMTWAppliedEff_=0.;
  elecDiLepEffMTWAppliedEff_=0.;
  totalWeightDiLep_=0.;
  totalWeightDiLepAvg_=0.;
  // HT=0.;
  // MHT=0.;
  // NJets=0.;
  // BTags=0.;
  // Weight=0.;
}


double Prediction::effUp(double eff, double up)
{
  double result = eff + up;
  if(result>0.995)result=0.995;
  return result;
}
double Prediction::effDown(double eff, double down)
{
  double result = eff - down;
  if(result<0.01)result=eff * 0.1;
  return result;
}

double Prediction::getEff(TH2D* effTH2D, double xValue, double yValue)
{
  double result = 0;
  if(xValue < effTH2D->GetXaxis()->GetXmin() )
    {
      // 		std::cout<<"Warning xValue: "<<xValue<<" is smaller than minimum of histo: "<<effTH2D->GetName()<<std::endl;
      xValue= effTH2D->GetXaxis()->GetXmin()+0.01;
    }
  else if(xValue > effTH2D->GetXaxis()->GetXmax() )
    {
      // 	  std::cout<<"Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<effTH2D->GetName()<<" which is: "<<effTH2D->GetXaxis()->GetXmax();
      xValue= effTH2D->GetXaxis()->GetXmax()-0.01;
      // 		std::cout<<" Setting xValue to: "<<xValue<<std::endl;
      if(xValue > effTH2D->GetXaxis()->GetXmax() )std::cout<<"Problem persists!!!!!!!!!"<<std::endl;
    }
	
  if(yValue < effTH2D->GetYaxis()->GetXmin() )
    {
      // 		std::cout<<"Warning yValue: "<<yValue<<" is smaller than minimum of histo: "<<effTH2D->GetName()<<std::endl;
      yValue= effTH2D->GetYaxis()->GetXmin()+0.01;
    }
  else if(yValue > effTH2D->GetYaxis()->GetXmax() )
    {
      // 		std::cout<<"Warning yValue: "<<yValue<<" is bigger than maximum of histo: "<<effTH2D->GetName()<<std::endl;
      yValue= effTH2D->GetYaxis()->GetXmax()-0.01;
    }
  result = effTH2D->GetBinContent(effTH2D->GetXaxis()->FindBin(xValue),effTH2D->GetYaxis()->FindBin(yValue));
  if(result<0.01)
    {
      std::cout<<"Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<effTH2D->GetName()<<std::endl;
      result =0.01;
    }
  if(result>1)
    {
      std::cout<<"Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<effTH2D->GetName()<<std::endl;
      result =0.99;
    }
  return result;
}
double Prediction::getEff(TH1D* effTH1D, double xValue)
{
  double result = 0;
  if(xValue < effTH1D->GetXaxis()->GetXmin() )
    {
      std::cout<<"Warning xValue: "<<xValue<<" is smaller than minimum of histo: "<<effTH1D->GetName()<<std::endl;
      xValue= effTH1D->GetXaxis()->GetXmin()+0.01;
    }
  else if(xValue > effTH1D->GetXaxis()->GetXmax() )
    {
      std::cout<<"Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<effTH1D->GetName()<<" which is: "<<effTH1D->GetXaxis()->GetXmax();
      xValue= effTH1D->GetXaxis()->GetXmax()-0.01;
      std::cout<<" Setting xValue to: "<<xValue<<std::endl;
      if(xValue > effTH1D->GetXaxis()->GetXmax() )std::cout<<"Problem persists!!!!!!!!!"<<std::endl;
    }
  
  
  result = effTH1D->GetBinContent(effTH1D->GetXaxis()->FindBin(xValue));
  if(result<0.01)
    {
      std::cout<<"Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<effTH1D->GetName()<<std::endl;
      result =0.01;
    }
  if(result>1)
    {
      std::cout<<"Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<effTH1D->GetName()<<std::endl;
      result =0.99;
    }
  return result;
}

THFeff::THFeff(TDirectory* inputFolder, const char* FolderName)
{
  useTH2f_=true;
  FolderName_=FolderName;
  TDirectory* EffFolder = (TDirectory*)inputFolder->Get(FolderName);
  TList* effList = (TList*)EffFolder->GetListOfKeys();
  effList->Print();
  TIter next(EffFolder->GetListOfKeys());
  TKey *key;
  int count =0;
  while ( (key = (TKey*)next()) )
    {
      TObject *object = key->ReadObj();
      if(object->IsA()->InheritsFrom( TH2::Class() ) )
	{
	  // 			std::cout<<"object: "<<object->GetName()<<" is a th2f\n";
	  refTH2D_ = (TH2D*)object;
	}
      if(object->IsA()->InheritsFrom( TGraphAsymmErrors::Class() ) )
	{
	  // 			std::cout<<"object: "<<object->GetName()<<" is a TGraphAsymmErrors\n";
	  TGraphAsymmErrorsVec_.push_back( (TGraphAsymmErrors*)object  );
	}
      count++;
    }
  // 	for(unsigned int i=0; i<TGraphAsymmErrorsVec_.size();i++)
  // 	{
  // 		std::cout<<"TGraph["<<i<<"]: "<<TGraphAsymmErrorsVec_[i]->GetTitle()<<std::endl;
  // 	}
  xMax_=0; xMin_=0; yMax_=0; yMin_=0;
  xMax_= refTH2D_->GetXaxis()->GetXmax();
  xMin_=refTH2D_->GetXaxis()->GetXmin();
  yMax_= refTH2D_->GetYaxis()->GetXmax();
  yMin_=refTH2D_->GetYaxis()->GetXmin();
  // 	std::cout<<"Eff: "<<refTH2D_->GetName()<<" range: ["<<xMin_<<","<<xMax_<<"],["<<yMin_<<","<<yMax_<<"]\n"; 
}
THFeff::THFeff(TGraphAsymmErrors* inputGraph)
{
  useTH2f_=false; 
  TGraphAsymmErrorsRef_=inputGraph; 
  const unsigned int points = TGraphAsymmErrorsRef_->GetN();
  std::vector<Double_t> values(TGraphAsymmErrorsRef_->GetX(),TGraphAsymmErrorsRef_->GetX() + points);
  xMax_=-999;
  xMin_=99999;
  for(unsigned int i=0; i<values.size();i++)
    {
      if(xMax_<values[i]+TGraphAsymmErrorsRef_->GetErrorXhigh(i))xMax_=values[i]+TGraphAsymmErrorsRef_->GetErrorXhigh(i);
      if(xMin_>values[i]-TGraphAsymmErrorsRef_->GetErrorXlow(i))xMin_=values[i]-TGraphAsymmErrorsRef_->GetErrorXlow(i);
    }

  std::cout<<"TEff1D: xMax: "<<xMax_<<" xMin: "<<xMin_<<std::endl;
	
}
double THFeff::GetEff(double xValue, double yValue)
{
  if(!useTH2f_)
    {
      std::cout<<"THFeff::ERROR: Wrong use of THFeff trying to retrieve 2d eff from a 1d map!!!!!! crash...."<<std::endl;
      return -99999;
    }
  double result =0;
  if(xValue>xMax_)
    {
      std::cout<<"THFeff::Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<refTH2D_->GetName()<<" which is: "<<xMax_;
      xValue= xMax_-0.001;
      std::cout<<" Setting xValue to: "<<xValue<<std::endl;
    }
  if(xValue<xMin_)
    {
      std::cout<<"THFeff::Warning xValue: "<<xValue<<" is smaller than min of histo: "<<refTH2D_->GetName()<<" which is: "<<xMin_;
      xValue= xMin_+0.001;
      std::cout<<" Setting xValue to: "<<xValue<<std::endl;
    }
  if(yValue>yMax_)
    {
      std::cout<<"THFeff::Warning yValue: "<<yValue<<" is bigger than maximum of histo: "<<refTH2D_->GetName()<<" which is: "<<yMax_;
      yValue= yMax_-0.001;
      std::cout<<" Setting xValue to: "<<yValue<<std::endl;
    }
  if(yValue<yMin_)
    {
      std::cout<<"THFeff::Warning yValue: "<<yValue<<" is smaller than min of histo: "<<refTH2D_->GetName()<<" which is: "<<yMin_;
      yValue= yMin_+0.001;
      std::cout<<" Setting yValue to: "<<yValue<<std::endl;
    }
  unsigned int yHistoIndex =refTH2D_->GetYaxis()->FindBin(yValue);
  if(yHistoIndex<1 || yHistoIndex>(TGraphAsymmErrorsVec_.size()+1))
    {
      std::cout<<"THFeff::Error selected HistoIndex for refTH2D: "<<refTH2D_->GetName()<<" is out of bounce: "<<yHistoIndex<<std::endl;
    }
  result = TGraphAsymmErrorsVec_[yHistoIndex-1]->Eval(xValue);
  if(result<0.01)
    {
      std::cout<<"THFeff::Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<refTH2D_->GetName()<<std::endl;
      result =0.01;
    }
  if(result>1)
    {
      std::cout<<"THFeff::Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<refTH2D_->GetName()<<std::endl;
      result =0.99;
    }
	
  return result;
}

double THFeff::GetEff(double xValue)
{
  if(useTH2f_)
    {
      std::cout<<"ERROR: Wrong use of THFeff trying to retrieve 1d eff from a 2d map!!!!!! crash...."<<std::endl;
      return -99999;
    }
  double result=0;
  if(xValue>xMax_)
    {
      std::cout<<"THFeff::Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<TGraphAsymmErrorsRef_->GetName()<<" which is: "<<xMax_;
      xValue= xMax_-0.001;
      std::cout<<" Setting xValue to: "<<xValue<<std::endl;
    }
  if(xValue<xMin_)
    {
      std::cout<<"THFeff::Warning xValue: "<<xValue<<" is smaller than min of histo: "<<TGraphAsymmErrorsRef_->GetName()<<" which is: "<<xMin_;
      xValue= xMin_+0.001;
      std::cout<<" Setting xValue to: "<<xValue<<std::endl;
    }
  result = TGraphAsymmErrorsRef_->Eval(xValue);
  if(result<0.01)
    {
      std::cout<<"THFeff::Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<TGraphAsymmErrorsRef_->GetName()<<std::endl;
      result =0.01;
    }
  if(result>1)
    {
      std::cout<<"THFeff::Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<TGraphAsymmErrorsRef_->GetName()<<std::endl;
      result =0.99;
    }
  return result;
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
  std::cout<<"Loaded bins: "<<bins_.size()<<std::endl;
  for(unsigned int i=0; i<bins_.size();i++)
    {
      usedBin_.push_back(0); 
    }
}

unsigned int SearchBins::GetBinNumber(double HT, double MHT, int NJets, int BTags)
{
  unsigned int result =999;
  int match =-1;

  // std::cout<<"HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<< BTags << "\n";

  for(unsigned int i=0; i<bins_.size();i++)
    {

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
	  usedBin_[i]=usedBin_[i]+1;
	  // std::cout<<"Bin["<<i<<"]: HT["<<bins_[i].HTmin_<<","<<bins_[i].HTmax_<<"] MHT["<<bins_[i].MHTmin_<<","<<bins_[i].MHTmax_<<"] NJets["<<bins_[i].NJetsmin_<<","<<bins_[i].NJetsmax_<<"] BTags["<<bins_[i].BTagsmin_<<","<<bins_[i].BTagsmax_<<"]\n";
	}
    }
  if(match==-1)
    {
      std::cout<<"Error event fits in no bin!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
      result=999;
    }
  if(match>0)
    {
      std::cout<<"Error event fits in more than one bin!!!! HT: "<<HT<<", MHT: "<<MHT<<", NJets: "<<NJets<<", BTags: "<<BTags<<std::endl;
    }
  return result+1; // to not use the 0 bin but start counting at 1
}
void SearchBins::PrintUsed()
{
  for(unsigned int i=0; i< usedBin_.size();i++) std::cout<<"Bin["<<i<<"] has been used: "<<usedBin_[i]<<std::endl; 
}
