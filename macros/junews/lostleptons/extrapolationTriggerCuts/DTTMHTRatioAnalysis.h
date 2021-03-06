//////////////////////////////////////////////////////////
// This class has been automatically generated on
// Wed Jun 24 13:19:12 2015 by ROOT version 5.34/25
// from TTree PreSelection/PreSelection
// found on file: ../../../../reduced_trees/13TeV/ttbar_arne/ReducedSelection_1.root
//////////////////////////////////////////////////////////

#ifndef DTTMHTRatioAnalysis_h
#define DTTMHTRatioAnalysis_h

#include <TROOT.h>
#include <TChain.h>
#include <TFile.h>

// Header file for the classes stored in the TTree if any.

// Fixed size dimensions of array or collections stored in the TTree if any.

class DTTMHTRatioAnalysis {
public :
   TTree          *fChain;   //!pointer to the analyzed TTree or TChain
   Int_t           fCurrent; //!current Tree number in a TChain

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
   Float_t         MHTPhi;
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
   Float_t         MuonsPt[4];   //[MuonsNum]
   Float_t         MuonsEta[4];   //[MuonsNum]
   Float_t         MuonsPhi[4];   //[MuonsNum]
   Float_t         MuonsE[4];   //[MuonsNum]
   UShort_t        ElectronsNum;
   Float_t         ElectronsPt[3];   //[ElectronsNum]
   Float_t         ElectronsEta[3];   //[ElectronsNum]
   Float_t         ElectronsPhi[3];   //[ElectronsNum]
   Float_t         ElectronsE[3];   //[ElectronsNum]
   UShort_t        IsolatedElectronTracksVetoNum;
   Float_t         IsolatedElectronTracksVetoPt[3];   //[IsolatedElectronTracksVetoNum]
   Float_t         IsolatedElectronTracksVetoEta[3];   //[IsolatedElectronTracksVetoNum]
   Float_t         IsolatedElectronTracksVetoPhi[3];   //[IsolatedElectronTracksVetoNum]
   Float_t         IsolatedElectronTracksVetoE[3];   //[IsolatedElectronTracksVetoNum]
   UShort_t        IsolatedMuonTracksVetoNum;
   Float_t         IsolatedMuonTracksVetoPt[3];   //[IsolatedMuonTracksVetoNum]
   Float_t         IsolatedMuonTracksVetoEta[3];   //[IsolatedMuonTracksVetoNum]
   Float_t         IsolatedMuonTracksVetoPhi[3];   //[IsolatedMuonTracksVetoNum]
   Float_t         IsolatedMuonTracksVetoE[3];   //[IsolatedMuonTracksVetoNum]
   UShort_t        IsolatedPionTracksVetoNum;
   Float_t         IsolatedPionTracksVetoPt[3];   //[IsolatedPionTracksVetoNum]
   Float_t         IsolatedPionTracksVetoEta[3];   //[IsolatedPionTracksVetoNum]
   Float_t         IsolatedPionTracksVetoPhi[3];   //[IsolatedPionTracksVetoNum]
   Float_t         IsolatedPionTracksVetoE[3];   //[IsolatedPionTracksVetoNum]
   UShort_t        selectedIDIsoMuonsNum;
   Float_t         selectedIDIsoMuonsPt[4];   //[selectedIDIsoMuonsNum]
   Float_t         selectedIDIsoMuonsEta[4];   //[selectedIDIsoMuonsNum]
   Float_t         selectedIDIsoMuonsPhi[4];   //[selectedIDIsoMuonsNum]
   Float_t         selectedIDIsoMuonsE[4];   //[selectedIDIsoMuonsNum]
   Float_t         selectedIDIsoMuons_MTW[4];   //[selectedIDIsoMuonsNum]
   UShort_t        selectedIDMuonsNum;
   Float_t         selectedIDMuonsPt[4];   //[selectedIDMuonsNum]
   Float_t         selectedIDMuonsEta[4];   //[selectedIDMuonsNum]
   Float_t         selectedIDMuonsPhi[4];   //[selectedIDMuonsNum]
   Float_t         selectedIDMuonsE[4];   //[selectedIDMuonsNum]
   Float_t         selectedIDMuons_MTW[4];   //[selectedIDMuonsNum]
   UShort_t        selectedIDIsoElectronsNum;
   Float_t         selectedIDIsoElectronsPt[3];   //[selectedIDIsoElectronsNum]
   Float_t         selectedIDIsoElectronsEta[3];   //[selectedIDIsoElectronsNum]
   Float_t         selectedIDIsoElectronsPhi[3];   //[selectedIDIsoElectronsNum]
   Float_t         selectedIDIsoElectronsE[3];   //[selectedIDIsoElectronsNum]
   Float_t         selectedIDIsoElectrons_MTW[3];   //[selectedIDIsoElectronsNum]
   UShort_t        selectedIDElectronsNum;
   Float_t         selectedIDElectronsPt[4];   //[selectedIDElectronsNum]
   Float_t         selectedIDElectronsEta[4];   //[selectedIDElectronsNum]
   Float_t         selectedIDElectronsPhi[4];   //[selectedIDElectronsNum]
   Float_t         selectedIDElectronsE[4];   //[selectedIDElectronsNum]
   Float_t         selectedIDElectrons_MTW[4];   //[selectedIDElectronsNum]
   UShort_t        SelectedPFCandidatesNum;
   Float_t         SelectedPFCandidatesPt[115];   //[SelectedPFCandidatesNum]
   Float_t         SelectedPFCandidatesEta[115];   //[SelectedPFCandidatesNum]
   Float_t         SelectedPFCandidatesPhi[115];   //[SelectedPFCandidatesNum]
   Float_t         SelectedPFCandidatesE[115];   //[SelectedPFCandidatesNum]
   Int_t           SelectedPFCandidates_Charge[115];   //[SelectedPFCandidatesNum]
   Int_t           SelectedPFCandidates_Typ[115];   //[SelectedPFCandidatesNum]
   UShort_t        GenBosonNum;
   Float_t         GenBosonPt[2];   //[GenBosonNum]
   Float_t         GenBosonEta[2];   //[GenBosonNum]
   Float_t         GenBosonPhi[2];   //[GenBosonNum]
   Float_t         GenBosonE[2];   //[GenBosonNum]
   Int_t           GenBoson_GenBosonPDGId[2];   //[GenBosonNum]
   UShort_t        GenMuNum;
   Float_t         GenMuPt[2];   //[GenMuNum]
   Float_t         GenMuEta[2];   //[GenMuNum]
   Float_t         GenMuPhi[2];   //[GenMuNum]
   Float_t         GenMuE[2];   //[GenMuNum]
   Int_t           GenMu_GenMuFromTau[2];   //[GenMuNum]
   UShort_t        GenElecNum;
   Float_t         GenElecPt[2];   //[GenElecNum]
   Float_t         GenElecEta[2];   //[GenElecNum]
   Float_t         GenElecPhi[2];   //[GenElecNum]
   Float_t         GenElecE[2];   //[GenElecNum]
   Int_t           GenElec_GenElecFromTau[2];   //[GenElecNum]
   UShort_t        GenTauNum;
   Float_t         GenTauPt[2];   //[GenTauNum]
   Float_t         GenTauEta[2];   //[GenTauNum]
   Float_t         GenTauPhi[2];   //[GenTauNum]
   Float_t         GenTauE[2];   //[GenTauNum]
   Int_t           GenTau_GenTauHad[2];   //[GenTauNum]
   UShort_t        TauDecayCandsNum;
   Float_t         TauDecayCandsPt[6];   //[TauDecayCandsNum]
   Float_t         TauDecayCandsEta[6];   //[TauDecayCandsNum]
   Float_t         TauDecayCandsPhi[6];   //[TauDecayCandsNum]
   Float_t         TauDecayCandsE[6];   //[TauDecayCandsNum]
   Int_t           TauDecayCands_pdgID[6];   //[TauDecayCandsNum]
   UShort_t        selectedIDIsoMuonsNoMiniIsoNum;
   Float_t         selectedIDIsoMuonsNoMiniIsoPt[4];   //[selectedIDIsoMuonsNoMiniIsoNum]
   Float_t         selectedIDIsoMuonsNoMiniIsoEta[4];   //[selectedIDIsoMuonsNoMiniIsoNum]
   Float_t         selectedIDIsoMuonsNoMiniIsoPhi[4];   //[selectedIDIsoMuonsNoMiniIsoNum]
   Float_t         selectedIDIsoMuonsNoMiniIsoE[4];   //[selectedIDIsoMuonsNoMiniIsoNum]
   UShort_t        selectedIDIsoElectronsNoMiniIsoNum;
   Float_t         selectedIDIsoElectronsNoMiniIsoPt[3];   //[selectedIDIsoElectronsNoMiniIsoNum]
   Float_t         selectedIDIsoElectronsNoMiniIsoEta[3];   //[selectedIDIsoElectronsNoMiniIsoNum]
   Float_t         selectedIDIsoElectronsNoMiniIsoPhi[3];   //[selectedIDIsoElectronsNoMiniIsoNum]
   Float_t         selectedIDIsoElectronsNoMiniIsoE[3];   //[selectedIDIsoElectronsNoMiniIsoNum]
   UShort_t        JetsNum;
   Float_t         JetsPt[13];   //[JetsNum]
   Float_t         JetsEta[13];   //[JetsNum]
   Float_t         JetsPhi[13];   //[JetsNum]
   Float_t         JetsE[13];   //[JetsNum]
   Float_t         Jets_bDiscriminator[13];   //[JetsNum]
   Float_t         Jets_chargedEmEnergyFraction[13];   //[JetsNum]
   Float_t         Jets_chargedHadronEnergyFraction[13];   //[JetsNum]
   Int_t           Jets_chargedHadronMultiplicity[13];   //[JetsNum]
   Int_t           Jets_electronMultiplicity[13];   //[JetsNum]
   Float_t         Jets_jetArea[13];   //[JetsNum]
   Float_t         Jets_muonEnergyFraction[13];   //[JetsNum]
   Int_t           Jets_muonMultiplicity[13];   //[JetsNum]
   Float_t         Jets_neutralEmEnergyFraction[13];   //[JetsNum]
   Int_t           Jets_neutralHadronMultiplicity[13];   //[JetsNum]
   Float_t         Jets_photonEnergyFraction[13];   //[JetsNum]
   Int_t           Jets_photonMultiplicity[13];   //[JetsNum]
   UShort_t        slimmedElectronsNum;
   Float_t         slimmedElectronsPt[9];   //[slimmedElectronsNum]
   Float_t         slimmedElectronsEta[9];   //[slimmedElectronsNum]
   Float_t         slimmedElectronsPhi[9];   //[slimmedElectronsNum]
   Float_t         slimmedElectronsE[9];   //[slimmedElectronsNum]
   UShort_t        slimmedMuonsNum;
   Float_t         slimmedMuonsPt[16];   //[slimmedMuonsNum]
   Float_t         slimmedMuonsEta[16];   //[slimmedMuonsNum]
   Float_t         slimmedMuonsPhi[16];   //[slimmedMuonsNum]
   Float_t         slimmedMuonsE[16];   //[slimmedMuonsNum]

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
   TBranch        *b_MHTPhi;   //!
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
   TBranch        *b_IsolatedMuonTracksVetoNum;   //!
   TBranch        *b_IsolatedMuonTracksVetoPt;   //!
   TBranch        *b_IsolatedMuonTracksVetoEta;   //!
   TBranch        *b_IsolatedMuonTracksVetoPhi;   //!
   TBranch        *b_IsolatedMuonTracksVetoE;   //!
   TBranch        *b_IsolatedPionTracksVetoNum;   //!
   TBranch        *b_IsolatedPionTracksVetoPt;   //!
   TBranch        *b_IsolatedPionTracksVetoEta;   //!
   TBranch        *b_IsolatedPionTracksVetoPhi;   //!
   TBranch        *b_IsolatedPionTracksVetoE;   //!
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

   DTTMHTRatioAnalysis(TTree *tree=0);
   virtual ~DTTMHTRatioAnalysis();
   virtual Int_t    Cut(Long64_t entry);
   virtual Int_t    GetEntry(Long64_t entry);
   virtual Long64_t LoadTree(Long64_t entry);
   virtual void     Init(TTree *tree);
   virtual void     Run(TString outputtag="default", bool verb=false);
   virtual Bool_t   Notify();
   virtual void     Show(Long64_t entry = -1);
};

#endif

#ifdef DTTMHTRatioAnalysis_cxx
DTTMHTRatioAnalysis::DTTMHTRatioAnalysis(TTree *tree) : fChain(0) 
{
// if parameter tree is not specified (or zero), connect the file
// used to generate this class and read the Tree.
   if (tree == 0) {
      TFile *f = (TFile*)gROOT->GetListOfFiles()->FindObject("../../../../reduced_trees/13TeV/ttbar_arne/ReducedSelection_1.root");
      if (!f || !f->IsOpen()) {
         f = new TFile("../../../../reduced_trees/13TeV/ttbar_arne/ReducedSelection_1.root");
      }
      TDirectory * dir = (TDirectory*)f->Get("../../../../reduced_trees/13TeV/ttbar_arne/ReducedSelection_1.root:/TreeMaker2");
      dir->GetObject("PreSelection",tree);

   }
   Init(tree);
}

DTTMHTRatioAnalysis::~DTTMHTRatioAnalysis()
{
   if (!fChain) return;
   delete fChain->GetCurrentFile();
}

Int_t DTTMHTRatioAnalysis::GetEntry(Long64_t entry)
{
// Read contents of entry.
   if (!fChain) return 0;
   return fChain->GetEntry(entry);
}
Long64_t DTTMHTRatioAnalysis::LoadTree(Long64_t entry)
{
// Set the environment to read one entry
   if (!fChain) return -5;
   Long64_t centry = fChain->LoadTree(entry);
   if (centry < 0) return centry;
   if (fChain->GetTreeNumber() != fCurrent) {
      fCurrent = fChain->GetTreeNumber();
      Notify();
   }
   return centry;
}

void DTTMHTRatioAnalysis::Init(TTree *tree)
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
   fCurrent = -1;
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
   fChain->SetBranchAddress("MHTPhi", &MHTPhi, &b_MHTPhi);
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
   fChain->SetBranchAddress("IsolatedMuonTracksVetoNum", &IsolatedMuonTracksVetoNum, &b_IsolatedMuonTracksVetoNum);
   fChain->SetBranchAddress("IsolatedMuonTracksVetoPt", IsolatedMuonTracksVetoPt, &b_IsolatedMuonTracksVetoPt);
   fChain->SetBranchAddress("IsolatedMuonTracksVetoEta", IsolatedMuonTracksVetoEta, &b_IsolatedMuonTracksVetoEta);
   fChain->SetBranchAddress("IsolatedMuonTracksVetoPhi", IsolatedMuonTracksVetoPhi, &b_IsolatedMuonTracksVetoPhi);
   fChain->SetBranchAddress("IsolatedMuonTracksVetoE", IsolatedMuonTracksVetoE, &b_IsolatedMuonTracksVetoE);
   fChain->SetBranchAddress("IsolatedPionTracksVetoNum", &IsolatedPionTracksVetoNum, &b_IsolatedPionTracksVetoNum);
   fChain->SetBranchAddress("IsolatedPionTracksVetoPt", IsolatedPionTracksVetoPt, &b_IsolatedPionTracksVetoPt);
   fChain->SetBranchAddress("IsolatedPionTracksVetoEta", IsolatedPionTracksVetoEta, &b_IsolatedPionTracksVetoEta);
   fChain->SetBranchAddress("IsolatedPionTracksVetoPhi", IsolatedPionTracksVetoPhi, &b_IsolatedPionTracksVetoPhi);
   fChain->SetBranchAddress("IsolatedPionTracksVetoE", IsolatedPionTracksVetoE, &b_IsolatedPionTracksVetoE);
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

   Notify();
}

Bool_t DTTMHTRatioAnalysis::Notify()
{
   // The Notify() function is called when a new file is opened. This
   // can be either for a new TTree in a TChain or when when a new TTree
   // is started when using PROOF. It is normally not necessary to make changes
   // to the generated code, but the routine can be extended by the
   // user if needed. The return value is currently not used.

   return kTRUE;
}

void DTTMHTRatioAnalysis::Show(Long64_t entry)
{
// Print contents of entry.
// If entry is not specified, print current entry
   if (!fChain) return;
   fChain->Show(entry);
}
Int_t DTTMHTRatioAnalysis::Cut(Long64_t entry)
{
// This function may be called from Loop.
// returns  1 if entry is accepted.
// returns -1 otherwise.
   return 1;
}
#endif // #ifdef DTTMHTRatioAnalysis_cxx
