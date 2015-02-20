#ifndef H_EVENTHANDLER
#define H_EVENTHANDLER

#include <cfloat>
#include <vector>
#include <string>
#include <utility>
#include <limits>
#include <stdint.h>
#include "TChain.h"
#include "TBranch.h"
#include "TLorentzVector.h"
#include "pu_constants.hpp"
#include "lumi_reweighting_stand_alone.hpp"
#include "cfa2014.hpp"
#include "b_jet.hpp"
#include "fat_jet.hpp"
#include "event_number.hpp"
#include "gen_muon.hpp"
#include "gen_electron.hpp"
#include "gen_tau.hpp"
#include "utils.hpp"

typedef unsigned int uint;
using std::vector;
using std::cout;
using std::endl;

namespace particleId {
  enum leptonType { 
    X=0,
    electron=2,
    muon=3,
    electronVeto=4,
    muonVeto=5
  };
}

struct mc_particle{
  mc_particle(const TLorentzVector &momentum,
              float charge,
              int id, int mom, int gmom, int ggmom,
              int status):
    momentum_(momentum),
    charge_(TMath::Nint(3.*charge)),
    id_(id),
    mom_(mom),
    gmom_(gmom),
    ggmom_(ggmom),
    status_(status){
  }

  TLorentzVector momentum_;
  float charge_;
  int id_, mom_, gmom_, ggmom_;
  int status_;
};

// enum lepton{electron=11, muon=13, tau=15};

class EventHandler : public cfA{
public:
  EventHandler(const std::string &, const bool, const double=1.0, const bool=false);

  void SetScaleFactor(const double);
  void SetScaleFactor(const double, const double, const int);

protected:
  mutable std::vector<BJet> sortedBJetCache;//caching for efficiency
  mutable bool bJetsUpToDate;//cached value correct
  mutable std::vector<FatJet> sortedFatJetCache;//caching for efficiency
  mutable bool FatJetsUpToDate;//cached value correct
  mutable std::vector<int> recoMuonCache;
  mutable bool recoMuonsUpToDate;
  mutable std::vector<int> recoElectronCache;
  mutable bool recoElectronsUpToDate; 
  mutable std::vector<int>recoTauCache;
  mutable bool recoTausUpToDate;
  mutable bool betaUpToDate;
  static const double CSVTCut, CSVMCut, CSVLCut;
  static const double ICSVTCut, ICSVMCut, ICSVLCut;
  double scaleFactor;
  
  mutable std::vector<double> beta;


  //stuff for the btag probability
  TFile *f_tageff_;
  TString AssembleBTagEffFilename();
  void LoadJetTagEffMaps();

  int GetcfAVersion() const;
  unsigned TypeCode() const;
  int GetMass(const std::string&) const;
  int GetGluinoMass() const;
  int GetLSPMass() const;

  double GetTriggerEffWeight(const double=50.) const;
  double GetBtagWeight(const double=50.) const;
  double GetPIDBtagWeight(const double=50.) const;
  double GetBtagSF(const int, const double, const double, const int=2) const;
  double GetBtagEffMC(const int, const double) const;
  

  void CalculateTagProbs(double &Prob0, double &ProbGEQ1, double &Prob1, double &ProbGEQ2,
			 double &Prob2, double &ProbGEQ3, double &Prob3, double &ProbGEQ4, double pt_cut=40.);
  double GetJetTagEff(unsigned int ijet, TH1D* h_btageff, TH1D* h_ctageff, TH1D* h_ltageff, double pt_cut=40.);
  double GetbJetFastsimSF(const TString & what, const int flavor, const double pt) const;
  int GetPtBinIndex(const float pt) const;
  double Get_AN_12_175_Table2_Value(const double pt) const;
  double Get_AN_12_175_Table10_Value(const double pt) const;
  double Get_AN_12_175_Table18_Value(const double pt) const;

  void ClusterFatJets() const;
  void GetSortedFatJets() const;
  double GetHighestFatJetmJ(const unsigned int=1) const;
  void GetSortedBJets() const;
  double GetHighestJetPt(const unsigned int=1) const;
  double GetHighestJetCSV(const unsigned int=1) const;
  int GetJetXIndex(const unsigned int=1) const;
  double GetJetXEta(const unsigned int=1) const;
  double GetJetXPhi(const unsigned int=1) const;
  double GetJetXCSV(const unsigned int=1) const;
  double GetJetXPartonId(const unsigned int=1) const;
  double GetJetXMotherId(const unsigned int=1) const;

  void GetEntry(const unsigned int);
  void GetBeta(const std::string which="beta") const;

  bool PassesJSONCut() const;

  bool PassesSpecificTrigger(const std::string) const;
  bool Passes2012RA2bTrigger() const;
  bool Passes2012METCleaningCut() const;
  bool PassesCSA14METCleaningCut() const;
  bool isProblemJet(const unsigned int) const;
  bool PassesBadJetFilter() const;
  int GetPBNR() const;

  bool isInMuonCollection(const double, const double) const;
  bool isInElectronCollection(const double, const double) const;
  bool isInTauCollection(const double, const double) const;

  double GetTrueNumInteractions() const;
  double GetNumInteractions() const;
  unsigned short GetNumVertices() const;
  double GetPUWeight(reweight::LumiReWeighting &) const;
  bool isGoodVertex(const unsigned int) const;
  double GetEarlyOutOfTimePU(const int=1) const;
  double GetLateOutOfTimePU() const;

  bool PassesPVCut() const;

  bool isGoodJet(const unsigned int, const bool=true, const double=50.0,
                 const double=2.4/*, const bool=true*/) const;
  bool isCleanJet(const unsigned int, const int) const;
  bool jetPassLooseID(const unsigned int) const;
  bool jetHasEMu(const int) const;

  bool isRA2bElectron(const unsigned int, const unsigned short=0, const bool=true) const;
  bool isRA2bMuon(const unsigned int, const unsigned short=0, const bool=true) const;
  bool isRA2bTau(const unsigned int, const unsigned short=0, const bool=true) const;

  double GetRA2bMuonRelIso(const unsigned int) const;

  int GetNumRA2bElectrons(const unsigned short=0, const bool=true) const;
  int GetNumRA2bMuons(const unsigned short=0, const bool=true) const;
  int GetNumRA2bTaus(const unsigned short=0, const bool=true) const;

  bool isIsoTrack(const unsigned int, const double=10.0) const;
  bool isQualityTrack(const unsigned int) const;

  int NewGetNumIsoTracks(const double=10.0) const;

  double GetRA2bElectronRelIso(const unsigned int) const;
  double GetElectronRelIso(const unsigned int) const;

  double GetTopPtWeight() const;
  double GetTopPt() const;
  double GetTopPtWeightOfficial() const;

  int GetNGenPartons(const float=0.) const;
  int GetNGenParticles(const int, const float=0., const bool=true) const;
  int GetGenParticleIndex(const int, const int=0) const;

  int GetJetGenId(const int) const;

  bool isTrueElectron(const double eta, const double phi) const;
 
  float GetCorrespondingTopPt(const int) const;
  float GetCorrespondingDeltaRWb(const int) const;
  float GetCorrespondingDeltaRlb(const int) const;

  bool hasPFMatch(const int index, const int pdgId) const;
  bool IsFromB(const int, const int, const int) const;

  TLorentzVector GetNearestJet(const TLorentzVector lepton, const uint jet_ind) const; 

  vector<int> GetRecoMuons(const bool veto, const bool check_pt=true, const bool check_iso=true, const bool use_mini_iso=false, const double mini_iso_cut=0.1) const;
  bool PassMuonID(const uint imu) const;
  bool isRecoMuon(const uint imu, const uint level=0, const bool=true, const bool=true, const bool=false, const double mini_iso_cut=0.1) const;
  double GetMuonD0(const unsigned int imu) const;
  bool isTrueMuon(const double eta, const double phi) const;
  bool isbMuon(const double eta, const double phi) const;
  double GetMuonRelIso(const unsigned int imu, const double R=0.4) const;
  bool passedRecoMuonSelection(uint imu, float MuonPTThreshold=20., float MuonETAThreshold=2.4);
  bool passedRecoMuonVeto(uint imu, float MuonPTThreshold=15., float MuonETAThreshold=2.5);
  double getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex);
  bool passedBaseMuonSelection(uint imu, float MuonPTThreshold=0., float MuonETAThreshold=5.);
  float GetRecoMuonIsolation(uint imu);

  vector<int> GetRecoElectrons(const bool veto, const bool check_pt=true, const bool check_iso=true, const bool use_mini_iso=false, const double mini_iso_cut=0.1) const;
  bool PassElectronID(const uint iel, const uint level) const;
  bool isRecoElectron(const uint iel, const uint level=0, const bool=true, const bool=true, const bool=false, const double mini_iso_cut=0.1) const;
  double GetElectronD0(const unsigned int iel) const;
  
  vector<int> GetRA2bElectrons(const bool veto) const;
  vector<int> GetRA2bMuons(const bool veto) const;

  vector<int> GetRecoTaus() const;

  bool passedRA4ElectronSelection(uint iel, float ElectronPTThreshold=20.);
  bool passedRA4ElectronVeto(uint iel, float ElectronPTThreshold=15.);
  bool passedBaseElectronSelection(uint iel, float ElectronPTThreshold=20.);
  float GetRA4ElectronIsolation(uint iel);
  float GetEffectiveArea(float SCEta, bool isMC);

  // vector<int> GetCSAElectrons(bool veto);
  bool passedCSAElectronSelection(uint iel, float ElectronPTThreshold=20.);
  bool passedCSAElectronVeto(uint iel, float ElectronPTThreshold=15.);
  float GetCSAElectronIsolation(const uint iel) const;
  bool passedCSABaseElectronSelection(uint iel, float ElectronPTThreshold=20.);

 
  bool IsMC();

  double GetFatJetPt(const unsigned int/*,  const unsigned int=30*/) const;
  double GetFatJetEta(const unsigned int/*,  const unsigned int=30*/) const;
  double GetFatJetPhi(const unsigned int/*,  const unsigned int=30*/) const;
  double GetFatJetEnergy(const unsigned int/*,  const unsigned int=30*/) const;
  int GetFatJetnConst(const unsigned int/*,  const unsigned int=30*/) const;
  double GetFatJetmJ(const unsigned int/*,  const unsigned int=30*/) const;
  int GetNFatJets(const double=50., const double=10./*,  const unsigned int=30*/) const;
  double GetMJ(const double=50., const double=10./*,  const unsigned int=30*/) const;

  double GetHT(const double=50.) const;
  double GetSumP(const double=50.) const;
  double GetCentrality(const double=50.) const;
  unsigned int GetNumGoodJets(const double pt=50.) const;
  unsigned int GetNumTruthMatchedBJets(const double pt=0., bool=false) const;
  unsigned int GetNumCSVTJets(const double=50.) const;
  unsigned int GetNumCSVMJets(const double=50.) const;
  unsigned int GetNumCSVLJets(const double=50.) const;
  unsigned int GetNumIncCSVTJets(const double=50.) const;
  unsigned int GetNumIncCSVMJets(const double=50.) const;
  unsigned int GetNumIncCSVLJets(const double=50.) const;

  double GetMTW(const double, const double, const double, const double) const;
  double GetMTWb(const double, const double, const double, const double, const bool=false) const;
  double GetMinMTWb(const double=30., const double=0.679, const bool=false) const;
  double Get2ndMTWb(const double=30., const double=0.679, const bool=false) const;
  double GetMinDeltaPhibMET(const double=30., const double=0.679) const;

  double getMinDeltaPhiMETN(unsigned int maxjets, float mainpt, float maineta, bool mainid, float otherpt, float othereta, bool otherid, bool useArcsin=true );
  double getMinDeltaPhiMETN(unsigned int maxjets) {return getMinDeltaPhiMETN(maxjets,50.,2.4,true,30.,2.4,true,true); };
  double getDeltaPhiMETN(  int goodJetI, float otherpt, float othereta, bool otherid, bool useArcsin ); //Ben
  double getDeltaPhiMETN( int goodJetI ) {return getDeltaPhiMETN(goodJetI,30,2.4,true,true); }; //Ben, overloaded

  double getDeltaPhiMETN_deltaT(unsigned int ijet, float otherpt, float othereta, bool otherid);
  double getDeltaPhiMETN_deltaT(unsigned int ijet) { return getDeltaPhiMETN_deltaT(ijet,30,2.4,true); } //overloaded
  double GetMinDeltaPhiMET(const unsigned int=3, const double=50., const double=2.4) const;

  int GetNumIsoTracks(const double=15.0, const bool=true) const;
  vector<std::pair<int,double> > GetIsoTracks(const double=15.0, const bool=true) const;
  void NewGetIsoTracks(vector<std::pair<int,double> > &eCands, vector<std::pair<int,double> > &muCands, vector<std::pair<int,double> > &hadCands, bool mT_cut=true);
  bool PassIsoTrackBaseline(const uint index) const;
  double GetPFCandIsolation(const uint indexA) const;
  double GetPFCandIsolationDeltaBetaCorr(const uint indexA) const;

  double GetTransverseMassMu() const;
  double GetTransverseMassEl() const;
  double GetDeltaThetaT(const double, const double) const;
  double GetWpT(const double, const double) const;

  double GetMHT(const double=30., const double=5.) const;
  double GetMHTPhi(const double=30., const double=5.) const;

  double GetDocMET() const;
  double GetGluinoPt(int=1) const;

  std::vector<int> GetTaus(const bool=false, const bool=false) const;
  bool PassPhys14TauID(const int, const bool=false, const bool=false) const;

  void GetTrueLeptons(std::vector<int> &true_electrons, std::vector<int> &true_muons, std::vector<int> &true_had_taus, std::vector<int> &true_lep_taus);
  double GetDRToClosestParton(const int imc) const;
  std::vector<int> MatchElectrons(const std::vector<int> true_electrons) const;
  int GetClosestRecoElectron(const uint imc) const;
  std::vector<int> MatchMuons(const std::vector<int> true_muons) const;
  int GetClosestRecoMuon(const uint imc) const;
  void PrintGenParticleInfo(const int imc) const;

  std::vector<int> MatchTks(const std::vector<int> true_leps, const vector<std::pair<int,double> > tracks) const;
  int GetClosestTk(const uint imc, const vector<std::pair<int,double> > tracks) const;
  
  double GetIsolation(const int ilep, const int ParticleType, double R=-1., const bool addCH=true, const bool addPH=true, const bool addNH=true, const bool usePFweight=false) const;

  int GetTrueElectron(float RecPt, float RecEta, float RecPhi, int &momID, bool &fromW, float &closest_dR) const;
  int GetTrueMuon(float RecPt, float RecEta, float RecPhi, int &momID, bool &fromW, float &closest_dR) const;
  int GetTrueParticle(float RecPt, float RecEta, float RecPhi, float &closest_dR, int ID) const;
  static int GetMom(float id, float mom, float gmom, float ggmom, bool &fromW);
  size_t MatchCandToStatus1(size_t icand, const std::vector<mc_particle> &parts) const;
  std::vector<mc_particle> GetMCParticles() const;
  std::vector<size_t> GetMoms(const std::vector<mc_particle> &parts);
  static size_t GetMom(size_t index, const std::vector<mc_particle> &parts);
  static bool FromTau(size_t index,
                      const std::vector<mc_particle> &parts,
                      const std::vector<size_t> &moms);
  static bool FromTauLep(size_t index,
                         const std::vector<mc_particle> &parts,
                         const std::vector<size_t> &moms);
  static bool FromW(size_t index,
                    const std::vector<mc_particle> &parts,
                    const std::vector<size_t> &moms);
  static unsigned NumDescendants(size_t index,
                                 const std::vector<mc_particle> &parts,
                                 const std::vector<size_t> &moms,
                                 bool req_chg=false);
  static bool IsDescendantOf(size_t descendant, size_t ancestor,
                             const std::vector<size_t> &moms);
  static size_t ParentTau(size_t index,
                          const std::vector<mc_particle> &parts,
                          const std::vector<size_t> &moms);
  static bool IsBrem(size_t index,
                     const std::vector<mc_particle> &parts,
                     const std::vector<size_t> &moms);

};

#endif
