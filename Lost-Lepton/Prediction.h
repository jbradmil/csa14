//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Fri Oct 24 09:51:04 2014 by ROOT version 5.34/04
// from TTree RA2PreSelection/RA2PreSelection
// found on file: /nfs/dust/cms/user/adraeger/CSA2014/mc/wjetHT600-Inf.root
//////////////////////////////////////////////////////////

#ifndef Prediction_h
#define Prediction_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2F.h>
#include <TH1F.h>
#include "TVector2.h"
#include <cmath>
#include "TEfficiency.h"
#include "TGraphAsymmErrors.h"
#include "TKey.h"
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.
// use gen infomation to fix purityy of muon controlsample
const bool useGenInfoToMatchCSMuonToGen_=0; // changed 20 Nov from 1 to 0
const double maxDeltaRGenToRecoIsoMuon_=0.3;
const double maxDiffPtGenToRecoIsoMuon_=0.3;
const double minHT_=500;
const double minMHT_=200;
const double minNJets_=3.5;
const double deltaPhi1_=0.5;
const double deltaPhi2_=0.5;
const double deltaPhi3_=0.3;
const double minDeltaPhiN_=4.0;
const bool applyFilters_=true;
const bool applyDiLepCorrection_=true;

const UShort_t NJetsLow_=2;
const UShort_t NJetsMedium_=5;
const UShort_t NJetsMedium2_=7;
const UShort_t NJetsHigh_=8;

// uncertainties
const double MuAccUncertaintyDown_ = 9;  // pdf

// actity around lepton
const double maxDeltaRMuActivity_=1.0;
const double maxDeltaRElecActivity_=1.0;
const double maxDeltaRIsoTrackActivity_=1.0;
const unsigned int elecActivityMethod_=0;               // ###############
const unsigned int muActivityMethod_=0;                 // ###############
const unsigned int isoTrackActivityMethod_=0;

// isolated track prediction
const double maxDeltaRRecoIsoMuToTack_ = 0.2;
const double maxDiffPtRecoIsoMuToTack_ = 0.5;

const double maxDeltaRRecoIsoElecToTack_ = 0.3;
const double maxDiffPtRecoIsoElecToTack_ = 0.5;
//using either oldschool th1/2 which have wrong error calculations or the updated tefficneices
const bool UseUpdatedTEfficiencies_=false;
const bool UseTagAndProbeEffIso_=false; // warning overriges all other choices for isolation efficiency
const bool UseTagAndProbeEffReco_=false; // warning overriges all other choices for reconstruction efficiency

const bool MuMTWSearchBinUse_=false; // warning overwrites UseUpdatedTEfficiencies_
const bool MuDiLepContributionMTWAppliedEffSearchBinUse_=false;
const bool MuIsoSearchBinUse_=false; 
const bool MuRecoSearchBinUse_=false; 
const bool MuAccSearchBinUse_=false;  

const bool ElecMTWSearchBinUse_=false;
const bool ElecIsoSearchBinUse_=false; 
const bool ElecRecoSearchBinUse_=false; 
const bool ElecAccSearchBinUse_=false; 
const bool ElecPuritySearchBinEff_=false;
const bool ElecDiLepContributionMTWAppliedEffSearchBinUse_=false;

class Bin
{
public:
  Bin(){}
  Bin(double HTmin, double HTmax, double MHTmin, double MHTmax, int NJetsmin, int NJetsmax, int BTagsmin, int BTagsmax)
  {
    HTmin_=HTmin;
    HTmax_=HTmax;
    MHTmin_=MHTmin;
    MHTmax_=MHTmax;
    NJetsmin_=NJetsmin;
    NJetsmax_=NJetsmax;
    BTagsmin_=BTagsmin;
    BTagsmax_=BTagsmax;
  }
  double HTmin_, HTmax_, MHTmin_, MHTmax_;
  int NJetsmin_, NJetsmax_, BTagsmin_, BTagsmax_;
  ~Bin(){}
private:
};
class SearchBins
{
public:
  SearchBins();
  unsigned int GetBinNumber(double HT, double MHT, int NJets, int BTags);
  void PrintUsed();
  
  ~SearchBins(){}
protected:
  std::vector<Bin> bins_;
  std::vector<int> usedBin_;
	unsigned int binning;
};

class THFeff
{
public:
  THFeff(){}
  THFeff(TDirectory* inputFolder, const char* FolderName);
  THFeff(TGraphAsymmErrors* inputGraph);
  double GetEff(double xValue, double yValue);
  double GetEff(double xValue);
  ~THFeff(){}
private:
  bool useTH2f_;
  const char* FolderName_;
  TH2F* refTH2F_;
  TGraphAsymmErrors* TGraphAsymmErrorsRef_;
  std::vector<TGraphAsymmErrors*> TGraphAsymmErrorsVec_;
  double xMax_, xMin_, yMax_, yMin_;
};




class Prediction : public TSelector {
public :
  TTree          *fChain;   //!pointer to the analyzed TTree or TChain
  void resetValues();
  bool FiltersPass();
  double deltaR(double eta1, double phi1, double eta2, double phi2);
  double MTWCalculator(double metPt,double  metPhi,double  lepPt,double  lepPhi);
  double effUp(double eff, double up);
  double effDown(double eff, double down);
  double getEff(TH2F* effTH2F, double xValue, double yValue);
  double getEff(TH1F* effTH2F, double xValue);
  double MuActivity(double muEta, double muPhi, unsigned int method);
  double ElecActivity( double elecEta, double elecPhi, unsigned int method);
  double IsoTrackActivityCalc( double isoTrackEta, double isoTrackPhi, unsigned int method);
  // output variables
  TTree *tPrediction_;
  SearchBins *SearchBins_;
  Float_t mtw;
  Float_t muPurityCorrection_;
  Float_t muMTWEff_, elecMTWEff_, mtwCorrectedWeight_;
  Float_t muDiLepContributionMTWAppliedEff_, mtwDiLepCorrectedWeight_;
  Float_t muIsoEff_, muIsoWeight_;
  Float_t muRecoEff_, muRecoWeight_;
  Float_t muAccEff_, muAccWeight_;
  Float_t muTotalWeight_, totalMuons_;
  Float_t elecPurityCorrection_, purityCorrectedWeight_, elecDiLepContributionMTWAppliedEff_;
  Float_t elecIsoEff_, elecIsoWeight_;
  Float_t elecRecoEff_, elecRecoWeight_;
  Float_t elecAccEff_, elecAccWeight_;
  Float_t elecTotalWeight_, totalElectrons_;
	Float_t IsoTrackReduction_, IsoMuTrackReduction_, IsoElecTrackReduction_, IsoPionTrackReduction_, IsoTrackReductionCombined_;
  Float_t muDiLepEffMTWAppliedEff_, elecDiLepEffMTWAppliedEff_;
  Float_t expectationReductionIsoTrackEff_;
  Float_t expectationReductionMuIsoTrackEff_,expectationReductionElecIsoTrackEff_,expectationReductionPionIsoTrackEff_, expectationReductionIsoTrackEffCombined_;
  
  Float_t totalWeight_, totalWeightDiLep_, totalWeightDiLepIsoTrackReduced_,totalWeightDiLepIsoMuTrackReduced_,totalWeightDiLepIsoElecTrackReduced_,totalWeightDiLepIsoPionTrackReduced_,totalWeightDiLepIsoTrackReducedCombined_;
  Float_t         selectedIDIsoMuonsActivity[5];   //[selectedIDIsoMuonsNum]
  Float_t         selectedIDIsoElectronsActivity[5];   //[selectedIDIsoMuonsNum]
  UShort_t searchBin_, Bin_;
  
  // search bin definition
  SearchBins* searchBinsRef_;
  
  // isolated track prediction
  bool IsolatedTracksMuMatched_, IsolatedTracksElecMatched_;
  
  // Effiecineices
  //    TH2F *MuMTWPTActivity_;
  TH1F *MuMTWNJets_;
  TH1F *MuDiLepContributionMTWAppliedNJets_;
  TH2F *MuIsoPTActivity_, *MuIsoPTActivityTAPMC_;
  //    TH1F *MuRecoActivitiy_;
  TH2F *MuRecoPTActivity_, *MuRecoPTActivityTAPMC_;
  //    TH2F *MuAccHTNJets_;
  //       TH2F *MuAccBTagNJets_;
  TH2F *MuAccMHTNJets_;
  TH1F *MuDiLepEffMTWAppliedNJets_;
  TH2F *MuPurityMHTNJets_;
  
  TH2F *ElecIsoPTActivity_, *ElecIsoPTActivityTAPMC_;
  //    TH1F *ElecRecoActivity_;
  TH2F *ElecRecoPTActivity_, *ElecRecoPTActivityTAPMC_;
  //    TH2F *ElecAccHTNJets_;
  //       TH2F *ElecAccBTagNJets_;
  TH2F *ElecAccMHTNJets_;
  TH2F *ElecPurityMHTNJets_;
  //       TH2F *ElecMTWPTActivity_;
  TH1F *ElecMTWNJets_;
  TH1F *ElecDiLepContributionMTWAppliedNJets_;
  TH1F *ElecDiLepEffMTWAppliedNJets_;
  
  // expectation reduction by the isolated track veto
  TH1F *ExpectationReductionIsoTrackNJetsEff_;
  TH2F *ExpectationReductionIsoTrackBTagsNJetsEff_;
  TH2F *ExpectationReductionMuIsoTrackBTagsNJetsEff_, *ExpectationReductionElecIsoTrackBTagsNJetsEff_, *ExpectationReductionPionIsoTrackBTagsNJetsEff_;
  
  UShort_t elecActivityMethod, muActivityMethod, isoTrackActivityMethod;
  
  // TEfficiency objects
  //       THFeff *MuMTWPTActivityEff_;
  THFeff *MuMTWNJetsEff_;
  THFeff *MuDiLepContributionMTWAppliedNJetsEff_;
  THFeff *MuIsoPTActivityEff_;
  THFeff *MuRecoPTActivityEff_;
  THFeff *MuAccMHTNJetsEff_;
  THFeff *MuDiLepEffMTWAppliedNJetsEff_;
  
  THFeff *ElecIsoPTActivityEff_;
  THFeff *ElecRecoPTActivityEff_;
  THFeff *ElecAccMHTNJetsEff_;
  THFeff *ElecPurityMHTNJetsEff_;
  //       THFeff *ElecMTWPTActivityEff_;
  THFeff *ElecMTWNJetsEff_;
  THFeff *ElecDiLepContributionMTWAppliedNJetsEff_;
  THFeff *ElecDiLepEffMTWAppliedNJetsEff_;
  
  //       THFeff *ExpectationReductionIsoTrackNJetsEff_;
  //       THFeff *ExpectationReductionIsoTrackMHTEff_;
  
  //Search bin based efficiencies
  THFeff *MuMTWSearchBinEff_;
  THFeff *MuDiLepContributionMTWAppliedSearchBinEff_;
  THFeff *MuIsoSearchBinEff_;
  THFeff *MuRecoSearchBinEff_;
  THFeff *MuAccSearchBinEff_;
  THFeff *MuDiLepEffMTWAppliedSearchBinEff_;
  
  THFeff *ElecIsoSearchBinEff_;
  THFeff *ElecRecoSearchBinEff_;
  THFeff *ElecAccSearchBinEff_;
  THFeff *ElecPuritySearchBinEff_;
  THFeff *ElecMTWSearchBinEff_;
  THFeff *ElecDiLepContributionMTWAppliedSearchBinEff_;
  THFeff *ElecDiLepEffMTWAppliedSearchBinEff_;
  
  Int_t          isoTracks;
  
  // Declaration of leaf types
  UInt_t          RunNum;
  UInt_t          LumiBlockNum;
  UInt_t          EvtNum;
  Int_t           NVtx;
  
  Int_t           isoElectronTracks;
  Int_t           isoMuonTracks;
  Int_t           isoPionTracks;
  Int_t           Leptons;
  Int_t           TagLeptonHighPT;
  Int_t           NJets;
  Int_t           BTags;
  Float_t         Weight;
  Float_t         HT;
  Float_t         MHT;
  Float_t         DeltaPhi1;
  Float_t         DeltaPhi2;
  Float_t         DeltaPhi3;
  Float_t         minDeltaPhiN;
  Float_t         DeltaPhiN1;
  Float_t         DeltaPhiN2;
  Float_t         DeltaPhiN3;
  Float_t         METPt;
  Float_t         METPhi;
  UChar_t         JetID;
  UShort_t        MuonsNum;
  Float_t         MuonsPt[3];   //[MuonsNum]
  Float_t         MuonsEta[3];   //[MuonsNum]
  Float_t         MuonsPhi[3];   //[MuonsNum]
  Float_t         MuonsE[3];   //[MuonsNum]
  UShort_t        ElectronsNum;
  Float_t         ElectronsPt[4];   //[ElectronsNum]
  Float_t         ElectronsEta[4];   //[ElectronsNum]
  Float_t         ElectronsPhi[4];   //[ElectronsNum]
  Float_t         ElectronsE[4];   //[ElectronsNum]
  UShort_t        IsolatedElectronTracksVetoNum;
  Float_t         IsolatedElectronTracksVetoPt[2];   //[IsolatedElectronTracksVetoNum]
  Float_t         IsolatedElectronTracksVetoEta[2];   //[IsolatedElectronTracksVetoNum]
  Float_t         IsolatedElectronTracksVetoPhi[2];   //[IsolatedElectronTracksVetoNum]
  Float_t         IsolatedElectronTracksVetoE[2];   //[IsolatedElectronTracksVetoNum]
  Float_t         IsolatedElectronTracksVeto_MT[2];   //[IsolatedElectronTracksVetoNum]
  UShort_t        IsolatedMuonTracksVetoNum;
  Float_t         IsolatedMuonTracksVetoPt[2];   //[IsolatedMuonTracksVetoNum]
  Float_t         IsolatedMuonTracksVetoEta[2];   //[IsolatedMuonTracksVetoNum]
  Float_t         IsolatedMuonTracksVetoPhi[2];   //[IsolatedMuonTracksVetoNum]
  Float_t         IsolatedMuonTracksVetoE[2];   //[IsolatedMuonTracksVetoNum]
  Float_t         IsolatedMuonTracksVeto_MT[2];   //[IsolatedMuonTracksVetoNum]
  UShort_t        IsolatedPionTracksVetoNum;
  Float_t         IsolatedPionTracksVetoPt[2];   //[IsolatedPionTracksVetoNum]
  Float_t         IsolatedPionTracksVetoEta[2];   //[IsolatedPionTracksVetoNum]
  Float_t         IsolatedPionTracksVetoPhi[2];   //[IsolatedPionTracksVetoNum]
  Float_t         IsolatedPionTracksVetoE[2];   //[IsolatedPionTracksVetoNum]
  Float_t         IsolatedPionTracksVeto_MT[2];   //[IsolatedPionTracksVetoNum]
  UShort_t        selectedIDIsoMuonsNum;
  Float_t         selectedIDIsoMuonsPt[3];   //[selectedIDIsoMuonsNum]
  Float_t         selectedIDIsoMuonsEta[3];   //[selectedIDIsoMuonsNum]
  Float_t         selectedIDIsoMuonsPhi[3];   //[selectedIDIsoMuonsNum]
  Float_t         selectedIDIsoMuonsE[3];   //[selectedIDIsoMuonsNum]
  Float_t         selectedIDIsoMuons_MTW[3];   //[selectedIDIsoMuonsNum]
  UShort_t        selectedIDMuonsNum;
  Float_t         selectedIDMuonsPt[5];   //[selectedIDMuonsNum]
  Float_t         selectedIDMuonsEta[5];   //[selectedIDMuonsNum]
  Float_t         selectedIDMuonsPhi[5];   //[selectedIDMuonsNum]
  Float_t         selectedIDMuonsE[5];   //[selectedIDMuonsNum]
  Float_t         selectedIDMuons_MTW[5];   //[selectedIDMuonsNum]
  UShort_t        selectedIDIsoElectronsNum;
  Float_t         selectedIDIsoElectronsPt[4];   //[selectedIDIsoElectronsNum]
  Float_t         selectedIDIsoElectronsEta[4];   //[selectedIDIsoElectronsNum]
  Float_t         selectedIDIsoElectronsPhi[4];   //[selectedIDIsoElectronsNum]
  Float_t         selectedIDIsoElectronsE[4];   //[selectedIDIsoElectronsNum]
  Float_t         selectedIDIsoElectrons_MTW[4];   //[selectedIDIsoElectronsNum]
  UShort_t        selectedIDElectronsNum;
  Float_t         selectedIDElectronsPt[5];   //[selectedIDElectronsNum]
  Float_t         selectedIDElectronsEta[5];   //[selectedIDElectronsNum]
  Float_t         selectedIDElectronsPhi[5];   //[selectedIDElectronsNum]
  Float_t         selectedIDElectronsE[5];   //[selectedIDElectronsNum]
  Float_t         selectedIDElectrons_MTW[5];   //[selectedIDElectronsNum]
  UShort_t        SelectedPFCandidatesNum;
  Float_t         SelectedPFCandidatesPt[182];   //[SelectedPFCandidatesNum]
  Float_t         SelectedPFCandidatesEta[182];   //[SelectedPFCandidatesNum]
  Float_t         SelectedPFCandidatesPhi[182];   //[SelectedPFCandidatesNum]
  Float_t         SelectedPFCandidatesE[182];   //[SelectedPFCandidatesNum]
  Int_t           SelectedPFCandidates_Charge[182];   //[SelectedPFCandidatesNum]
  Int_t           SelectedPFCandidates_Typ[182];   //[SelectedPFCandidatesNum]
  UShort_t        GenBosonNum;
  Float_t         GenBosonPt[4];   //[GenBosonNum]
  Float_t         GenBosonEta[4];   //[GenBosonNum]
  Float_t         GenBosonPhi[4];   //[GenBosonNum]
  Float_t         GenBosonE[4];   //[GenBosonNum]
  Int_t           GenBoson_GenBosonPDGId[4];   //[GenBosonNum]
  UShort_t        GenMuNum;
  Float_t         GenMuPt[3];   //[GenMuNum]
  Float_t         GenMuEta[3];   //[GenMuNum]
  Float_t         GenMuPhi[3];   //[GenMuNum]
  Float_t         GenMuE[3];   //[GenMuNum]
  Int_t           GenMu_GenMuFromTau[3];   //[GenMuNum]
  UShort_t        GenElecNum;
  Float_t         GenElecPt[4];   //[GenElecNum]
  Float_t         GenElecEta[4];   //[GenElecNum]
  Float_t         GenElecPhi[4];   //[GenElecNum]
  Float_t         GenElecE[4];   //[GenElecNum]
  Int_t           GenElec_GenElecFromTau[4];   //[GenElecNum]
  UShort_t        GenTauNum;
  Float_t         GenTauPt[4];   //[GenTauNum]
  Float_t         GenTauEta[4];   //[GenTauNum]
  Float_t         GenTauPhi[4];   //[GenTauNum]
  Float_t         GenTauE[4];   //[GenTauNum]
  Int_t           GenTau_GenTauHad[4];   //[GenTauNum]
  UShort_t        TauDecayCandsNum;
  Float_t         TauDecayCandsPt[9];   //[TauDecayCandsNum]
  Float_t         TauDecayCandsEta[9];   //[TauDecayCandsNum]
  Float_t         TauDecayCandsPhi[9];   //[TauDecayCandsNum]
  Float_t         TauDecayCandsE[9];   //[TauDecayCandsNum]
  Int_t           TauDecayCands_pdgID[9];   //[TauDecayCandsNum]
  UShort_t        selectedIDIsoMuonsNoMiniIsoNum;
  Float_t         selectedIDIsoMuonsNoMiniIsoPt[3];   //[selectedIDIsoMuonsNoMiniIsoNum]
  Float_t         selectedIDIsoMuonsNoMiniIsoEta[3];   //[selectedIDIsoMuonsNoMiniIsoNum]
  Float_t         selectedIDIsoMuonsNoMiniIsoPhi[3];   //[selectedIDIsoMuonsNoMiniIsoNum]
  Float_t         selectedIDIsoMuonsNoMiniIsoE[3];   //[selectedIDIsoMuonsNoMiniIsoNum]
  UShort_t        selectedIDIsoElectronsNoMiniIsoNum;
  Float_t         selectedIDIsoElectronsNoMiniIsoPt[4];   //[selectedIDIsoElectronsNoMiniIsoNum]
  Float_t         selectedIDIsoElectronsNoMiniIsoEta[4];   //[selectedIDIsoElectronsNoMiniIsoNum]
  Float_t         selectedIDIsoElectronsNoMiniIsoPhi[4];   //[selectedIDIsoElectronsNoMiniIsoNum]
  Float_t         selectedIDIsoElectronsNoMiniIsoE[4];   //[selectedIDIsoElectronsNoMiniIsoNum]
  UShort_t        JetsNum;
  Float_t         JetsPt[17];   //[JetsNum]
  Float_t         JetsEta[17];   //[JetsNum]
  Float_t         JetsPhi[17];   //[JetsNum]
  Float_t         JetsE[17];   //[JetsNum]
  Float_t         Jets_bDiscriminator[17];   //[JetsNum]
  Float_t         Jets_chargedEmEnergyFraction[17];   //[JetsNum]
  Float_t         Jets_chargedHadronEnergyFraction[17];   //[JetsNum]
  Int_t           Jets_chargedHadronMultiplicity[17];   //[JetsNum]
  Int_t           Jets_electronMultiplicity[17];   //[JetsNum]
  Float_t         Jets_jetArea[17];   //[JetsNum]
  Float_t         Jets_muonEnergyFraction[17];   //[JetsNum]
  Int_t           Jets_muonMultiplicity[17];   //[JetsNum]
  Float_t         Jets_neutralEmEnergyFraction[17];   //[JetsNum]
  Int_t           Jets_neutralHadronMultiplicity[17];   //[JetsNum]
  Float_t         Jets_photonEnergyFraction[17];   //[JetsNum]
  Int_t           Jets_photonMultiplicity[17];   //[JetsNum]
  UShort_t        slimmedElectronsNum;
  Float_t         slimmedElectronsPt[10];   //[slimmedElectronsNum]
  Float_t         slimmedElectronsEta[10];   //[slimmedElectronsNum]
  Float_t         slimmedElectronsPhi[10];   //[slimmedElectronsNum]
  Float_t         slimmedElectronsE[10];   //[slimmedElectronsNum]
  UShort_t        slimmedMuonsNum;
  Float_t         slimmedMuonsPt[29];   //[slimmedMuonsNum]
  Float_t         slimmedMuonsEta[29];   //[slimmedMuonsNum]
  Float_t         slimmedMuonsPhi[29];   //[slimmedMuonsNum]
  Float_t         slimmedMuonsE[29];   //[slimmedMuonsNum]
  
  // List of branches
  TBranch        *b_RunNum;   //!
  TBranch        *b_LumiBlockNum;   //!
  TBranch        *b_EvtNum;   //!
  TBranch        *b_NVtx;   //!
  TBranch        *b_isoElectronTracks;   //!
  TBranch        *b_isoMuonTracks;   //!
  TBranch        *b_isoPionTracks;   //!
  TBranch        *b_Leptons;   //!
  TBranch        *b_TagLeptonHighPT;   //!
  TBranch        *b_NJets;   //!
  TBranch        *b_BTags;   //!
  TBranch        *b_Weight;   //!
  TBranch        *b_HT;   //!
  TBranch        *b_MHT;   //!
  TBranch        *b_DeltaPhi1;   //!
  TBranch        *b_DeltaPhi2;   //!
  TBranch        *b_DeltaPhi3;   //!
  TBranch        *b_minDeltaPhiN;   //!
  TBranch        *b_DeltaPhiN1;   //!
  TBranch        *b_DeltaPhiN2;   //!
  TBranch        *b_DeltaPhiN3;   //!
  TBranch        *b_METPt;   //!
  TBranch        *b_METPhi;   //!
  TBranch        *b_JetID;   //!
  TBranch        *b_MuonsNum;   //!
  TBranch        *b_MuonsPt;   //!
  TBranch        *b_MuonsEta;   //!
  TBranch        *b_MuonsPhi;   //!
  TBranch        *b_MuonsE;   //!
  TBranch        *b_ElectronsNum;   //!
  TBranch        *b_ElectronsPt;   //!
  TBranch        *b_ElectronsEta;   //!
  TBranch        *b_ElectronsPhi;   //!
  TBranch        *b_ElectronsE;   //!
  TBranch        *b_IsolatedElectronTracksVetoNum;   //!
  TBranch        *b_IsolatedElectronTracksVetoPt;   //!
  TBranch        *b_IsolatedElectronTracksVetoEta;   //!
  TBranch        *b_IsolatedElectronTracksVetoPhi;   //!
  TBranch        *b_IsolatedElectronTracksVetoE;   //!
  TBranch        *b_IsolatedElectronTracksVeto_MT;   //!
  TBranch        *b_IsolatedMuonTracksVetoNum;   //!
  TBranch        *b_IsolatedMuonTracksVetoPt;   //!
  TBranch        *b_IsolatedMuonTracksVetoEta;   //!
  TBranch        *b_IsolatedMuonTracksVetoPhi;   //!
  TBranch        *b_IsolatedMuonTracksVetoE;   //!
  TBranch        *b_IsolatedMuonTracksVeto_MT;   //!
  TBranch        *b_IsolatedPionTracksVetoNum;   //!
  TBranch        *b_IsolatedPionTracksVetoPt;   //!
  TBranch        *b_IsolatedPionTracksVetoEta;   //!
  TBranch        *b_IsolatedPionTracksVetoPhi;   //!
  TBranch        *b_IsolatedPionTracksVetoE;   //!
  TBranch        *b_IsolatedPionTracksVeto_MT;   //!
  TBranch        *b_selectedIDIsoMuonsNum;   //!
  TBranch        *b_selectedIDIsoMuonsPt;   //!
  TBranch        *b_selectedIDIsoMuonsEta;   //!
  TBranch        *b_selectedIDIsoMuonsPhi;   //!
  TBranch        *b_selectedIDIsoMuonsE;   //!
  TBranch        *b_selectedIDIsoMuons_MTW;   //!
  TBranch        *b_selectedIDMuonsNum;   //!
  TBranch        *b_selectedIDMuonsPt;   //!
  TBranch        *b_selectedIDMuonsEta;   //!
  TBranch        *b_selectedIDMuonsPhi;   //!
  TBranch        *b_selectedIDMuonsE;   //!
  TBranch        *b_selectedIDMuons_MTW;   //!
  TBranch        *b_selectedIDIsoElectronsNum;   //!
  TBranch        *b_selectedIDIsoElectronsPt;   //!
  TBranch        *b_selectedIDIsoElectronsEta;   //!
  TBranch        *b_selectedIDIsoElectronsPhi;   //!
  TBranch        *b_selectedIDIsoElectronsE;   //!
  TBranch        *b_selectedIDIsoElectrons_MTW;   //!
  TBranch        *b_selectedIDElectronsNum;   //!
  TBranch        *b_selectedIDElectronsPt;   //!
  TBranch        *b_selectedIDElectronsEta;   //!
  TBranch        *b_selectedIDElectronsPhi;   //!
  TBranch        *b_selectedIDElectronsE;   //!
  TBranch        *b_selectedIDElectrons_MTW;   //!
  TBranch        *b_SelectedPFCandidatesNum;   //!
  TBranch        *b_SelectedPFCandidatesPt;   //!
  TBranch        *b_SelectedPFCandidatesEta;   //!
  TBranch        *b_SelectedPFCandidatesPhi;   //!
  TBranch        *b_SelectedPFCandidatesE;   //!
  TBranch        *b_SelectedPFCandidates_Charge;   //!
  TBranch        *b_SelectedPFCandidates_Typ;   //!
  TBranch        *b_GenBosonNum;   //!
  TBranch        *b_GenBosonPt;   //!
  TBranch        *b_GenBosonEta;   //!
  TBranch        *b_GenBosonPhi;   //!
  TBranch        *b_GenBosonE;   //!
  TBranch        *b_GenBoson_GenBosonPDGId;   //!
  TBranch        *b_GenMuNum;   //!
  TBranch        *b_GenMuPt;   //!
  TBranch        *b_GenMuEta;   //!
  TBranch        *b_GenMuPhi;   //!
  TBranch        *b_GenMuE;   //!
  TBranch        *b_GenMu_GenMuFromTau;   //!
  TBranch        *b_GenElecNum;   //!
  TBranch        *b_GenElecPt;   //!
  TBranch        *b_GenElecEta;   //!
  TBranch        *b_GenElecPhi;   //!
  TBranch        *b_GenElecE;   //!
  TBranch        *b_GenElec_GenElecFromTau;   //!
  TBranch        *b_GenTauNum;   //!
  TBranch        *b_GenTauPt;   //!
  TBranch        *b_GenTauEta;   //!
  TBranch        *b_GenTauPhi;   //!
  TBranch        *b_GenTauE;   //!
  TBranch        *b_GenTau_GenTauHad;   //!
  TBranch        *b_TauDecayCandsNum;   //!
  TBranch        *b_TauDecayCandsPt;   //!
  TBranch        *b_TauDecayCandsEta;   //!
  TBranch        *b_TauDecayCandsPhi;   //!
  TBranch        *b_TauDecayCandsE;   //!
  TBranch        *b_TauDecayCands_pdgID;   //!
  TBranch        *b_selectedIDIsoMuonsNoMiniIsoNum;   //!
  TBranch        *b_selectedIDIsoMuonsNoMiniIsoPt;   //!
  TBranch        *b_selectedIDIsoMuonsNoMiniIsoEta;   //!
  TBranch        *b_selectedIDIsoMuonsNoMiniIsoPhi;   //!
  TBranch        *b_selectedIDIsoMuonsNoMiniIsoE;   //!
  TBranch        *b_selectedIDIsoElectronsNoMiniIsoNum;   //!
  TBranch        *b_selectedIDIsoElectronsNoMiniIsoPt;   //!
  TBranch        *b_selectedIDIsoElectronsNoMiniIsoEta;   //!
  TBranch        *b_selectedIDIsoElectronsNoMiniIsoPhi;   //!
  TBranch        *b_selectedIDIsoElectronsNoMiniIsoE;   //!
  TBranch        *b_JetsNum;   //!
  TBranch        *b_JetsPt;   //!
  TBranch        *b_JetsEta;   //!
  TBranch        *b_JetsPhi;   //!
  TBranch        *b_JetsE;   //!
  TBranch        *b_Jets_bDiscriminator;   //!
  TBranch        *b_Jets_chargedEmEnergyFraction;   //!
  TBranch        *b_Jets_chargedHadronEnergyFraction;   //!
  TBranch        *b_Jets_chargedHadronMultiplicity;   //!
  TBranch        *b_Jets_electronMultiplicity;   //!
  TBranch        *b_Jets_jetArea;   //!
  TBranch        *b_Jets_muonEnergyFraction;   //!
  TBranch        *b_Jets_muonMultiplicity;   //!
  TBranch        *b_Jets_neutralEmEnergyFraction;   //!
  TBranch        *b_Jets_neutralHadronMultiplicity;   //!
  TBranch        *b_Jets_photonEnergyFraction;   //!
  TBranch        *b_Jets_photonMultiplicity;   //!
  TBranch        *b_slimmedElectronsNum;   //!
  TBranch        *b_slimmedElectronsPt;   //!
  TBranch        *b_slimmedElectronsEta;   //!
  TBranch        *b_slimmedElectronsPhi;   //!
  TBranch        *b_slimmedElectronsE;   //!
  TBranch        *b_slimmedMuonsNum;   //!
  TBranch        *b_slimmedMuonsPt;   //!
  TBranch        *b_slimmedMuonsEta;   //!
  TBranch        *b_slimmedMuonsPhi;   //!
  TBranch        *b_slimmedMuonsE;   //!
  
  Prediction(TTree * /*tree*/ =0) : fChain(0) { }
  virtual ~Prediction() { }
  virtual Int_t   Version() const { return 2; }
  virtual void    Begin(TTree *tree);
  virtual void    SlaveBegin(TTree *tree);
  virtual void    Init(TTree *tree);
  virtual Bool_t  Notify();
  virtual Bool_t  Process(Long64_t entry);
  virtual Int_t   GetEntry(Long64_t entry, Int_t getall = 0) { return fChain ? fChain->GetTree()->GetEntry(entry, getall) : 0; }
  virtual void    SetOption(const char *option) { fOption = option; }
  virtual void    SetObject(TObject *obj) { fObject = obj; }
  virtual void    SetInputList(TList *input) { fInput = input; }
  virtual TList  *GetOutputList() const { return fOutput; }
  virtual void    SlaveTerminate();
  virtual void    Terminate();
  
  ClassDef(Prediction,0);
};

#endif

#ifdef Prediction_cxx
void Prediction::Init(TTree *tree)
{
  // The Init() function is called when the selector needs to initialize
  // a new tree or chain. Typically here the branch addresses and branch
  // pointers of the tree will be set.
  // It is normally not necessary to make changes to the generated
  // code, but the routine can be extended by the user if needed.
  // Init() will be called many times when running on PROOF
  // (once per file to be processed).
  
  // Set branch addresses and branch pointers
  if (!tree) return;
  fChain = tree;
  fChain->SetMakeClass(1);
  
  fChain->SetBranchAddress("RunNum", &RunNum, &b_RunNum);
  fChain->SetBranchAddress("LumiBlockNum", &LumiBlockNum, &b_LumiBlockNum);
  fChain->SetBranchAddress("EvtNum", &EvtNum, &b_EvtNum);
  fChain->SetBranchAddress("NVtx", &NVtx, &b_NVtx);
  fChain->SetBranchAddress("isoElectronTracks", &isoElectronTracks, &b_isoElectronTracks);
  fChain->SetBranchAddress("isoMuonTracks", &isoMuonTracks, &b_isoMuonTracks);
  fChain->SetBranchAddress("isoPionTracks", &isoPionTracks, &b_isoPionTracks);
  fChain->SetBranchAddress("Leptons", &Leptons, &b_Leptons);
  fChain->SetBranchAddress("TagLeptonHighPT", &TagLeptonHighPT, &b_TagLeptonHighPT);
  fChain->SetBranchAddress("NJets", &NJets, &b_NJets);
  fChain->SetBranchAddress("BTags", &BTags, &b_BTags);
  fChain->SetBranchAddress("Weight", &Weight, &b_Weight);
  fChain->SetBranchAddress("HT", &HT, &b_HT);
  fChain->SetBranchAddress("MHT", &MHT, &b_MHT);
  fChain->SetBranchAddress("DeltaPhi1", &DeltaPhi1, &b_DeltaPhi1);
  fChain->SetBranchAddress("DeltaPhi2", &DeltaPhi2, &b_DeltaPhi2);
  fChain->SetBranchAddress("DeltaPhi3", &DeltaPhi3, &b_DeltaPhi3);
  fChain->SetBranchAddress("minDeltaPhiN", &minDeltaPhiN, &b_minDeltaPhiN);
  fChain->SetBranchAddress("DeltaPhiN1", &DeltaPhiN1, &b_DeltaPhiN1);
  fChain->SetBranchAddress("DeltaPhiN2", &DeltaPhiN2, &b_DeltaPhiN2);
  fChain->SetBranchAddress("DeltaPhiN3", &DeltaPhiN3, &b_DeltaPhiN3);
  fChain->SetBranchAddress("METPt", &METPt, &b_METPt);
  fChain->SetBranchAddress("METPhi", &METPhi, &b_METPhi);
  fChain->SetBranchAddress("JetID", &JetID, &b_JetID);
  fChain->SetBranchAddress("MuonsNum", &MuonsNum, &b_MuonsNum);
  fChain->SetBranchAddress("MuonsPt", MuonsPt, &b_MuonsPt);
  fChain->SetBranchAddress("MuonsEta", MuonsEta, &b_MuonsEta);
  fChain->SetBranchAddress("MuonsPhi", MuonsPhi, &b_MuonsPhi);
  fChain->SetBranchAddress("MuonsE", MuonsE, &b_MuonsE);
  fChain->SetBranchAddress("ElectronsNum", &ElectronsNum, &b_ElectronsNum);
  fChain->SetBranchAddress("ElectronsPt", ElectronsPt, &b_ElectronsPt);
  fChain->SetBranchAddress("ElectronsEta", ElectronsEta, &b_ElectronsEta);
  fChain->SetBranchAddress("ElectronsPhi", ElectronsPhi, &b_ElectronsPhi);
  fChain->SetBranchAddress("ElectronsE", ElectronsE, &b_ElectronsE);
  fChain->SetBranchAddress("IsolatedElectronTracksVetoNum", &IsolatedElectronTracksVetoNum, &b_IsolatedElectronTracksVetoNum);
  fChain->SetBranchAddress("IsolatedElectronTracksVetoPt", IsolatedElectronTracksVetoPt, &b_IsolatedElectronTracksVetoPt);
  fChain->SetBranchAddress("IsolatedElectronTracksVetoEta", IsolatedElectronTracksVetoEta, &b_IsolatedElectronTracksVetoEta);
  fChain->SetBranchAddress("IsolatedElectronTracksVetoPhi", IsolatedElectronTracksVetoPhi, &b_IsolatedElectronTracksVetoPhi);
  fChain->SetBranchAddress("IsolatedElectronTracksVetoE", IsolatedElectronTracksVetoE, &b_IsolatedElectronTracksVetoE);
  fChain->SetBranchAddress("IsolatedElectronTracksVeto_MT", IsolatedElectronTracksVeto_MT, &b_IsolatedElectronTracksVeto_MT);
  fChain->SetBranchAddress("IsolatedMuonTracksVetoNum", &IsolatedMuonTracksVetoNum, &b_IsolatedMuonTracksVetoNum);
  fChain->SetBranchAddress("IsolatedMuonTracksVetoPt", IsolatedMuonTracksVetoPt, &b_IsolatedMuonTracksVetoPt);
  fChain->SetBranchAddress("IsolatedMuonTracksVetoEta", IsolatedMuonTracksVetoEta, &b_IsolatedMuonTracksVetoEta);
  fChain->SetBranchAddress("IsolatedMuonTracksVetoPhi", IsolatedMuonTracksVetoPhi, &b_IsolatedMuonTracksVetoPhi);
  fChain->SetBranchAddress("IsolatedMuonTracksVetoE", IsolatedMuonTracksVetoE, &b_IsolatedMuonTracksVetoE);
  fChain->SetBranchAddress("IsolatedMuonTracksVeto_MT", IsolatedMuonTracksVeto_MT, &b_IsolatedMuonTracksVeto_MT);
  fChain->SetBranchAddress("IsolatedPionTracksVetoNum", &IsolatedPionTracksVetoNum, &b_IsolatedPionTracksVetoNum);
  fChain->SetBranchAddress("IsolatedPionTracksVetoPt", IsolatedPionTracksVetoPt, &b_IsolatedPionTracksVetoPt);
  fChain->SetBranchAddress("IsolatedPionTracksVetoEta", IsolatedPionTracksVetoEta, &b_IsolatedPionTracksVetoEta);
  fChain->SetBranchAddress("IsolatedPionTracksVetoPhi", IsolatedPionTracksVetoPhi, &b_IsolatedPionTracksVetoPhi);
  fChain->SetBranchAddress("IsolatedPionTracksVetoE", IsolatedPionTracksVetoE, &b_IsolatedPionTracksVetoE);
  fChain->SetBranchAddress("IsolatedPionTracksVeto_MT", IsolatedPionTracksVeto_MT, &b_IsolatedPionTracksVeto_MT);
  fChain->SetBranchAddress("selectedIDIsoMuonsNum", &selectedIDIsoMuonsNum, &b_selectedIDIsoMuonsNum);
  fChain->SetBranchAddress("selectedIDIsoMuonsPt", selectedIDIsoMuonsPt, &b_selectedIDIsoMuonsPt);
  fChain->SetBranchAddress("selectedIDIsoMuonsEta", selectedIDIsoMuonsEta, &b_selectedIDIsoMuonsEta);
  fChain->SetBranchAddress("selectedIDIsoMuonsPhi", selectedIDIsoMuonsPhi, &b_selectedIDIsoMuonsPhi);
  fChain->SetBranchAddress("selectedIDIsoMuonsE", selectedIDIsoMuonsE, &b_selectedIDIsoMuonsE);
  fChain->SetBranchAddress("selectedIDIsoMuons_MTW", selectedIDIsoMuons_MTW, &b_selectedIDIsoMuons_MTW);
  fChain->SetBranchAddress("selectedIDMuonsNum", &selectedIDMuonsNum, &b_selectedIDMuonsNum);
  fChain->SetBranchAddress("selectedIDMuonsPt", selectedIDMuonsPt, &b_selectedIDMuonsPt);
  fChain->SetBranchAddress("selectedIDMuonsEta", selectedIDMuonsEta, &b_selectedIDMuonsEta);
  fChain->SetBranchAddress("selectedIDMuonsPhi", selectedIDMuonsPhi, &b_selectedIDMuonsPhi);
  fChain->SetBranchAddress("selectedIDMuonsE", selectedIDMuonsE, &b_selectedIDMuonsE);
  fChain->SetBranchAddress("selectedIDMuons_MTW", selectedIDMuons_MTW, &b_selectedIDMuons_MTW);
  fChain->SetBranchAddress("selectedIDIsoElectronsNum", &selectedIDIsoElectronsNum, &b_selectedIDIsoElectronsNum);
  fChain->SetBranchAddress("selectedIDIsoElectronsPt", selectedIDIsoElectronsPt, &b_selectedIDIsoElectronsPt);
  fChain->SetBranchAddress("selectedIDIsoElectronsEta", selectedIDIsoElectronsEta, &b_selectedIDIsoElectronsEta);
  fChain->SetBranchAddress("selectedIDIsoElectronsPhi", selectedIDIsoElectronsPhi, &b_selectedIDIsoElectronsPhi);
  fChain->SetBranchAddress("selectedIDIsoElectronsE", selectedIDIsoElectronsE, &b_selectedIDIsoElectronsE);
  fChain->SetBranchAddress("selectedIDIsoElectrons_MTW", selectedIDIsoElectrons_MTW, &b_selectedIDIsoElectrons_MTW);
  fChain->SetBranchAddress("selectedIDElectronsNum", &selectedIDElectronsNum, &b_selectedIDElectronsNum);
  fChain->SetBranchAddress("selectedIDElectronsPt", selectedIDElectronsPt, &b_selectedIDElectronsPt);
  fChain->SetBranchAddress("selectedIDElectronsEta", selectedIDElectronsEta, &b_selectedIDElectronsEta);
  fChain->SetBranchAddress("selectedIDElectronsPhi", selectedIDElectronsPhi, &b_selectedIDElectronsPhi);
  fChain->SetBranchAddress("selectedIDElectronsE", selectedIDElectronsE, &b_selectedIDElectronsE);
  fChain->SetBranchAddress("selectedIDElectrons_MTW", selectedIDElectrons_MTW, &b_selectedIDElectrons_MTW);
  fChain->SetBranchAddress("SelectedPFCandidatesNum", &SelectedPFCandidatesNum, &b_SelectedPFCandidatesNum);
  fChain->SetBranchAddress("SelectedPFCandidatesPt", SelectedPFCandidatesPt, &b_SelectedPFCandidatesPt);
  fChain->SetBranchAddress("SelectedPFCandidatesEta", SelectedPFCandidatesEta, &b_SelectedPFCandidatesEta);
  fChain->SetBranchAddress("SelectedPFCandidatesPhi", SelectedPFCandidatesPhi, &b_SelectedPFCandidatesPhi);
  fChain->SetBranchAddress("SelectedPFCandidatesE", SelectedPFCandidatesE, &b_SelectedPFCandidatesE);
  fChain->SetBranchAddress("SelectedPFCandidates_Charge", SelectedPFCandidates_Charge, &b_SelectedPFCandidates_Charge);
  fChain->SetBranchAddress("SelectedPFCandidates_Typ", SelectedPFCandidates_Typ, &b_SelectedPFCandidates_Typ);
  fChain->SetBranchAddress("GenBosonNum", &GenBosonNum, &b_GenBosonNum);
  fChain->SetBranchAddress("GenBosonPt", GenBosonPt, &b_GenBosonPt);
  fChain->SetBranchAddress("GenBosonEta", GenBosonEta, &b_GenBosonEta);
  fChain->SetBranchAddress("GenBosonPhi", GenBosonPhi, &b_GenBosonPhi);
  fChain->SetBranchAddress("GenBosonE", GenBosonE, &b_GenBosonE);
  fChain->SetBranchAddress("GenBoson_GenBosonPDGId", GenBoson_GenBosonPDGId, &b_GenBoson_GenBosonPDGId);
  fChain->SetBranchAddress("GenMuNum", &GenMuNum, &b_GenMuNum);
  fChain->SetBranchAddress("GenMuPt", GenMuPt, &b_GenMuPt);
  fChain->SetBranchAddress("GenMuEta", GenMuEta, &b_GenMuEta);
  fChain->SetBranchAddress("GenMuPhi", GenMuPhi, &b_GenMuPhi);
  fChain->SetBranchAddress("GenMuE", GenMuE, &b_GenMuE);
  fChain->SetBranchAddress("GenMu_GenMuFromTau", GenMu_GenMuFromTau, &b_GenMu_GenMuFromTau);
  fChain->SetBranchAddress("GenElecNum", &GenElecNum, &b_GenElecNum);
  fChain->SetBranchAddress("GenElecPt", GenElecPt, &b_GenElecPt);
  fChain->SetBranchAddress("GenElecEta", GenElecEta, &b_GenElecEta);
  fChain->SetBranchAddress("GenElecPhi", GenElecPhi, &b_GenElecPhi);
  fChain->SetBranchAddress("GenElecE", GenElecE, &b_GenElecE);
  fChain->SetBranchAddress("GenElec_GenElecFromTau", GenElec_GenElecFromTau, &b_GenElec_GenElecFromTau);
  fChain->SetBranchAddress("GenTauNum", &GenTauNum, &b_GenTauNum);
  fChain->SetBranchAddress("GenTauPt", GenTauPt, &b_GenTauPt);
  fChain->SetBranchAddress("GenTauEta", GenTauEta, &b_GenTauEta);
  fChain->SetBranchAddress("GenTauPhi", GenTauPhi, &b_GenTauPhi);
  fChain->SetBranchAddress("GenTauE", GenTauE, &b_GenTauE);
  fChain->SetBranchAddress("GenTau_GenTauHad", GenTau_GenTauHad, &b_GenTau_GenTauHad);
  fChain->SetBranchAddress("TauDecayCandsNum", &TauDecayCandsNum, &b_TauDecayCandsNum);
  fChain->SetBranchAddress("TauDecayCandsPt", TauDecayCandsPt, &b_TauDecayCandsPt);
  fChain->SetBranchAddress("TauDecayCandsEta", TauDecayCandsEta, &b_TauDecayCandsEta);
  fChain->SetBranchAddress("TauDecayCandsPhi", TauDecayCandsPhi, &b_TauDecayCandsPhi);
  fChain->SetBranchAddress("TauDecayCandsE", TauDecayCandsE, &b_TauDecayCandsE);
  fChain->SetBranchAddress("TauDecayCands_pdgID", TauDecayCands_pdgID, &b_TauDecayCands_pdgID);
  fChain->SetBranchAddress("selectedIDIsoMuonsNoMiniIsoNum", &selectedIDIsoMuonsNoMiniIsoNum, &b_selectedIDIsoMuonsNoMiniIsoNum);
  fChain->SetBranchAddress("selectedIDIsoMuonsNoMiniIsoPt", selectedIDIsoMuonsNoMiniIsoPt, &b_selectedIDIsoMuonsNoMiniIsoPt);
  fChain->SetBranchAddress("selectedIDIsoMuonsNoMiniIsoEta", selectedIDIsoMuonsNoMiniIsoEta, &b_selectedIDIsoMuonsNoMiniIsoEta);
  fChain->SetBranchAddress("selectedIDIsoMuonsNoMiniIsoPhi", selectedIDIsoMuonsNoMiniIsoPhi, &b_selectedIDIsoMuonsNoMiniIsoPhi);
  fChain->SetBranchAddress("selectedIDIsoMuonsNoMiniIsoE", selectedIDIsoMuonsNoMiniIsoE, &b_selectedIDIsoMuonsNoMiniIsoE);
  fChain->SetBranchAddress("selectedIDIsoElectronsNoMiniIsoNum", &selectedIDIsoElectronsNoMiniIsoNum, &b_selectedIDIsoElectronsNoMiniIsoNum);
  fChain->SetBranchAddress("selectedIDIsoElectronsNoMiniIsoPt", selectedIDIsoElectronsNoMiniIsoPt, &b_selectedIDIsoElectronsNoMiniIsoPt);
  fChain->SetBranchAddress("selectedIDIsoElectronsNoMiniIsoEta", selectedIDIsoElectronsNoMiniIsoEta, &b_selectedIDIsoElectronsNoMiniIsoEta);
  fChain->SetBranchAddress("selectedIDIsoElectronsNoMiniIsoPhi", selectedIDIsoElectronsNoMiniIsoPhi, &b_selectedIDIsoElectronsNoMiniIsoPhi);
  fChain->SetBranchAddress("selectedIDIsoElectronsNoMiniIsoE", selectedIDIsoElectronsNoMiniIsoE, &b_selectedIDIsoElectronsNoMiniIsoE);
  fChain->SetBranchAddress("JetsNum", &JetsNum, &b_JetsNum);
  fChain->SetBranchAddress("JetsPt", JetsPt, &b_JetsPt);
  fChain->SetBranchAddress("JetsEta", JetsEta, &b_JetsEta);
  fChain->SetBranchAddress("JetsPhi", JetsPhi, &b_JetsPhi);
  fChain->SetBranchAddress("JetsE", JetsE, &b_JetsE);
  fChain->SetBranchAddress("Jets_bDiscriminator", Jets_bDiscriminator, &b_Jets_bDiscriminator);
  fChain->SetBranchAddress("Jets_chargedEmEnergyFraction", Jets_chargedEmEnergyFraction, &b_Jets_chargedEmEnergyFraction);
  fChain->SetBranchAddress("Jets_chargedHadronEnergyFraction", Jets_chargedHadronEnergyFraction, &b_Jets_chargedHadronEnergyFraction);
  fChain->SetBranchAddress("Jets_chargedHadronMultiplicity", Jets_chargedHadronMultiplicity, &b_Jets_chargedHadronMultiplicity);
  fChain->SetBranchAddress("Jets_electronMultiplicity", Jets_electronMultiplicity, &b_Jets_electronMultiplicity);
  fChain->SetBranchAddress("Jets_jetArea", Jets_jetArea, &b_Jets_jetArea);
  fChain->SetBranchAddress("Jets_muonEnergyFraction", Jets_muonEnergyFraction, &b_Jets_muonEnergyFraction);
  fChain->SetBranchAddress("Jets_muonMultiplicity", Jets_muonMultiplicity, &b_Jets_muonMultiplicity);
  fChain->SetBranchAddress("Jets_neutralEmEnergyFraction", Jets_neutralEmEnergyFraction, &b_Jets_neutralEmEnergyFraction);
  fChain->SetBranchAddress("Jets_neutralHadronMultiplicity", Jets_neutralHadronMultiplicity, &b_Jets_neutralHadronMultiplicity);
  fChain->SetBranchAddress("Jets_photonEnergyFraction", Jets_photonEnergyFraction, &b_Jets_photonEnergyFraction);
  fChain->SetBranchAddress("Jets_photonMultiplicity", Jets_photonMultiplicity, &b_Jets_photonMultiplicity);
  fChain->SetBranchAddress("slimmedElectronsNum", &slimmedElectronsNum, &b_slimmedElectronsNum);
  fChain->SetBranchAddress("slimmedElectronsPt", slimmedElectronsPt, &b_slimmedElectronsPt);
  fChain->SetBranchAddress("slimmedElectronsEta", slimmedElectronsEta, &b_slimmedElectronsEta);
  fChain->SetBranchAddress("slimmedElectronsPhi", slimmedElectronsPhi, &b_slimmedElectronsPhi);
  fChain->SetBranchAddress("slimmedElectronsE", slimmedElectronsE, &b_slimmedElectronsE);
  fChain->SetBranchAddress("slimmedMuonsNum", &slimmedMuonsNum, &b_slimmedMuonsNum);
  fChain->SetBranchAddress("slimmedMuonsPt", slimmedMuonsPt, &b_slimmedMuonsPt);
  fChain->SetBranchAddress("slimmedMuonsEta", slimmedMuonsEta, &b_slimmedMuonsEta);
  fChain->SetBranchAddress("slimmedMuonsPhi", slimmedMuonsPhi, &b_slimmedMuonsPhi);
  fChain->SetBranchAddress("slimmedMuonsE", slimmedMuonsE, &b_slimmedMuonsE);
}

Bool_t Prediction::Notify()
{
  // The Notify() function is called when a new file is opened. This
  // can be either for a new TTree in a TChain or when when a new TTree
  // is started when using PROOF. It is normally not necessary to make changes
  // to the generated code, but the routine can be extended by the
  // user if needed. The return value is currently not used.
  
  return kTRUE;
}

#endif // #ifdef Prediction_cxx
