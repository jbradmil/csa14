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
#include <iostream>


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
  
  TFile *effInput = new TFile("Efficiencies.root","UPDATE");
  TDirectory *EffInputFolder =   (TDirectory*)effInput->Get("Efficiencies");
  if(UseTagAndProbeEffIso_)
  {
    TDirectory *EffTagAndProbeInputFolder =   (TDirectory*)effInput->Get("TagAndProbe");
    MuIsoPTActivityTAPMC_ = (TH2F*)EffTagAndProbeInputFolder->Get("MuIsoTagAndProbeMC");
    
    ElecIsoPTActivityTAPMC_ = (TH2F*)EffTagAndProbeInputFolder->Get("ElecIsoTagAndProbeMC");
    
  }
  if(UseTagAndProbeEffReco_)
  {
    TDirectory *EffTagAndProbeInputFolder =   (TDirectory*)effInput->Get("TagAndProbe");
    MuRecoPTActivityTAPMC_ = (TH2F*)EffTagAndProbeInputFolder->Get("MuRecoTagAndProbeMC");
    ElecRecoPTActivityTAPMC_ = (TH2F*)EffTagAndProbeInputFolder->Get("ElecRecoTagAndProbeMC");
    
  }
  //    MuMTWPTActivity_ = (TH2F*) EffInputFolder->Get("MuMTWPTActivity");
  MuMTWNJets_ = (TH1F*) EffInputFolder->Get("MuMTWNJets1D");
  MuDiLepContributionMTWAppliedNJets_ = (TH1F*) EffInputFolder->Get("MuDiLepContributionMTWNJets1D");
  MuDiLepEffMTWAppliedNJets_ = (TH1F*) EffInputFolder->Get("MuDiLepMTWNJets1D");
  MuIsoPTActivity_ = (TH2F*)EffInputFolder->Get("MuIsoPTActivity");
  //     MuRecoActivitiy_ = (TH1F*)EffInputFolder->Get("MuRecoActivity");
  MuRecoPTActivity_= (TH2F*)EffInputFolder->Get("MuRecoPTActivity");
  //    MuAccHTNJets_ = (TH2F*)EffInputFolder->Get("MuAccHTNJets");
  //       MuAccBTagNJets_ = (TH2F*)EffInputFolder->Get("MuAccBTagNJets");
  MuAccMHTNJets_ = (TH2F*)EffInputFolder->Get("MuAccMHTNJets");
  MuPurityMHTNJets_ = (TH2F*)EffInputFolder->Get("MuonPurity");
  
  ElecIsoPTActivity_ = (TH2F*)EffInputFolder->Get("ElecIsoPTActivity");
  //     ElecRecoActivity_ = (TH1F*)EffInputFolder->Get("ElecRecoActivity");
  ElecRecoPTActivity_= (TH2F*)EffInputFolder->Get("ElecRecoPTActivity");
  //    ElecAccHTNJets_ = (TH2F*)EffInputFolder->Get("ElecAccHTNJets");
  //       ElecAccBTagNJets_ = (TH2F*)EffInputFolder->Get("ElecAccBTagNJets");
  ElecAccMHTNJets_ = (TH2F*)EffInputFolder->Get("ElecAccMHTNJets");
  
  ElecPurityMHTNJets_ = (TH2F*)EffInputFolder->Get("ElecPurity");
  //       ElecMTWPTActivity_ = (TH2F*)EffInputFolder->Get("ElecMTWPTActivity");
  ElecMTWNJets_ = (TH1F*) EffInputFolder->Get("ElecMTWNJets1D");
  ElecDiLepContributionMTWAppliedNJets_ = (TH1F*) EffInputFolder->Get("ElecDiLepContributionMTWNJets1D");
  ElecDiLepEffMTWAppliedNJets_ = (TH1F*) EffInputFolder->Get("ElecDiLepMTWNJets1D");
  
  // load TEfficiencies
  TDirectory *TEffInputFolder =   (TDirectory*)effInput->Get("TEfficiencies");
  
  MuIsoPTActivityEff_ = new THFeff(TEffInputFolder,"MuIsoPTActivity");
  ElecIsoPTActivityEff_ = new THFeff(TEffInputFolder,"ElecIsoPTActivity");
  MuDiLepContributionMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuDiLepContributionMTWNJets1D") );
  
  
  //       MuMTWPTActivityEff_ =  new THFeff(TEffInputFolder,"MuMTWPTActivity");
  MuMTWNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuMTWNJets1D"));
  MuDiLepContributionMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuDiLepContributionMTWNJets1D"));
  MuDiLepEffMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("MuDiLepMTWNJets1D"));
  MuIsoPTActivityEff_ =  new THFeff(TEffInputFolder,"MuIsoPTActivity");
  MuRecoPTActivityEff_=  new THFeff(TEffInputFolder,"MuRecoPTActivity");
  MuAccMHTNJetsEff_ =  new THFeff(TEffInputFolder,"MuAccMHTNJets");
  ElecIsoPTActivityEff_ =  new THFeff(TEffInputFolder,"ElecIsoPTActivity");
  
  ElecRecoPTActivityEff_=  new THFeff(TEffInputFolder,"ElecRecoPTActivity");
  ElecAccMHTNJetsEff_ =  new THFeff(TEffInputFolder,"ElecAccMHTNJets");
  
  ElecPurityMHTNJetsEff_ =  new THFeff(TEffInputFolder,"ElecPurity");
  //       ElecMTWPTActivityEff_ =  new THFeff(TEffInputFolder,"ElecMTWPTActivity");
  ElecMTWNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ElecMTWNJets1D"));
  ElecDiLepContributionMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ElecDiLepContributionMTWNJets1D"));
  ElecDiLepEffMTWAppliedNJetsEff_ = new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ElecDiLepMTWNJets1D"));
  
  //       ExpectationReductionIsoTrackNJetsEff_ =  new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ExpectationReductionIsoTrackNJets1D"));
  //       ExpectationReductionIsoTrackMHTEff_ =  new THFeff( (TGraphAsymmErrors*) TEffInputFolder->Get("ExpectationReductionIsoTrackMHT1D"));
  ExpectationReductionIsoTrackNJetsEff_ =  (TH1F*)EffInputFolder->Get("ExpectationReductionIsoTrackNJetsEff");
  ExpectationReductionIsoTrackBTagsNJetsEff_= (TH2F*)EffInputFolder->Get("IsoTrackReductionBTagNJets");
  ExpectationReductionMuIsoTrackBTagsNJetsEff_= (TH2F*)EffInputFolder->Get("MuIsoTrackReductionBTagNJets");
  ExpectationReductionElecIsoTrackBTagsNJetsEff_= (TH2F*)EffInputFolder->Get("ElecIsoTrackReductionBTagNJets");
  ExpectationReductionPionIsoTrackBTagsNJetsEff_= (TH2F*)EffInputFolder->Get("PionIsoTrackReductionBTagNJets");
  
  // load search variables bined efficiencies
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
  tPrediction_->Branch("HT",&HT,"HT/F");
  tPrediction_->Branch("MHT",&MHT,"MHT/F");
  tPrediction_->Branch("NJets",&NJets,"NJets/s");
  tPrediction_->Branch("BTags",&BTags,"BTags/s");
  tPrediction_->Branch("NVtx",&NVtx,"NVtx/s");
  tPrediction_->Branch("Bin",&Bin_,"Bin/s");
  tPrediction_->Branch("isoTracks",&isoTracks,"isoTracks/s");
  tPrediction_->Branch("Leptons",&Leptons,"Leptons/s");
  tPrediction_->Branch("DeltaPhi1",&DeltaPhi1,"DeltaPhi1/F");
  tPrediction_->Branch("DeltaPhi2",&DeltaPhi2,"DeltaPhi2/F");
  tPrediction_->Branch("DeltaPhi3",&DeltaPhi3,"DeltaPhi3/F");
  tPrediction_->Branch("minDeltaPhiN",&minDeltaPhiN,"minDeltaPhiN/F");
  tPrediction_->Branch("DeltaPhiN1",&DeltaPhiN1,"DeltaPhiN1/F");
  tPrediction_->Branch("DeltaPhiN2",&DeltaPhiN2,"DeltaPhiN2/F");
  tPrediction_->Branch("DeltaPhiN3",&DeltaPhiN3,"DeltaPhiN3/F");
  tPrediction_->Branch("Weight", &Weight, "Weight/F");
  tPrediction_->Branch("searchBin",&searchBin_,"searchBin/s");
  tPrediction_->Branch("MET",&METPt,"MET/F");
  tPrediction_->Branch("METPhi",&METPhi,"METPhi/F");
  tPrediction_->Branch("selectedIDIsoMuonsNum",&selectedIDIsoMuonsNum,"selectedIDIsoMuonsNum/s");
  tPrediction_->Branch("selectedIDIsoMuonsPt", selectedIDIsoMuonsPt, "selectedIDIsoMuonsPt[selectedIDIsoMuonsNum]/F");
  tPrediction_->Branch("selectedIDIsoMuonsEta", selectedIDIsoMuonsEta, "selectedIDIsoMuonsEta[selectedIDIsoMuonsNum]/F");
  tPrediction_->Branch("selectedIDIsoMuonsPhi", selectedIDIsoMuonsPhi, "selectedIDIsoMuonsPhi[selectedIDIsoMuonsNum]/F");
  tPrediction_->Branch("selectedIDIsoMuonsActivity", selectedIDIsoMuonsActivity, "selectedIDIsoMuonsActivity[selectedIDIsoMuonsNum]/F");
  tPrediction_->Branch("RecoIsoMuonE", selectedIDIsoMuonsE, "RecoIsoMuonE[selectedIDIsoMuonsNum]/F");
  tPrediction_->Branch("selectedIDIsoElectronsNum",&selectedIDIsoElectronsNum,"selectedIDIsoElectronsNum/s");
  tPrediction_->Branch("RecoIsoElecPt", selectedIDIsoElectronsPt, "RecoIsoElecPt[selectedIDIsoElectronsNum]/F");
  tPrediction_->Branch("RecoIsoElecEta", selectedIDIsoElectronsEta, "RecoIsoElecEta[selectedIDIsoElectronsNum]/F");
  tPrediction_->Branch("RecoIsoElecPhi", selectedIDIsoElectronsPhi, "RecoIsoElecPhi[selectedIDIsoElectronsNum]/F");
  tPrediction_->Branch("RecoIsoElecE", selectedIDIsoElectronsE, "RecoIsoElecE[selectedIDIsoElectronsNum]/F");
  tPrediction_->Branch("selectedIDIsoElectronsActivity", selectedIDIsoElectronsActivity, "selectedIDIsoElectronsActivity[selectedIDIsoElectronsNum]/F");
  tPrediction_->Branch("MTW",&mtw,"MTW/F");
  muActivityMethod=muActivityMethod_;
  elecActivityMethod=elecActivityMethod_;
  isoTrackActivityMethod=isoTrackActivityMethod_;
  tPrediction_->Branch("muActivityMethod",&muActivityMethod,"muActivityMethod/s");  
  tPrediction_->Branch("elecActivityMethod",&elecActivityMethod,"elecActivityMethod/s");  
  tPrediction_->Branch("isoTrackActivityMethod",&isoTrackActivityMethod,"isoTrackActivityMethod/s");
  tPrediction_->Branch("muMTWEff",&muMTWEff_,"muMTWEff/F");
  tPrediction_->Branch("mtwCorrectedWeight",&mtwCorrectedWeight_,"mtwCorrectedWeight/F");
  tPrediction_->Branch("muDiLepContributionMTWAppliedEff",&muDiLepContributionMTWAppliedEff_,"muDiLepContributionMTWAppliedEff/F");
  tPrediction_->Branch("mtwDiLepCorrectedWeight",&mtwDiLepCorrectedWeight_,"mtwDiLepCorrectedWeight/F");
  tPrediction_->Branch("totalWeightDiLepIsoTrackReduced",&totalWeightDiLepIsoTrackReduced_,"totalWeightDiLepIsoTrackReduced/F");
  tPrediction_->Branch("totalWeightDiLepIsoMuTrackReduced",&totalWeightDiLepIsoMuTrackReduced_,"totalWeightDiLepIsoMuTrackReduced/F");
  tPrediction_->Branch("totalWeightDiLepIsoElecTrackReduced",&totalWeightDiLepIsoElecTrackReduced_,"totalWeightDiLepIsoElecTrackReduced/F");
  tPrediction_->Branch("totalWeightDiLepIsoPionTrackReduced",&totalWeightDiLepIsoPionTrackReduced_,"totalWeightDiLepIsoPionTrackReduced/F");
  tPrediction_->Branch("totalWeightDiLepIsoTrackReducedCombined",&totalWeightDiLepIsoTrackReducedCombined_,"totalWeightDiLepIsoTrackReducedCombined/F");
  tPrediction_->Branch("muIsoWeight",&muIsoWeight_,"muIsoWeight/F");
  tPrediction_->Branch("muIsoEff",&muIsoEff_,"muIsoEff/F");
  tPrediction_->Branch("muRecoWeight",&muRecoWeight_,"muRecoWeight/F");
  tPrediction_->Branch("muRecoEff",&muRecoEff_,"muRecoEff/F");
  tPrediction_->Branch("muAccWeight",&muAccWeight_,"muAccWeight/F");
  tPrediction_->Branch("muAccEff",&muAccEff_,"muAccEff/F");
  tPrediction_->Branch("muTotalWeight",&muTotalWeight_,"muTotalWeight/F");
  tPrediction_->Branch("totalMuons",&totalMuons_,"totalMuons/F");
  tPrediction_->Branch("elecAccWeight",&elecAccWeight_,"elecAccWeight/F");
  tPrediction_->Branch("elecAccEff",&elecAccEff_,"elecAccEff/F");
  tPrediction_->Branch("elecRecoWeight",&elecRecoWeight_,"elecRecoWeight/F");
  tPrediction_->Branch("elecRecoEff",&elecRecoEff_,"elecRecoEff/F");
  tPrediction_->Branch("elecIsoWeight",&elecIsoWeight_,"elecIsoWeight/F");
  tPrediction_->Branch("elecIsoEff",&elecIsoEff_,"elecIsoEff/F");
  tPrediction_->Branch("elecTotalWeight",&elecTotalWeight_,"elecTotalWeight/F");
	tPrediction_->Branch("IsoTrackReduction",&IsoTrackReduction_,"IsoTrackReduction/F");
	tPrediction_->Branch("IsoMuTrackReduction",&IsoMuTrackReduction_,"IsoMuTrackReduction/F");
	tPrediction_->Branch("IsoElecTrackReduction",&IsoElecTrackReduction_,"IsoElecTrackReduction/F");
	tPrediction_->Branch("IsoPionTrackReduction",&IsoPionTrackReduction_,"IsoPionTrackReduction/F");
	tPrediction_->Branch("IsoTrackReductionCombined",&IsoTrackReductionCombined_,"IsoTrackReductionCombined/F");
  tPrediction_->Branch("expectationReductionIsoTrackEff",&expectationReductionIsoTrackEff_,"expectationReductionIsoTrackEff/F");
  tPrediction_->Branch("expectationReductionMuIsoTrackEff",&expectationReductionMuIsoTrackEff_,"expectationReductionMuIsoTrackEff/F");
  tPrediction_->Branch("expectationReductionElecIsoTrackEff",&expectationReductionElecIsoTrackEff_,"expectationReductionElecIsoTrackEff/F");
  tPrediction_->Branch("expectationReductionPionIsoTrackEff",&expectationReductionPionIsoTrackEff_,"expectationReductionPionIsoTrackEff/F");
  tPrediction_->Branch("expectationReductionIsoTrackEffCombined",&expectationReductionIsoTrackEffCombined_,"expectationReductionIsoTrackEffCombined/F");
  tPrediction_->Branch("totalWeight",&totalWeight_,"totalWeight/F");
  tPrediction_->Branch("muDiLepEffMTWAppliedEff",&muDiLepEffMTWAppliedEff_,"muDiLepEffMTWAppliedEff/F");
  tPrediction_->Branch("elecDiLepEffMTWAppliedEff",&elecDiLepEffMTWAppliedEff_,"elecDiLepEffMTWAppliedEff/F");
  tPrediction_->Branch("totalWeightDiLep",&totalWeightDiLep_,"totalWeightDiLep/F");
  // iso track control sample usage
  tPrediction_->Branch("IsolatedMuonTracksVetoNum",&IsolatedMuonTracksVetoNum,"IsolatedMuonTracksVetoNum/s");
  tPrediction_->Branch("IsolatedMuonTracksVetoPt", IsolatedMuonTracksVetoPt, "IsolatedMuonTracksVetoPt[IsolatedMuonTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedMuonTracksVetoEta", IsolatedMuonTracksVetoEta, "IsolatedMuonTracksVetoEta[IsolatedMuonTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedMuonTracksVetoPhi", IsolatedMuonTracksVetoPhi, "IsolatedMuonTracksVetoPhi[IsolatedMuonTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedMuonTracksVetoE", IsolatedMuonTracksVetoE, "IsolatedMuonTracksVetoE[IsolatedMuonTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedMuonTracksVeto_MT", IsolatedMuonTracksVeto_MT, "IsolatedMuonTracksVeto_MT[IsolatedMuonTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedElectronTracksVetoNum",&IsolatedElectronTracksVetoNum,"IsolatedElectronTracksVetoNum/s");
  tPrediction_->Branch("IsolatedElectronTracksVetoPt", IsolatedElectronTracksVetoPt, "IsolatedElectronTracksVetoPt[IsolatedElectronTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedElectronTracksVetoEta", IsolatedElectronTracksVetoEta, "IsolatedElectronTracksVetoEta[IsolatedElectronTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedElectronTracksVetoPhi", IsolatedElectronTracksVetoPhi, "IsolatedElectronTracksVetoPhi[IsolatedElectronTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedElectronTracksVetoE", IsolatedElectronTracksVetoE, "IsolatedElectronTracksVetoE[IsolatedElectronTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedElectronTracksVeto_MT", IsolatedElectronTracksVeto_MT, "IsolatedElectronTracksVeto_MT[IsolatedElectronTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedPionTracksVetoNum",&IsolatedPionTracksVetoNum,"IsolatedPionTracksVetoNum/s");
  tPrediction_->Branch("IsolatedPionTracksVetoPt", IsolatedPionTracksVetoPt, "IsolatedPionTracksVetoPt[IsolatedPionTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedPionTracksVetoEta", IsolatedPionTracksVetoEta, "IsolatedPionTracksVetoEta[IsolatedPionTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedPionTracksVetoPhi", IsolatedPionTracksVetoPhi, "IsolatedPionTracksVetoPhi[IsolatedPionTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedPionTracksVetoE", IsolatedPionTracksVetoE, "IsolatedPionTracksVetoE[IsolatedPionTracksVetoNum]/F");
  tPrediction_->Branch("IsolatedPionTracksVeto_MT", IsolatedPionTracksVeto_MT, "IsolatedPionTracksVeto_MT[IsolatedPionTracksVetoNum]/F");
  GetOutputList()->Add(tPrediction_);
  SearchBins_ = new SearchBins();
  std::cout<<"Applying filters: "<<applyFilters_<<std::endl;
  std::cout<<"----------------"<<std::endl;
  std::cout<<"UseUpdatedTEfficiencies: "<<UseUpdatedTEfficiencies_<<std::endl;
  std::cout<<"UseTagAndProbeEffIso: "<<UseTagAndProbeEffIso_<<std::endl;
  std::cout<<"UseTagAndProbeEffReco: "<<UseTagAndProbeEffReco_<<std::endl;
  
}

Bool_t Prediction::Process(Long64_t entry)
{
  resetValues();
  fChain->GetTree()->GetEntry(entry);
  isoTracks= isoElectronTracks + isoMuonTracks + isoPionTracks;
  if(HT<minHT_ || MHT< minMHT_ || NJets < minNJets_  ) return kTRUE;
  if(DeltaPhi1 < deltaPhi1_ || DeltaPhi2 < deltaPhi2_ || DeltaPhi3 < deltaPhi3_ )return kTRUE;
  //if(minDeltaPhiN<minDeltaPhiN_) return kTRUE;
  if(applyFilters_ &&  !FiltersPass() ) return kTRUE;
  Bin_ = SearchBins_->GetBinNumber(HT,MHT,NJets,BTags);
  //      if((GenMuNum+GenElecNum)!=1) return kTRUE;
  searchBin_ = searchBinsRef_->GetBinNumber(HT,MHT,NJets,BTags);
  //      expectationReductionIsoTrackEff_= ExpectationReductionIsoTrackMHTEff_->GetEff(MHT);
  //      expectationReductionIsoTrackEff_= ExpectationReductionIsoTrackNJetsEff_->GetEff(NJets);
  
  //      expectationReductionIsoTrackEff_= getEff(ExpectationReductionIsoTrackNJetsEff_,NJets);
  expectationReductionIsoTrackEff_= getEff(ExpectationReductionIsoTrackBTagsNJetsEff_,BTags,NJets);
  expectationReductionMuIsoTrackEff_ = getEff(ExpectationReductionMuIsoTrackBTagsNJetsEff_,BTags,NJets);
  expectationReductionElecIsoTrackEff_ = getEff(ExpectationReductionElecIsoTrackBTagsNJetsEff_,BTags,NJets);
  expectationReductionPionIsoTrackEff_ = getEff(ExpectationReductionPionIsoTrackBTagsNJetsEff_,BTags,NJets);
	expectationReductionIsoTrackEffCombined_ = (1-expectationReductionMuIsoTrackEff_) * (1-expectationReductionElecIsoTrackEff_) * (1-expectationReductionPionIsoTrackEff_);
	expectationReductionIsoTrackEffCombined_ = 1-expectationReductionIsoTrackEffCombined_;
  if(selectedIDIsoMuonsNum==1 && selectedIDIsoElectronsNum==0)
  {
    mtw =  MTWCalculator(METPt,METPhi, selectedIDIsoMuonsPt[0], selectedIDIsoMuonsPhi[0]);
    selectedIDIsoMuonsActivity[0]=MuActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],muActivityMethod_);
    muPurityCorrection_ =  getEff(MuPurityMHTNJets_,MHT,NJets);
    //              if(!UseUpdatedTEfficiencies_) muMTWEff_ = getEff(MuMTWPTActivity_,selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]);
    if(!UseUpdatedTEfficiencies_) muMTWEff_ = getEff(MuMTWNJets_,NJets);
    //              else muMTWEff_ = MuMTWPTActivityEff_->GetEff(selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]);
    else muMTWEff_ = MuMTWNJetsEff_->GetEff(NJets);
    if(MuMTWSearchBinUse_)muMTWEff_=MuMTWSearchBinEff_->GetEff(searchBin_+0.01);
    purityCorrectedWeight_ = Weight * muPurityCorrection_;
    mtwCorrectedWeight_ = purityCorrectedWeight_ / muMTWEff_;
    if(!UseUpdatedTEfficiencies_) muDiLepContributionMTWAppliedEff_ = getEff(MuDiLepContributionMTWAppliedNJets_,NJets);
    else muDiLepContributionMTWAppliedEff_ = MuDiLepContributionMTWAppliedNJetsEff_->GetEff(NJets);
    if(MuDiLepContributionMTWAppliedEffSearchBinUse_)muDiLepContributionMTWAppliedEff_=MuDiLepContributionMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    mtwDiLepCorrectedWeight_ = mtwCorrectedWeight_ * muDiLepContributionMTWAppliedEff_;
    if(!UseUpdatedTEfficiencies_) muIsoEff_ = getEff(MuIsoPTActivity_, selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]);
    else muIsoEff_ = MuIsoPTActivityEff_->GetEff( selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]);
    if(MuIsoSearchBinUse_)muIsoEff_=MuIsoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffIso_)muIsoEff_ = getEff(MuIsoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]); 
    //              mtwDiLepCorrectedWeight_=Weight;
    muIsoWeight_ = mtwDiLepCorrectedWeight_* (1 - muIsoEff_)/muIsoEff_; 
    //              muIsoWeight_ = Weight* (1 - muIsoEff_)/muIsoEff_;
    //        muRecoEff_ = getEff(MuRecoActivitiy_,selectedIDIsoMuonsActivity[0]);
    if(!UseUpdatedTEfficiencies_) muRecoEff_ = getEff(MuRecoPTActivity_,selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]);
    else muRecoEff_ = MuRecoPTActivityEff_->GetEff(selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]);
    if(MuRecoSearchBinUse_)muRecoEff_=MuRecoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffReco_)muRecoEff_ = getEff(MuRecoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]); 
    muRecoWeight_ = mtwDiLepCorrectedWeight_* 1 / muIsoEff_ * (1-muRecoEff_)/muRecoEff_;
    //        muAccEff_ = getEff(MuAccHTNJets_,HT,NJets);
    //              muAccEff_ = getEff(MuAccBTagNJets_,BTags,NJets);
    if(!UseUpdatedTEfficiencies_) muAccEff_ = getEff(MuAccMHTNJets_,MHT,NJets);
    else muAccEff_ = MuAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(MuAccSearchBinUse_)muAccEff_=MuAccSearchBinEff_->GetEff(searchBin_+0.01);
    muAccWeight_ = mtwDiLepCorrectedWeight_* 1 / muIsoEff_ * 1 / muRecoEff_ * (1-muAccEff_)/muAccEff_;
    
    muTotalWeight_ = muIsoWeight_ + muRecoWeight_ + muAccWeight_;
    totalMuons_ = mtwDiLepCorrectedWeight_ / ( muIsoEff_ * muRecoEff_ * muAccEff_);
    
    //        elecAccEff_ = getEff(ElecAccHTNJets_,HT,NJets);
    //              elecAccEff_ = getEff(ElecAccBTagNJets_,BTags,NJets);
    if(!UseUpdatedTEfficiencies_) elecAccEff_ = getEff(ElecAccMHTNJets_,MHT,NJets);
    else elecAccEff_ = ElecAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(ElecAccSearchBinUse_)elecAccEff_=ElecAccSearchBinEff_->GetEff(searchBin_+0.01);
    elecAccWeight_ = totalMuons_ * (1 - elecAccEff_);
    //        elecRecoEff_ = getEff(ElecRecoActivity_,ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_));
    if(!UseUpdatedTEfficiencies_) elecRecoEff_ = getEff(ElecRecoPTActivity_,selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_));
    else elecRecoEff_ = ElecRecoPTActivityEff_->GetEff(selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_));
    if(ElecRecoSearchBinUse_)elecRecoEff_=ElecRecoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffReco_)elecRecoEff_ = getEff(ElecRecoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_)); 
    elecRecoWeight_ = totalMuons_ * (elecAccEff_) * (1-elecRecoEff_);
    if(!UseUpdatedTEfficiencies_) elecIsoEff_ = getEff(ElecIsoPTActivity_,selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_));
    else elecIsoEff_= ElecIsoPTActivityEff_->GetEff(selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_));
    if(ElecIsoSearchBinUse_)elecIsoEff_=ElecIsoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffIso_)elecIsoEff_ = getEff(ElecIsoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_)); 
    elecIsoWeight_ = totalMuons_ * (elecAccEff_) * (elecRecoEff_) * (1-elecIsoEff_);
    elecTotalWeight_ = elecIsoWeight_ + elecRecoWeight_ + elecAccWeight_;
    totalWeight_ = elecTotalWeight_ + muTotalWeight_;
    if(!UseUpdatedTEfficiencies_) muDiLepEffMTWAppliedEff_ = getEff(MuDiLepEffMTWAppliedNJets_,NJets);
    else muDiLepEffMTWAppliedEff_= MuDiLepContributionMTWAppliedNJetsEff_->GetEff(NJets);
    if(MuDiLepContributionMTWAppliedEffSearchBinUse_)muDiLepEffMTWAppliedEff_=MuDiLepEffMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    totalWeightDiLep_ = totalWeight_ + (1-muDiLepContributionMTWAppliedEff_) * mtwCorrectedWeight_ * (1-muDiLepEffMTWAppliedEff_)/muDiLepEffMTWAppliedEff_;
    totalWeightDiLepIsoTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionIsoTrackEff_);
		IsoTrackReduction_ = totalWeightDiLep_ *expectationReductionIsoTrackEff_;
    totalWeightDiLepIsoMuTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionMuIsoTrackEff_);
		IsoMuTrackReduction_ = totalWeightDiLep_ *expectationReductionMuIsoTrackEff_;
    totalWeightDiLepIsoElecTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionElecIsoTrackEff_);
		IsoElecTrackReduction_ = totalWeightDiLep_ *expectationReductionElecIsoTrackEff_;
    totalWeightDiLepIsoPionTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionPionIsoTrackEff_);
		IsoPionTrackReduction_ = totalWeightDiLep_ *expectationReductionPionIsoTrackEff_;
		totalWeightDiLepIsoTrackReducedCombined_ = totalWeightDiLep_ * (1 - expectationReductionIsoTrackEffCombined_);
		IsoTrackReductionCombined_ = totalWeightDiLep_ *expectationReductionIsoTrackEffCombined_;
    
  }       
  else if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==1)
  {
    mtw =  MTWCalculator(METPt,METPhi, selectedIDIsoElectronsPt[0], selectedIDIsoElectronsPhi[0]);
    selectedIDIsoElectronsActivity[0]=ElecActivity(selectedIDIsoElectronsEta[0], selectedIDIsoElectronsPhi[0],elecActivityMethod_);
    if(!UseUpdatedTEfficiencies_) elecPurityCorrection_ =  getEff(ElecPurityMHTNJets_,MHT,NJets);
    else elecPurityCorrection_ = ElecRecoPTActivityEff_->GetEff(MHT,NJets);
    if(ElecPuritySearchBinEff_) elecPurityCorrection_ = ElecPuritySearchBinEff_->GetEff(searchBin_+0.01);
    //              if(!UseUpdatedTEfficiencies_) elecMTWEff_ = getEff(ElecMTWPTActivity_,selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    if(!UseUpdatedTEfficiencies_) elecMTWEff_ = getEff(ElecMTWNJets_,NJets);
    //              else elecMTWEff_= ElecMTWPTActivityEff_->GetEff(selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    else elecMTWEff_ = ElecMTWNJetsEff_->GetEff(NJets);
    if(ElecMTWSearchBinUse_)elecMTWEff_=ElecMTWSearchBinEff_->GetEff(searchBin_+0.01);
    if(!UseUpdatedTEfficiencies_) elecIsoEff_ =  getEff(ElecIsoPTActivity_,selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    else elecIsoEff_ = ElecIsoPTActivityEff_->GetEff(selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    if(ElecIsoSearchBinUse_)elecIsoEff_=ElecIsoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffIso_)elecIsoEff_ = getEff(ElecIsoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_)); 
    //              elecRecoEff_ = getEff(ElecRecoActivity_,selectedIDIsoElectronsActivity[0]);
    if(!UseUpdatedTEfficiencies_) elecRecoEff_ = getEff(ElecRecoPTActivity_,selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    else elecRecoEff_ = ElecRecoPTActivityEff_->GetEff(selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    if(ElecRecoSearchBinUse_)elecRecoEff_=ElecRecoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffReco_)elecRecoEff_ = getEff(ElecRecoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],ElecActivity(selectedIDIsoMuonsEta[0], selectedIDIsoMuonsPhi[0],elecActivityMethod_)); 
    //              elecAccEff_ = getEff(ElecAccHTNJets_,HT,NJets);
    //              elecAccEff_ = getEff(ElecAccBTagNJets_,BTags,NJets);
    if(!UseUpdatedTEfficiencies_) elecAccEff_ = getEff(ElecAccMHTNJets_,MHT,NJets);
    else elecAccEff_ = ElecAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(ElecAccSearchBinUse_)elecAccEff_=ElecAccSearchBinEff_->GetEff(searchBin_+0.01);
    
    //              muAccEff_ = getEff(MuAccHTNJets_,HT,NJets);
    //              muAccEff_ = getEff(MuAccBTagNJets_,BTags,NJets);
    if(!UseUpdatedTEfficiencies_) muAccEff_ = getEff(MuAccMHTNJets_,MHT,NJets);
    else muAccEff_ = MuAccMHTNJetsEff_->GetEff(MHT,NJets);
    if(MuAccSearchBinUse_)muAccEff_=MuAccSearchBinEff_->GetEff(searchBin_+0.01);
    //              muRecoEff_ = getEff(MuRecoActivitiy_,selectedIDIsoElectronsActivity[0]);
    if(!UseUpdatedTEfficiencies_) muRecoEff_ = getEff(MuRecoPTActivity_, selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    else muRecoEff_ = MuRecoPTActivityEff_->GetEff(selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    if(MuRecoSearchBinUse_)muRecoEff_=MuRecoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffReco_)muRecoEff_ = getEff(MuRecoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]); 
    if(!UseUpdatedTEfficiencies_) muIsoEff_ = getEff(MuIsoPTActivity_, selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    else muIsoEff_ = MuIsoPTActivityEff_->GetEff(selectedIDIsoElectronsPt[0],selectedIDIsoElectronsActivity[0]);
    if(MuIsoSearchBinUse_)muIsoEff_=MuIsoSearchBinEff_->GetEff(searchBin_+0.01);
    if(UseTagAndProbeEffIso_)muIsoEff_ = getEff(MuIsoPTActivityTAPMC_, selectedIDIsoMuonsPt[0],selectedIDIsoMuonsActivity[0]); 
    purityCorrectedWeight_ = Weight * elecPurityCorrection_;
    mtwCorrectedWeight_ =  purityCorrectedWeight_ / elecMTWEff_;
    if(!UseUpdatedTEfficiencies_) elecDiLepContributionMTWAppliedEff_ = getEff(ElecDiLepContributionMTWAppliedNJets_,NJets);
    else elecDiLepContributionMTWAppliedEff_ = ElecDiLepContributionMTWAppliedNJetsEff_->GetEff(NJets);
    if(ElecDiLepContributionMTWAppliedEffSearchBinUse_)elecDiLepContributionMTWAppliedEff_=ElecDiLepContributionMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    mtwDiLepCorrectedWeight_ = mtwCorrectedWeight_ * elecDiLepContributionMTWAppliedEff_;
    elecIsoWeight_= mtwDiLepCorrectedWeight_ * (1 - elecIsoEff_)/elecIsoEff_;
    //              elecIsoWeight_= Weight * (1 - elecIsoEff_)/elecIsoEff_;
    elecRecoWeight_ = mtwDiLepCorrectedWeight_* 1 / elecIsoEff_ * (1-elecRecoEff_)/elecRecoEff_;
    elecAccWeight_ = mtwDiLepCorrectedWeight_* 1 / elecIsoEff_ * 1 / elecRecoEff_ * (1-elecAccEff_)/elecAccEff_;
    elecTotalWeight_ = elecIsoWeight_ + elecRecoWeight_ + elecAccWeight_;
    totalElectrons_ = mtwDiLepCorrectedWeight_ / ( elecIsoEff_ * elecRecoEff_ * elecAccEff_);
    muAccWeight_ = totalElectrons_ * (1-muAccEff_);
    muRecoWeight_ = totalElectrons_ * (muAccEff_) * (1-muRecoEff_);
    muIsoWeight_ = totalElectrons_ * (muAccEff_) * (muRecoEff_) * (1-muIsoEff_);
    muTotalWeight_ = muIsoWeight_ + muRecoWeight_ + muAccWeight_;
    totalWeight_ = elecTotalWeight_ + muTotalWeight_;
    if(!UseUpdatedTEfficiencies_) elecDiLepEffMTWAppliedEff_ = getEff(ElecDiLepEffMTWAppliedNJets_,NJets);
    else elecDiLepEffMTWAppliedEff_ = ElecDiLepEffMTWAppliedNJetsEff_->GetEff(NJets);
    if(ElecDiLepContributionMTWAppliedEffSearchBinUse_)elecDiLepEffMTWAppliedEff_=ElecDiLepContributionMTWAppliedSearchBinEff_->GetEff(searchBin_+0.01);
    totalWeightDiLep_ = totalWeight_ + (1-elecDiLepContributionMTWAppliedEff_) * mtwCorrectedWeight_ * (1-elecDiLepEffMTWAppliedEff_)/elecDiLepEffMTWAppliedEff_;
    totalWeightDiLepIsoTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionIsoTrackEff_);
		IsoTrackReduction_ = totalWeightDiLep_ *expectationReductionIsoTrackEff_;
		totalWeightDiLepIsoMuTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionMuIsoTrackEff_);
		IsoMuTrackReduction_ = totalWeightDiLep_ *expectationReductionMuIsoTrackEff_;
		totalWeightDiLepIsoElecTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionElecIsoTrackEff_);
		IsoElecTrackReduction_ = totalWeightDiLep_ *expectationReductionElecIsoTrackEff_;
		totalWeightDiLepIsoPionTrackReduced_ = totalWeightDiLep_ * (1 - expectationReductionPionIsoTrackEff_);
		IsoPionTrackReduction_ = totalWeightDiLep_ *expectationReductionPionIsoTrackEff_;
		totalWeightDiLepIsoTrackReducedCombined_ = totalWeightDiLep_ * (1 - expectationReductionIsoTrackEffCombined_);
		IsoTrackReductionCombined_ = totalWeightDiLep_ *expectationReductionIsoTrackEffCombined_;
  }
  
  //      if(selectedIDIsoMuonsNum==0 && selectedIDIsoElectronsNum==0 && !IsolatedTracksMuMatched_ && !IsolatedTracksElecMatched_) return kTRUE;
  tPrediction_->Fill();
  return kTRUE;
}

void Prediction::SlaveTerminate()
{
  // The SlaveTerminate() function is called after all entries or objects
  // have been processed. When running with PROOF SlaveTerminate() is called
  // on each slave server.
  SearchBins_->PrintUsed();
  
}

void Prediction::Terminate()
{
  // The Terminate() function is the last function to be called during
  // a query. It always runs on the client, it can be used to present
  // the results graphically or save the results to file.
  TFile *outPutFile = new TFile("Prediction.root","RECREATE"); ;
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
  totalMuons_=0;
  elecIsoEff_=0;
  elecIsoWeight_=0;
  elecRecoEff_=0;
  elecRecoWeight_=0;
  elecAccEff_=0;
  elecAccWeight_=0;
  elecTotalWeight_=0;
  totalElectrons_=0.;
  totalWeight_=0.;
  muDiLepEffMTWAppliedEff_=0.;
  elecDiLepEffMTWAppliedEff_=0.;
  totalWeightDiLep_=0.;
  totalWeightDiLepIsoTrackReduced_=0.;
  // isolated track prediction
  IsolatedTracksMuMatched_=false;
  IsolatedTracksElecMatched_=false;
	IsoTrackReduction_=0.;
	IsoMuTrackReduction_=0.;
	IsoElecTrackReduction_=0.;
	IsoPionTrackReduction_=0.;
	IsoTrackReductionCombined_=0.;
  
}
bool Prediction::FiltersPass()
{
  bool result=true;
  // if(Filter_HBHENoiseFilterRA2==0) result=false;
  if(!JetID) result=false;
  return result;
}
double Prediction::deltaR(double eta1, double phi1, double eta2, double phi2)
{
  double deta = eta1-eta2;
  double dphi = TVector2::Phi_mpi_pi(phi1-phi2);
  return sqrt(deta * deta + dphi *dphi); 
}

double Prediction::MTWCalculator(double metPt,double  metPhi,double  lepPt,double  lepPhi)
{
  double deltaPhi =TVector2::Phi_mpi_pi(lepPhi-metPhi);
  return sqrt(2*lepPt*metPt*(1-cos(deltaPhi)) );
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

double Prediction::getEff(TH2F* effTH2F, double xValue, double yValue)
{
  double result = 0;
  if(xValue < effTH2F->GetXaxis()->GetXmin() )
  {
    //              std::cout<<"Warning xValue: "<<xValue<<" is smaller than minimum of histo: "<<effTH2F->GetName()<<std::endl;
    xValue= effTH2F->GetXaxis()->GetXmin()+0.01;
  }
  else if(xValue > effTH2F->GetXaxis()->GetXmax() )
  {
    //        std::cout<<"Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<effTH2F->GetName()<<" which is: "<<effTH2F->GetXaxis()->GetXmax();
    xValue= effTH2F->GetXaxis()->GetXmax()-0.01;
    //              std::cout<<" Setting xValue to: "<<xValue<<std::endl;
    if(xValue > effTH2F->GetXaxis()->GetXmax() )std::cout<<"Problem persists!!!!!!!!!"<<std::endl;
  }
  
  if(yValue < effTH2F->GetYaxis()->GetXmin() )
  {
    //              std::cout<<"Warning yValue: "<<yValue<<" is smaller than minimum of histo: "<<effTH2F->GetName()<<std::endl;
    yValue= effTH2F->GetYaxis()->GetXmin()+0.01;
  }
  else if(yValue > effTH2F->GetYaxis()->GetXmax() )
  {
    //              std::cout<<"Warning yValue: "<<yValue<<" is bigger than maximum of histo: "<<effTH2F->GetName()<<std::endl;
    yValue= effTH2F->GetYaxis()->GetXmax()-0.01;
  }
  result = effTH2F->GetBinContent(effTH2F->GetXaxis()->FindBin(xValue),effTH2F->GetYaxis()->FindBin(yValue));
  if(result<0.01)
  {
    std::cout<<"Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<effTH2F->GetName()<<std::endl;
    result =0.01;
  }
  if(result>1)
  {
    std::cout<<"Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<effTH2F->GetName()<<std::endl;
    result =0.99;
  }
  return result;
}
double Prediction::getEff(TH1F* effTH1F, double xValue)
{
  double result = 0;
  if(xValue < effTH1F->GetXaxis()->GetXmin() )
  {
    std::cout<<"Warning xValue: "<<xValue<<" is smaller than minimum of histo: "<<effTH1F->GetName()<<std::endl;
    xValue= effTH1F->GetXaxis()->GetXmin()+0.01;
  }
  else if(xValue > effTH1F->GetXaxis()->GetXmax() )
  {
    std::cout<<"Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<effTH1F->GetName()<<" which is: "<<effTH1F->GetXaxis()->GetXmax();
    xValue= effTH1F->GetXaxis()->GetXmax()-0.01;
    std::cout<<" Setting xValue to: "<<xValue<<std::endl;
    if(xValue > effTH1F->GetXaxis()->GetXmax() )std::cout<<"Problem persists!!!!!!!!!"<<std::endl;
  }
  
  
  result = effTH1F->GetBinContent(effTH1F->GetXaxis()->FindBin(xValue));
  if(result<0.01)
  {
    std::cout<<"Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<effTH1F->GetName()<<std::endl;
    result =0.01;
  }
  if(result>1)
  {
    std::cout<<"Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<effTH1F->GetName()<<std::endl;
    result =0.99;
  }
  return result;
}

double Prediction::MuActivity( double muEta, double muPhi, unsigned int method)
{
  double result =0;
  if(method==0)
  {
    for (unsigned int i=0; i < JetsNum ; i++)
    {
      if(deltaR(muEta,muPhi,JetsEta[i],JetsPhi[i])>maxDeltaRMuActivity_ ) continue;
      result+=JetsPt[i] * (Jets_chargedEmEnergyFraction[i] + Jets_chargedHadronEnergyFraction[i]);
    }
  }
  if(method==1)
  {
    for (unsigned int i=0; i < JetsNum ; i++)
    {
      if(deltaR(muEta,muPhi,JetsEta[i],JetsPhi[i])>maxDeltaRMuActivity_ ) continue;
      result+=JetsPt[i] * (Jets_chargedEmEnergyFraction[i] + Jets_chargedHadronEnergyFraction[i])*(1/(deltaR(muEta,muPhi,JetsEta[i],JetsPhi[i])+0.5));
    }
  }
  if(method==2)
  {
    for(unsigned int i=0; i< SelectedPFCandidatesNum; i++)
    {
      if(deltaR(muEta,muPhi,SelectedPFCandidatesEta[i],SelectedPFCandidatesPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=SelectedPFCandidatesPt[i];
    }
  }
  if(method==3)
  {
    for(unsigned int i=0; i< SelectedPFCandidatesNum; i++)
    {
      if(SelectedPFCandidates_Charge[i]!=0) continue;
      if(deltaR(muEta,muPhi,SelectedPFCandidatesEta[i],SelectedPFCandidatesPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=SelectedPFCandidatesPt[i];
    }
  }
  return result;
  
}
double Prediction::ElecActivity( double elecEta, double elecPhi, unsigned int method)
{
  double result =0;
  if(method==0)
  {
    for (unsigned int i=0; i < JetsNum ; i++)
    {
      if(deltaR(elecEta,elecPhi,JetsEta[i],JetsPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=JetsPt[i] * (Jets_chargedHadronEnergyFraction[i]);
      //                      result+=JetsPt[i] * (Jets_chargedEmEnergyFraction[i] + Jets_chargedHadronEnergyFraction[i]);
    }
  }
  if(method==1)
  {
    for (unsigned int i=0; i < JetsNum ; i++)
    {
      if(deltaR(elecEta,elecPhi,JetsEta[i],JetsPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=JetsPt[i] * (Jets_chargedHadronEnergyFraction[i])*(1/(deltaR(elecEta,elecPhi,JetsEta[i],JetsPhi[i])+0.5));
    }
  }
  if(method==2)
  {
    for(unsigned int i=0; i< SelectedPFCandidatesNum; i++)
    {
      if(deltaR(elecEta,elecPhi,SelectedPFCandidatesEta[i],SelectedPFCandidatesPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=SelectedPFCandidatesPt[i];
    }
  }
  if(method==3)
  {
    for(unsigned int i=0; i< SelectedPFCandidatesNum; i++)
    {
      if(SelectedPFCandidates_Charge[i]!=0) continue;
      if(deltaR(elecEta,elecPhi,SelectedPFCandidatesEta[i],SelectedPFCandidatesPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=SelectedPFCandidatesPt[i];
    }
  }
  return result;
  
}
double Prediction::IsoTrackActivityCalc( double isoTrackEta, double isoTrackPhi, unsigned int method)
{
  double result =0;
  if(method==0)
  {
    for (unsigned int i=0; i < JetsNum ; i++)
    {
      if(deltaR(isoTrackEta,isoTrackPhi,JetsEta[i],JetsPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=JetsPt[i] * (Jets_neutralEmEnergyFraction[i] + Jets_photonEnergyFraction[i]);
    }
  }
  if(method==1)
  {
    for (unsigned int i=0; i < JetsNum ; i++)
    {
      if(deltaR(isoTrackEta,isoTrackPhi,JetsEta[i],JetsPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=JetsPt[i] * (Jets_neutralEmEnergyFraction[i] + Jets_photonEnergyFraction[i])*(1/(deltaR(isoTrackEta,isoTrackPhi,JetsEta[i],JetsPhi[i])+0.5));
    }
  }
  if(method==2)
  {
    for(unsigned int i=0; i< SelectedPFCandidatesNum; i++)
    {
      if(deltaR(isoTrackEta,isoTrackPhi,SelectedPFCandidatesEta[i],SelectedPFCandidatesPhi[i])>maxDeltaRElecActivity_ ) continue;
      result+=SelectedPFCandidatesPt[i];
    }
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
      //                      std::cout<<"object: "<<object->GetName()<<" is a th2f\n";
      refTH2F_ = (TH2F*)object;
    }
    if(object->IsA()->InheritsFrom( TGraphAsymmErrors::Class() ) )
    {
      //                      std::cout<<"object: "<<object->GetName()<<" is a TGraphAsymmErrors\n";
      TGraphAsymmErrorsVec_.push_back( (TGraphAsymmErrors*)object  );
    }
    count++;
  }
  //      for(unsigned int i=0; i<TGraphAsymmErrorsVec_.size();i++)
  //      {
  //              std::cout<<"TGraph["<<i<<"]: "<<TGraphAsymmErrorsVec_[i]->GetTitle()<<std::endl;
  //      }
  xMax_=0; xMin_=0; yMax_=0; yMin_=0;
  xMax_= refTH2F_->GetXaxis()->GetXmax();
  xMin_=refTH2F_->GetXaxis()->GetXmin();
  yMax_= refTH2F_->GetYaxis()->GetXmax();
  yMin_=refTH2F_->GetYaxis()->GetXmin();
  //      std::cout<<"Eff: "<<refTH2F_->GetName()<<" range: ["<<xMin_<<","<<xMax_<<"],["<<yMin_<<","<<yMax_<<"]\n"; 
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
    std::cout<<"THFeff::Warning xValue: "<<xValue<<" is bigger than maximum of histo: "<<refTH2F_->GetName()<<" which is: "<<xMax_;
    xValue= xMax_-0.001;
    std::cout<<" Setting xValue to: "<<xValue<<std::endl;
  }
  if(xValue<xMin_)
  {
    std::cout<<"THFeff::Warning xValue: "<<xValue<<" is smaller than min of histo: "<<refTH2F_->GetName()<<" which is: "<<xMin_;
    xValue= xMin_+0.001;
    std::cout<<" Setting xValue to: "<<xValue<<std::endl;
  }
  if(yValue>yMax_)
  {
    std::cout<<"THFeff::Warning yValue: "<<yValue<<" is bigger than maximum of histo: "<<refTH2F_->GetName()<<" which is: "<<yMax_;
    yValue= yMax_-0.001;
    std::cout<<" Setting xValue to: "<<yValue<<std::endl;
  }
  if(yValue<yMin_)
  {
    std::cout<<"THFeff::Warning yValue: "<<yValue<<" is smaller than min of histo: "<<refTH2F_->GetName()<<" which is: "<<yMin_;
    yValue= yMin_+0.001;
    std::cout<<" Setting yValue to: "<<yValue<<std::endl;
  }
  unsigned int yHistoIndex =refTH2F_->GetYaxis()->FindBin(yValue);
  if(yHistoIndex<1 || yHistoIndex>(TGraphAsymmErrorsVec_.size()+1))
  {
    std::cout<<"THFeff::Error selected HistoIndex for refTH2F: "<<refTH2F_->GetName()<<" is out of bounce: "<<yHistoIndex<<std::endl;
  }
  result = TGraphAsymmErrorsVec_[yHistoIndex-1]->Eval(xValue);
  if(result<0.01)
  {
    std::cout<<"THFeff::Warning efficiency is: "<<result<<" is smaller than 1% for histo: "<<refTH2F_->GetName()<<std::endl;
    result =0.01;
  }
  if(result>1)
  {
    std::cout<<"THFeff::Warning efficiency is: "<<result<<" is bigger than 1 for histo: "<<refTH2F_->GetName()<<std::endl;
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
  
	binning=1;
	// HTmin,HTmax,MHTmin,MHTmax,NJetsmin,NJetsmax,BTagsmin,BTagsmax
	// NJets 4,6 BTags=0
	// fixed ht Njets btags all MHT bins
	if(binning==0)
	{
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
	else if(binning==1)
	{
		bins_.push_back( Bin(500,800,200,500,4,6,-1,0) );
		bins_.push_back( Bin(800,1200,200,500,4,6,-1,0) );
		bins_.push_back( Bin(1200,99999,200,500,4,6,-1,0) );
		
		bins_.push_back( Bin(500,1200,500,750,4,6,-1,0) );
		bins_.push_back( Bin(1200,99999,500,750,4,6,-1,0) );
		
		bins_.push_back( Bin(800,99999,750,9999,4,6,-1,0) );
		
		// NJets 4,6 BTags=1
		// fixed ht Njets btags all MHT bins
		bins_.push_back( Bin(500,800,200,500,4,6,1,1) );
		bins_.push_back( Bin(800,1200,200,500,4,6,1,1) );
		bins_.push_back( Bin(1200,99999,200,500,4,6,1,1) );
		
		bins_.push_back( Bin(500,1200,500,750,4,6,1,1) );
		bins_.push_back( Bin(1200,99999,500,750,4,6,1,1) );
		
		bins_.push_back( Bin(800,99999,750,9999,4,6,1,1) );
		
		// NJets 4,6 BTags=2
		// fixed ht Njets btags all MHT bins
		bins_.push_back( Bin(500,800,200,500,4,6,2,2) );
		bins_.push_back( Bin(800,1200,200,500,4,6,2,2) );
		bins_.push_back( Bin(1200,99999,200,500,4,6,2,2) );
		
		bins_.push_back( Bin(500,1200,500,750,4,6,2,2) );
		bins_.push_back( Bin(1200,99999,500,750,4,6,2,2) );
		
		bins_.push_back( Bin(800,99999,750,9999,4,6,2,2) );
		
		// NJets 4,6 BTags=>3
		// fixed ht Njets btags all MHT bins
		bins_.push_back( Bin(500,800,200,500,4,6,3,9999) );
		bins_.push_back( Bin(800,1200,200,500,4,6,3,9999) );
		bins_.push_back( Bin(1200,99999,200,500,4,6,3,9999) );
		
		bins_.push_back( Bin(500,1200,500,750,4,6,3,9999) );
		bins_.push_back( Bin(1200,99999,500,750,4,6,3,9999) );
		
		bins_.push_back( Bin(800,99999,750,9999,4,6,3,9999) );
		
		// NJewts 7,8 BTags=0
		bins_.push_back( Bin(500,800,200,500,7,8,-1,0) );
		bins_.push_back( Bin(800,1200,200,500,7,8,-1,0) );
		bins_.push_back( Bin(1200,99999,200,500,7,8,-1,0) );
		
		bins_.push_back( Bin(500,1200,500,750,7,8,-1,0) );
		bins_.push_back( Bin(1200,99999,500,750,7,8,-1,0) );
		
		bins_.push_back( Bin(800,99999,750,9999,7,8,-1,0) );
		
		// NJewts 7,8 BTags=1
		bins_.push_back( Bin(500,800,200,500,7,8,1,1) );
		bins_.push_back( Bin(800,1200,200,500,7,8,1,1) );
		bins_.push_back( Bin(1200,99999,200,500,7,8,1,1) );
		
		bins_.push_back( Bin(500,1200,500,750,7,8,1,1) );
		bins_.push_back( Bin(1200,99999,500,750,7,8,1,1) );
		
		bins_.push_back( Bin(800,99999,750,9999,7,8,1,1) );
		
		// NJewts 7,8 BTags=2
		bins_.push_back( Bin(500,800,200,500,7,8,2,2) );
		bins_.push_back( Bin(800,1200,200,500,7,8,2,2) );
		bins_.push_back( Bin(1200,99999,200,500,7,8,2,2) );
		
		bins_.push_back( Bin(500,1200,500,750,7,8,2,2) );
		bins_.push_back( Bin(1200,99999,500,750,7,8,2,2) );
		
		bins_.push_back( Bin(800,99999,750,9999,7,8,2,2) );
		
		// NJewts 7,8 BTags=>3
		bins_.push_back( Bin(500,800,200,500,7,8,3,9999) );
		bins_.push_back( Bin(800,1200,200,500,7,8,3,9999) );
		bins_.push_back( Bin(1200,99999,200,500,7,8,3,9999) );
		
		bins_.push_back( Bin(500,1200,500,750,7,8,3,9999) );
		bins_.push_back( Bin(1200,99999,500,750,7,8,3,9999) );
		
		bins_.push_back( Bin(800,99999,750,9999,7,8,3,9999) );
		
		
		// NJewts 9,9999 BTags=0
		bins_.push_back( Bin(500,800,200,500,9,9999,-1,0) );
		bins_.push_back( Bin(800,1200,200,500,9,9999,-1,0) );
		bins_.push_back( Bin(1200,99999,200,500,9,9999,-1,0) );
		
		bins_.push_back( Bin(500,1200,500,750,9,9999,-1,0) );
		bins_.push_back( Bin(1200,99999,500,750,9,9999,-1,0) );
		
		bins_.push_back( Bin(800,99999,750,9999,9,9999,-1,0) );
		
		
		// NJewts 9,9999 BTags=1
		bins_.push_back( Bin(500,800,200,500,9,9999,1,1) );
		bins_.push_back( Bin(800,1200,200,500,9,9999,1,1) );
		bins_.push_back( Bin(1200,99999,200,500,9,9999,1,1) );
		
		bins_.push_back( Bin(500,1200,500,750,9,9999,1,1) );
		bins_.push_back( Bin(1200,99999,500,750,9,9999,1,1) );
		
		bins_.push_back( Bin(800,99999,750,9999,9,9999,1,1) );
		
		
		// NJewts 9,9999 BTags=2
		bins_.push_back( Bin(500,800,200,500,9,9999,2,2) );
		bins_.push_back( Bin(800,1200,200,500,9,9999,2,2) );
		bins_.push_back( Bin(1200,99999,200,500,9,9999,2,2) );
		
		bins_.push_back( Bin(500,1200,500,750,9,9999,2,2) );
		bins_.push_back( Bin(1200,99999,500,750,9,9999,2,2) );
		
		bins_.push_back( Bin(800,99999,750,9999,9,9999,2,2) );
		
		
		// NJewts 9,9999 BTags=>3
		bins_.push_back( Bin(500,800,200,500,9,9999,3,9999) );
		bins_.push_back( Bin(800,1200,200,500,9,9999,3,9999) );
		bins_.push_back( Bin(1200,99999,200,500,9,9999,3,9999) );
		
		bins_.push_back( Bin(500,1200,500,750,9,9999,3,9999) );
		bins_.push_back( Bin(1200,99999,500,750,9,9999,3,9999) );
		
		bins_.push_back( Bin(800,99999,750,9999,9,9999,3,9999) );
		
	}
  for(unsigned int i=0; i<bins_.size();i++)
  {
    usedBin_.push_back(0); 
  }
}

unsigned int SearchBins::GetBinNumber(double HT, double MHT, int NJets, int BTags)
{
  unsigned int result =999;
  int match =-1;
  for(unsigned int i=0; i<bins_.size();i++)
  {
    //              std::cout<<"Bin["<<i<<"]: HT["<<bins_[i].HTmin_<<","<<bins_[i].HTmax_<<"] MHT["<<bins_[i].MHTmin_<<","<<bins_[i].MHTmax_<<"] NJets["<<bins_[i].NJetsmin_<<","<<bins_[i].NJetsmax_<<"] BTags["<<bins_[i].BTagsmin_<<","<<bins_[i].BTagsmax_<<"]\n";
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
