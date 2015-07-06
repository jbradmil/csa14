//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Mar 18 14:50:47 2015 by ROOT version 5.34/04
// from TTree PreSelection/PreSelection
// found on file: DY_MC.root
//////////////////////////////////////////////////////////

#ifndef TagAndProbe_h
#define TagAndProbe_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>
#include <TSelector.h>
#include <TH2F.h>
#include <TH1F.h>
#include "TVector2.h"
#include <cmath>
#include "TCanvas.h"

// cuts baseline
const double minHT_=400;
const double minNJets_=3.4;
const double minDeltaPhiN_=-9999;
const bool applyFilters_=true;
const bool truthMatching_=true;

// for truth truthMatching_		
const double maxDeltaRGenToRecoMu_ =0.3;
const double maxDiffPtGenToRecoMu_ =0.5;
const double maxDeltaRGenToRecoElec_ =0.3;
const double maxDiffPtGenToRecoElec_ =0.3;

// actity around lepton
const double maxDeltaRMuActivity_=1.0;
const double maxDeltaRElecActivity_=1.0;
const double maxDeltaRIsoTrackActivity_=1.0;
const unsigned int elecActivityMethod_=0;               // ###############
const unsigned int muActivityMethod_=0;                 // ###############
const unsigned int isoTrackActivityMethod_=0;
// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class TagAndProbe : public TSelector {
public :
	void resetValues();
	bool FiltersPass();
	double deltaR(double eta1, double phi1, double eta2, double phi2);
	double MuActivity(double muEta, double muPhi, unsigned int method);
	double ElecActivity( double elecEta, double elecPhi, unsigned int method);
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   TTree	*tTagAndProbeMuIso_, *tTagAndProbeMuReco_, *tTagAndProbeElecIso_, *tTagAndProbeElecReco_;
	 //out put variables
	 Int_t Probe_PassingOrFail_;
	 Float_t Probe_InvariantMass_, ProbePt_, ProbeEta_, ProbePhi_, ProbeActivity_;
	 // x-check
	 int eventsFailingGenMatchForTagLeptonIsoMu_, eventsFailingGenMatchForTagLeptonRecoMu_, eventsFailingGenMatchForTagLeptonIsoElec_, eventsFailingGenMatchForTagLeptonRecoElec_;
	 int eventsGenMatchForTagLeptonIsoMu_, eventsGenMatchForTagLeptonRecoMu_, eventsGenMatchForTagLeptonIsoElec_, eventsGenMatchForTagLeptonRecoElec_;
	 
	 // Declaration of leaf types
	 UInt_t          RunNum;
	 UInt_t          LumiBlockNum;
	 UInt_t          EvtNum;
	 Int_t           NVtx;
	 Int_t           isoTracks;
	 Int_t           Leptons;
	 Int_t           LeptonsNoMiniIsolation;
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
	 Float_t         MuIso_minDeltaPhiN;
	 Float_t         MuID_minDeltaPhiN;
	 Float_t         ElecIso_minDeltaPhiN;
	 Float_t         ElecID_minDeltaPhiN;
	 UChar_t         JetID;
	 UShort_t        TagIsoMuonNum;
	 Float_t         TagIsoMuonPt[1000];   //[TagIsoMuonNum]
	 Float_t         TagIsoMuonEta[1000];   //[TagIsoMuonNum]
	 Float_t         TagIsoMuonPhi[1000];   //[TagIsoMuonNum]
	 Float_t         TagIsoMuonE[1000];   //[TagIsoMuonNum]
	 UShort_t        ProbeIsoMuonNum;
	 Float_t         ProbeIsoMuonPt[100];   //[ProbeIsoMuonNum]
	 Float_t         ProbeIsoMuonEta[100];   //[ProbeIsoMuonNum]
	 Float_t         ProbeIsoMuonPhi[100];   //[ProbeIsoMuonNum]
	 Float_t         ProbeIsoMuonE[100];   //[ProbeIsoMuonNum]
	 Float_t         ProbeIsoMuon_InvariantMass[100];   //[ProbeIsoMuonNum]
	 Int_t           ProbeIsoMuon_PassingOrFail[100];   //[ProbeIsoMuonNum]
	 UShort_t        TagIDMuonNum;
	 Float_t         TagIDMuonPt[1000];   //[TagIDMuonNum]
	 Float_t         TagIDMuonEta[1000];   //[TagIDMuonNum]
	 Float_t         TagIDMuonPhi[1000];   //[TagIDMuonNum]
	 Float_t         TagIDMuonE[1000];   //[TagIDMuonNum]
	 UShort_t        ProbeIDMuonNum;
	 Float_t         ProbeIDMuonPt[1000];   //[ProbeIDMuonNum]
	 Float_t         ProbeIDMuonEta[1000];   //[ProbeIDMuonNum]
	 Float_t         ProbeIDMuonPhi[1000];   //[ProbeIDMuonNum]
	 Float_t         ProbeIDMuonE[1000];   //[ProbeIDMuonNum]
	 Float_t         ProbeIDMuon_InvariantMass[1000];   //[ProbeIDMuonNum]
	 Int_t           ProbeIDMuon_PassingOrFail[1000];   //[ProbeIDMuonNum]
	 UShort_t        TagIsoElectronNum;
	 Float_t         TagIsoElectronPt[1000];   //[TagIsoElectronNum]
	 Float_t         TagIsoElectronEta[1000];   //[TagIsoElectronNum]
	 Float_t         TagIsoElectronPhi[1000];   //[TagIsoElectronNum]
	 Float_t         TagIsoElectronE[1000];   //[TagIsoElectronNum]
	 UShort_t        ProbeIsoElectronNum;
	 Float_t         ProbeIsoElectronPt[1000];   //[ProbeIsoElectronNum]
	 Float_t         ProbeIsoElectronEta[1000];   //[ProbeIsoElectronNum]
	 Float_t         ProbeIsoElectronPhi[1000];   //[ProbeIsoElectronNum]
	 Float_t         ProbeIsoElectronE[1000];   //[ProbeIsoElectronNum]
	 Float_t         ProbeIsoElectron_InvariantMass[1000];   //[ProbeIsoElectronNum]
	 Int_t           ProbeIsoElectron_PassingOrFail[1000];   //[ProbeIsoElectronNum]
	 UShort_t        TagIDElectronNum;
	 Float_t         TagIDElectronPt[1000];   //[TagIDElectronNum]
	 Float_t         TagIDElectronEta[1000];   //[TagIDElectronNum]
	 Float_t         TagIDElectronPhi[1000];   //[TagIDElectronNum]
	 Float_t         TagIDElectronE[1000];   //[TagIDElectronNum]
	 UShort_t        ProbeIDElectronNum;
	 Float_t         ProbeIDElectronPt[5];   //[ProbeIDElectronNum]
	 Float_t         ProbeIDElectronEta[5];   //[ProbeIDElectronNum]
	 Float_t         ProbeIDElectronPhi[5];   //[ProbeIDElectronNum]
	 Float_t         ProbeIDElectronE[5];   //[ProbeIDElectronNum]
	 Float_t         ProbeIDElectron_InvariantMass[5];   //[ProbeIDElectronNum]
	 Int_t           ProbeIDElectron_PassingOrFail[5];   //[ProbeIDElectronNum]
	 UShort_t        JetsNum;
	 Float_t         JetsPt[1000];   //[JetsNum]
	 Float_t         JetsEta[1000];   //[JetsNum]
	 Float_t         JetsPhi[1000];   //[JetsNum]
	 Float_t         JetsE[1000];   //[JetsNum]
	 Float_t         Jets_bDiscriminator[1000];   //[JetsNum]
	 Float_t         Jets_chargedEmEnergyFraction[1000];   //[JetsNum]
	 Float_t         Jets_chargedHadronEnergyFraction[1000];   //[JetsNum]
	 Int_t           Jets_chargedHadronMultiplicity[1000];   //[JetsNum]
	 Int_t           Jets_electronMultiplicity[1000];   //[JetsNum]
	 Float_t         Jets_jetArea[1000];   //[JetsNum]
	 Float_t         Jets_muonEnergyFraction[1000];   //[JetsNum]
	 Int_t           Jets_muonMultiplicity[1000];   //[JetsNum]
	 Float_t         Jets_neutralEmEnergyFraction[1000];   //[JetsNum]
	 Int_t           Jets_neutralHadronMultiplicity[1000];   //[JetsNum]
	 Float_t         Jets_photonEnergyFraction[1000];   //[JetsNum]
	 Int_t           Jets_photonMultiplicity[1000];   //[JetsNum]
	 UShort_t        SelectedPFCandidatesNum;
	 Float_t         SelectedPFCandidatesPt[1000];   //[SelectedPFCandidatesNum]
	 Float_t         SelectedPFCandidatesEta[1000];   //[SelectedPFCandidatesNum]
	 Float_t         SelectedPFCandidatesPhi[1000];   //[SelectedPFCandidatesNum]
	 Float_t         SelectedPFCandidatesE[1000];   //[SelectedPFCandidatesNum]
	 Int_t           SelectedPFCandidates_Charge[1000];   //[SelectedPFCandidatesNum]
	 Int_t           SelectedPFCandidates_Typ[1000];   //[SelectedPFCandidatesNum]
	 UShort_t        GenBosonNum;
	 Float_t         GenBosonPt[1000];   //[GenBosonNum]
	 Float_t         GenBosonEta[1000];   //[GenBosonNum]
	 Float_t         GenBosonPhi[1000];   //[GenBosonNum]
	 Float_t         GenBosonE[1000];   //[GenBosonNum]
	 Int_t           GenBoson_GenBosonPDGId[1000];   //[GenBosonNum]
	 UShort_t        GenMuNum;
	 Float_t         GenMuPt[1000];   //[GenMuNum]
	 Float_t         GenMuEta[1000];   //[GenMuNum]
	 Float_t         GenMuPhi[1000];   //[GenMuNum]
	 Float_t         GenMuE[1000];   //[GenMuNum]
	 Int_t           GenMu_GenMuFromTau[1000];   //[GenMuNum]
	 UShort_t        GenElecNum;
	 Float_t         GenElecPt[1000];   //[GenElecNum]
	 Float_t         GenElecEta[1000];   //[GenElecNum]
	 Float_t         GenElecPhi[1000];   //[GenElecNum]
	 Float_t         GenElecE[1000];   //[GenElecNum]
	 Int_t           GenElec_GenElecFromTau[1000];   //[GenElecNum]
	 UShort_t        GenTauNum;
	 Float_t         GenTauPt[1000];   //[GenTauNum]
	 Float_t         GenTauEta[1000];   //[GenTauNum]
	 Float_t         GenTauPhi[1000];   //[GenTauNum]
	 Float_t         GenTauE[1000];   //[GenTauNum]
	 Int_t           GenTau_GenTauHad[1000];   //[GenTauNum]
	 UShort_t        IsolatedTracksNum;
	 Float_t         IsolatedTracksPt[1000];   //[IsolatedTracksNum]
	 Float_t         IsolatedTracksEta[1000];   //[IsolatedTracksNum]
	 Float_t         IsolatedTracksPhi[1000];   //[IsolatedTracksNum]
	 Float_t         IsolatedTracksE[1000];   //[IsolatedTracksNum]
	 UShort_t        selectedIDIsoMuonsNum;
	 Float_t         selectedIDIsoMuonsPt[100];   //[selectedIDIsoMuonsNum]
	 Float_t         selectedIDIsoMuonsEta[100];   //[selectedIDIsoMuonsNum]
	 Float_t         selectedIDIsoMuonsPhi[100];   //[selectedIDIsoMuonsNum]
	 Float_t         selectedIDIsoMuonsE[100];   //[selectedIDIsoMuonsNum]
	 UShort_t        selectedIDMuonsNum;
	 Float_t         selectedIDMuonsPt[1000];   //[selectedIDMuonsNum]
	 Float_t         selectedIDMuonsEta[1000];   //[selectedIDMuonsNum]
	 Float_t         selectedIDMuonsPhi[1000];   //[selectedIDMuonsNum]
	 Float_t         selectedIDMuonsE[1000];   //[selectedIDMuonsNum]
	 UShort_t        selectedIDIsoElectronsNum;
	 Float_t         selectedIDIsoElectronsPt[100];   //[selectedIDIsoElectronsNum]
	 Float_t         selectedIDIsoElectronsEta[100];   //[selectedIDIsoElectronsNum]
	 Float_t         selectedIDIsoElectronsPhi[100];   //[selectedIDIsoElectronsNum]
	 Float_t         selectedIDIsoElectronsE[100];   //[selectedIDIsoElectronsNum]
	 UShort_t        selectedIDElectronsNum;
	 Float_t         selectedIDElectronsPt[1000];   //[selectedIDElectronsNum]
	 Float_t         selectedIDElectronsEta[1000];   //[selectedIDElectronsNum]
	 Float_t         selectedIDElectronsPhi[1000];   //[selectedIDElectronsNum]
	 Float_t         selectedIDElectronsE[1000];   //[selectedIDElectronsNum]
	 UShort_t        SelectedPFCandidates_1Num;
	 Float_t         SelectedPFCandidates_1Pt[1000];   //[SelectedPFCandidates_1Num]
	 Float_t         SelectedPFCandidates_1Eta[1000];   //[SelectedPFCandidates_1Num]
	 Float_t         SelectedPFCandidates_1Phi[1000];   //[SelectedPFCandidates_1Num]
	 Float_t         SelectedPFCandidates_1E[1000];   //[SelectedPFCandidates_1Num]
	 Int_t           SelectedPFCandidates_1_Charge[1000];   //[SelectedPFCandidates_1Num]
	 Int_t           SelectedPFCandidates_1_Typ[1000];   //[SelectedPFCandidates_1Num]
	 UShort_t        MuonsNum;
	 Float_t         MuonsPt[100];   //[MuonsNum]
	 Float_t         MuonsEta[100];   //[MuonsNum]
	 Float_t         MuonsPhi[100];   //[MuonsNum]
	 Float_t         MuonsE[100];   //[MuonsNum]
	 UShort_t        ElectronsNum;
	 Float_t         ElectronsPt[100];   //[ElectronsNum]
	 Float_t         ElectronsEta[100];   //[ElectronsNum]
	 Float_t         ElectronsPhi[100];   //[ElectronsNum]
	 Float_t         ElectronsE[100];   //[ElectronsNum]
	 
	 // List of branches
	 TBranch        *b_RunNum;   //!
	 TBranch        *b_LumiBlockNum;   //!
	 TBranch        *b_EvtNum;   //!
	 TBranch        *b_NVtx;   //!
	 TBranch        *b_isoTracks;   //!
	 TBranch        *b_Leptons;   //!
	 TBranch        *b_LeptonsNoMiniIsolation;   //!
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
	 TBranch        *b_MuIso_minDeltaPhiN;   //!
	 TBranch        *b_MuID_minDeltaPhiN;   //!
	 TBranch        *b_ElecIso_minDeltaPhiN;   //!
	 TBranch        *b_ElecID_minDeltaPhiN;   //!
	 TBranch        *b_JetID;   //!
	 TBranch        *b_TagIsoMuonNum;   //!
	 TBranch        *b_TagIsoMuonPt;   //!
	 TBranch        *b_TagIsoMuonEta;   //!
	 TBranch        *b_TagIsoMuonPhi;   //!
	 TBranch        *b_TagIsoMuonE;   //!
	 TBranch        *b_ProbeIsoMuonNum;   //!
	 TBranch        *b_ProbeIsoMuonPt;   //!
	 TBranch        *b_ProbeIsoMuonEta;   //!
	 TBranch        *b_ProbeIsoMuonPhi;   //!
	 TBranch        *b_ProbeIsoMuonE;   //!
	 TBranch        *b_ProbeIsoMuon_InvariantMass;   //!
	 TBranch        *b_ProbeIsoMuon_PassingOrFail;   //!
	 TBranch        *b_TagIDMuonNum;   //!
	 TBranch        *b_TagIDMuonPt;   //!
	 TBranch        *b_TagIDMuonEta;   //!
	 TBranch        *b_TagIDMuonPhi;   //!
	 TBranch        *b_TagIDMuonE;   //!
	 TBranch        *b_ProbeIDMuonNum;   //!
	 TBranch        *b_ProbeIDMuonPt;   //!
	 TBranch        *b_ProbeIDMuonEta;   //!
	 TBranch        *b_ProbeIDMuonPhi;   //!
	 TBranch        *b_ProbeIDMuonE;   //!
	 TBranch        *b_ProbeIDMuon_InvariantMass;   //!
	 TBranch        *b_ProbeIDMuon_PassingOrFail;   //!
	 TBranch        *b_TagIsoElectronNum;   //!
	 TBranch        *b_TagIsoElectronPt;   //!
	 TBranch        *b_TagIsoElectronEta;   //!
	 TBranch        *b_TagIsoElectronPhi;   //!
	 TBranch        *b_TagIsoElectronE;   //!
	 TBranch        *b_ProbeIsoElectronNum;   //!
	 TBranch        *b_ProbeIsoElectronPt;   //!
	 TBranch        *b_ProbeIsoElectronEta;   //!
	 TBranch        *b_ProbeIsoElectronPhi;   //!
	 TBranch        *b_ProbeIsoElectronE;   //!
	 TBranch        *b_ProbeIsoElectron_InvariantMass;   //!
	 TBranch        *b_ProbeIsoElectron_PassingOrFail;   //!
	 TBranch        *b_TagIDElectronNum;   //!
	 TBranch        *b_TagIDElectronPt;   //!
	 TBranch        *b_TagIDElectronEta;   //!
	 TBranch        *b_TagIDElectronPhi;   //!
	 TBranch        *b_TagIDElectronE;   //!
	 TBranch        *b_ProbeIDElectronNum;   //!
	 TBranch        *b_ProbeIDElectronPt;   //!
	 TBranch        *b_ProbeIDElectronEta;   //!
	 TBranch        *b_ProbeIDElectronPhi;   //!
	 TBranch        *b_ProbeIDElectronE;   //!
	 TBranch        *b_ProbeIDElectron_InvariantMass;   //!
	 TBranch        *b_ProbeIDElectron_PassingOrFail;   //!
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
	 TBranch        *b_IsolatedTracksNum;   //!
	 TBranch        *b_IsolatedTracksPt;   //!
	 TBranch        *b_IsolatedTracksEta;   //!
	 TBranch        *b_IsolatedTracksPhi;   //!
	 TBranch        *b_IsolatedTracksE;   //!
	 TBranch        *b_selectedIDIsoMuonsNum;   //!
	 TBranch        *b_selectedIDIsoMuonsPt;   //!
	 TBranch        *b_selectedIDIsoMuonsEta;   //!
	 TBranch        *b_selectedIDIsoMuonsPhi;   //!
	 TBranch        *b_selectedIDIsoMuonsE;   //!
	 TBranch        *b_selectedIDMuonsNum;   //!
	 TBranch        *b_selectedIDMuonsPt;   //!
	 TBranch        *b_selectedIDMuonsEta;   //!
	 TBranch        *b_selectedIDMuonsPhi;   //!
	 TBranch        *b_selectedIDMuonsE;   //!
	 TBranch        *b_selectedIDIsoElectronsNum;   //!
	 TBranch        *b_selectedIDIsoElectronsPt;   //!
	 TBranch        *b_selectedIDIsoElectronsEta;   //!
	 TBranch        *b_selectedIDIsoElectronsPhi;   //!
	 TBranch        *b_selectedIDIsoElectronsE;   //!
	 TBranch        *b_selectedIDElectronsNum;   //!
	 TBranch        *b_selectedIDElectronsPt;   //!
	 TBranch        *b_selectedIDElectronsEta;   //!
	 TBranch        *b_selectedIDElectronsPhi;   //!
	 TBranch        *b_selectedIDElectronsE;   //!
	 TBranch        *b_SelectedPFCandidates_1Num;   //!
	 TBranch        *b_SelectedPFCandidates_1Pt;   //!
	 TBranch        *b_SelectedPFCandidates_1Eta;   //!
	 TBranch        *b_SelectedPFCandidates_1Phi;   //!
	 TBranch        *b_SelectedPFCandidates_1E;   //!
	 TBranch        *b_SelectedPFCandidates_1_Charge;   //!
	 TBranch        *b_SelectedPFCandidates_1_Typ;   //!
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
	 
	 TagAndProbe(TTree * /*tree*/ =0) : fChain(0) { }
	 virtual ~TagAndProbe() { }
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
	 
	 ClassDef(TagAndProbe,0);
};

#endif

#ifdef TagAndProbe_cxx
void TagAndProbe::Init(TTree *tree)
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
  fChain->SetBranchAddress("isoTracks", &isoTracks, &b_isoTracks);
  fChain->SetBranchAddress("Leptons", &Leptons, &b_Leptons);
  fChain->SetBranchAddress("LeptonsNoMiniIsolation", &LeptonsNoMiniIsolation, &b_LeptonsNoMiniIsolation);
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
  fChain->SetBranchAddress("MuIso_minDeltaPhiN", &MuIso_minDeltaPhiN, &b_MuIso_minDeltaPhiN);
  fChain->SetBranchAddress("MuID_minDeltaPhiN", &MuID_minDeltaPhiN, &b_MuID_minDeltaPhiN);
  fChain->SetBranchAddress("ElecIso_minDeltaPhiN", &ElecIso_minDeltaPhiN, &b_ElecIso_minDeltaPhiN);
  fChain->SetBranchAddress("ElecID_minDeltaPhiN", &ElecID_minDeltaPhiN, &b_ElecID_minDeltaPhiN);
  fChain->SetBranchAddress("JetID", &JetID, &b_JetID);
  fChain->SetBranchAddress("TagIsoMuonNum", &TagIsoMuonNum, &b_TagIsoMuonNum);
  fChain->SetBranchAddress("TagIsoMuonPt", TagIsoMuonPt, &b_TagIsoMuonPt);
  fChain->SetBranchAddress("TagIsoMuonEta", TagIsoMuonEta, &b_TagIsoMuonEta);
  fChain->SetBranchAddress("TagIsoMuonPhi", TagIsoMuonPhi, &b_TagIsoMuonPhi);
  fChain->SetBranchAddress("TagIsoMuonE", TagIsoMuonE, &b_TagIsoMuonE);
  fChain->SetBranchAddress("ProbeIsoMuonNum", &ProbeIsoMuonNum, &b_ProbeIsoMuonNum);
  fChain->SetBranchAddress("ProbeIsoMuonPt", ProbeIsoMuonPt, &b_ProbeIsoMuonPt);
  fChain->SetBranchAddress("ProbeIsoMuonEta", ProbeIsoMuonEta, &b_ProbeIsoMuonEta);
  fChain->SetBranchAddress("ProbeIsoMuonPhi", ProbeIsoMuonPhi, &b_ProbeIsoMuonPhi);
  fChain->SetBranchAddress("ProbeIsoMuonE", ProbeIsoMuonE, &b_ProbeIsoMuonE);
  fChain->SetBranchAddress("ProbeIsoMuon_InvariantMass", ProbeIsoMuon_InvariantMass, &b_ProbeIsoMuon_InvariantMass);
  fChain->SetBranchAddress("ProbeIsoMuon_PassingOrFail", ProbeIsoMuon_PassingOrFail, &b_ProbeIsoMuon_PassingOrFail);
  fChain->SetBranchAddress("TagIDMuonNum", &TagIDMuonNum, &b_TagIDMuonNum);
  fChain->SetBranchAddress("TagIDMuonPt", TagIDMuonPt, &b_TagIDMuonPt);
  fChain->SetBranchAddress("TagIDMuonEta", TagIDMuonEta, &b_TagIDMuonEta);
  fChain->SetBranchAddress("TagIDMuonPhi", TagIDMuonPhi, &b_TagIDMuonPhi);
  fChain->SetBranchAddress("TagIDMuonE", TagIDMuonE, &b_TagIDMuonE);
  fChain->SetBranchAddress("ProbeIDMuonNum", &ProbeIDMuonNum, &b_ProbeIDMuonNum);
  fChain->SetBranchAddress("ProbeIDMuonPt", ProbeIDMuonPt, &b_ProbeIDMuonPt);
  fChain->SetBranchAddress("ProbeIDMuonEta", ProbeIDMuonEta, &b_ProbeIDMuonEta);
  fChain->SetBranchAddress("ProbeIDMuonPhi", ProbeIDMuonPhi, &b_ProbeIDMuonPhi);
  fChain->SetBranchAddress("ProbeIDMuonE", ProbeIDMuonE, &b_ProbeIDMuonE);
  fChain->SetBranchAddress("ProbeIDMuon_InvariantMass", ProbeIDMuon_InvariantMass, &b_ProbeIDMuon_InvariantMass);
  fChain->SetBranchAddress("ProbeIDMuon_PassingOrFail", ProbeIDMuon_PassingOrFail, &b_ProbeIDMuon_PassingOrFail);
  fChain->SetBranchAddress("TagIsoElectronNum", &TagIsoElectronNum, &b_TagIsoElectronNum);
  fChain->SetBranchAddress("TagIsoElectronPt", TagIsoElectronPt, &b_TagIsoElectronPt);
  fChain->SetBranchAddress("TagIsoElectronEta", TagIsoElectronEta, &b_TagIsoElectronEta);
  fChain->SetBranchAddress("TagIsoElectronPhi", TagIsoElectronPhi, &b_TagIsoElectronPhi);
  fChain->SetBranchAddress("TagIsoElectronE", TagIsoElectronE, &b_TagIsoElectronE);
  fChain->SetBranchAddress("ProbeIsoElectronNum", &ProbeIsoElectronNum, &b_ProbeIsoElectronNum);
  fChain->SetBranchAddress("ProbeIsoElectronPt", ProbeIsoElectronPt, &b_ProbeIsoElectronPt);
  fChain->SetBranchAddress("ProbeIsoElectronEta", ProbeIsoElectronEta, &b_ProbeIsoElectronEta);
  fChain->SetBranchAddress("ProbeIsoElectronPhi", ProbeIsoElectronPhi, &b_ProbeIsoElectronPhi);
  fChain->SetBranchAddress("ProbeIsoElectronE", ProbeIsoElectronE, &b_ProbeIsoElectronE);
  fChain->SetBranchAddress("ProbeIsoElectron_InvariantMass", ProbeIsoElectron_InvariantMass, &b_ProbeIsoElectron_InvariantMass);
  fChain->SetBranchAddress("ProbeIsoElectron_PassingOrFail", ProbeIsoElectron_PassingOrFail, &b_ProbeIsoElectron_PassingOrFail);
  fChain->SetBranchAddress("TagIDElectronNum", &TagIDElectronNum, &b_TagIDElectronNum);
  fChain->SetBranchAddress("TagIDElectronPt", TagIDElectronPt, &b_TagIDElectronPt);
  fChain->SetBranchAddress("TagIDElectronEta", TagIDElectronEta, &b_TagIDElectronEta);
  fChain->SetBranchAddress("TagIDElectronPhi", TagIDElectronPhi, &b_TagIDElectronPhi);
  fChain->SetBranchAddress("TagIDElectronE", TagIDElectronE, &b_TagIDElectronE);
  fChain->SetBranchAddress("ProbeIDElectronNum", &ProbeIDElectronNum, &b_ProbeIDElectronNum);
  fChain->SetBranchAddress("ProbeIDElectronPt", ProbeIDElectronPt, &b_ProbeIDElectronPt);
  fChain->SetBranchAddress("ProbeIDElectronEta", ProbeIDElectronEta, &b_ProbeIDElectronEta);
  fChain->SetBranchAddress("ProbeIDElectronPhi", ProbeIDElectronPhi, &b_ProbeIDElectronPhi);
  fChain->SetBranchAddress("ProbeIDElectronE", ProbeIDElectronE, &b_ProbeIDElectronE);
  fChain->SetBranchAddress("ProbeIDElectron_InvariantMass", ProbeIDElectron_InvariantMass, &b_ProbeIDElectron_InvariantMass);
  fChain->SetBranchAddress("ProbeIDElectron_PassingOrFail", ProbeIDElectron_PassingOrFail, &b_ProbeIDElectron_PassingOrFail);
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
  fChain->SetBranchAddress("IsolatedTracksNum", &IsolatedTracksNum, &b_IsolatedTracksNum);
  fChain->SetBranchAddress("IsolatedTracksPt", IsolatedTracksPt, &b_IsolatedTracksPt);
  fChain->SetBranchAddress("IsolatedTracksEta", IsolatedTracksEta, &b_IsolatedTracksEta);
  fChain->SetBranchAddress("IsolatedTracksPhi", IsolatedTracksPhi, &b_IsolatedTracksPhi);
  fChain->SetBranchAddress("IsolatedTracksE", IsolatedTracksE, &b_IsolatedTracksE);
  fChain->SetBranchAddress("selectedIDIsoMuonsNum", &selectedIDIsoMuonsNum, &b_selectedIDIsoMuonsNum);
  fChain->SetBranchAddress("selectedIDIsoMuonsPt", selectedIDIsoMuonsPt, &b_selectedIDIsoMuonsPt);
  fChain->SetBranchAddress("selectedIDIsoMuonsEta", selectedIDIsoMuonsEta, &b_selectedIDIsoMuonsEta);
  fChain->SetBranchAddress("selectedIDIsoMuonsPhi", selectedIDIsoMuonsPhi, &b_selectedIDIsoMuonsPhi);
  fChain->SetBranchAddress("selectedIDIsoMuonsE", selectedIDIsoMuonsE, &b_selectedIDIsoMuonsE);
  fChain->SetBranchAddress("selectedIDMuonsNum", &selectedIDMuonsNum, &b_selectedIDMuonsNum);
  fChain->SetBranchAddress("selectedIDMuonsPt", selectedIDMuonsPt, &b_selectedIDMuonsPt);
  fChain->SetBranchAddress("selectedIDMuonsEta", selectedIDMuonsEta, &b_selectedIDMuonsEta);
  fChain->SetBranchAddress("selectedIDMuonsPhi", selectedIDMuonsPhi, &b_selectedIDMuonsPhi);
  fChain->SetBranchAddress("selectedIDMuonsE", selectedIDMuonsE, &b_selectedIDMuonsE);
  fChain->SetBranchAddress("selectedIDIsoElectronsNum", &selectedIDIsoElectronsNum, &b_selectedIDIsoElectronsNum);
  fChain->SetBranchAddress("selectedIDIsoElectronsPt", selectedIDIsoElectronsPt, &b_selectedIDIsoElectronsPt);
  fChain->SetBranchAddress("selectedIDIsoElectronsEta", selectedIDIsoElectronsEta, &b_selectedIDIsoElectronsEta);
  fChain->SetBranchAddress("selectedIDIsoElectronsPhi", selectedIDIsoElectronsPhi, &b_selectedIDIsoElectronsPhi);
  fChain->SetBranchAddress("selectedIDIsoElectronsE", selectedIDIsoElectronsE, &b_selectedIDIsoElectronsE);
  fChain->SetBranchAddress("selectedIDElectronsNum", &selectedIDElectronsNum, &b_selectedIDElectronsNum);
  fChain->SetBranchAddress("selectedIDElectronsPt", selectedIDElectronsPt, &b_selectedIDElectronsPt);
  fChain->SetBranchAddress("selectedIDElectronsEta", selectedIDElectronsEta, &b_selectedIDElectronsEta);
  fChain->SetBranchAddress("selectedIDElectronsPhi", selectedIDElectronsPhi, &b_selectedIDElectronsPhi);
  fChain->SetBranchAddress("selectedIDElectronsE", selectedIDElectronsE, &b_selectedIDElectronsE);
  fChain->SetBranchAddress("SelectedPFCandidates_1Num", &SelectedPFCandidates_1Num, &b_SelectedPFCandidates_1Num);
  fChain->SetBranchAddress("SelectedPFCandidates_1Pt", SelectedPFCandidates_1Pt, &b_SelectedPFCandidates_1Pt);
  fChain->SetBranchAddress("SelectedPFCandidates_1Eta", SelectedPFCandidates_1Eta, &b_SelectedPFCandidates_1Eta);
  fChain->SetBranchAddress("SelectedPFCandidates_1Phi", SelectedPFCandidates_1Phi, &b_SelectedPFCandidates_1Phi);
  fChain->SetBranchAddress("SelectedPFCandidates_1E", SelectedPFCandidates_1E, &b_SelectedPFCandidates_1E);
  fChain->SetBranchAddress("SelectedPFCandidates_1_Charge", SelectedPFCandidates_1_Charge, &b_SelectedPFCandidates_1_Charge);
  fChain->SetBranchAddress("SelectedPFCandidates_1_Typ", SelectedPFCandidates_1_Typ, &b_SelectedPFCandidates_1_Typ);
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
}

Bool_t TagAndProbe::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

#endif // #ifdef TagAndProbe_cxx
