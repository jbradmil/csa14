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
#include "event_number.hpp"
#include "gen_muon.hpp"
#include "gen_electron.hpp"
#include "gen_tau.hpp"

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

class EventHandler : public cfA{
public:
  EventHandler(const std::string &, const bool, const double=1.0, const bool=false);

  void SetScaleFactor(const double);
  void SetScaleFactor(const double, const double, const int);

protected:
  mutable std::vector<BJet> sortedBJetCache;//caching for efficiency
  mutable bool bJetsUpToDate;//cached value correct
  mutable std::vector<GenMuon> genMuonCache;
  mutable bool genMuonsUpToDate;
  mutable std::vector<GenElectron> genElectronCache;
  mutable bool genElectronsUpToDate; 
  mutable std::vector<GenTau>genTauCache;
  mutable bool genTausUpToDate;
  mutable std::vector<int> recoMuonCache;
  mutable bool recoMuonsUpToDate;
  mutable std::vector<int> recoElectronCache;
  mutable bool recoElectronsUpToDate; 
  mutable std::vector<int>recoTauCache;
  mutable bool recoTausUpToDate;
  mutable bool betaUpToDate;
  static const double CSVTCut, CSVMCut, CSVLCut;
  double scaleFactor;
  mutable std::vector<double> beta;

  int GetcfAVersion() const;
  void GetSortedBJets() const;
  double GetHighestJetPt(const unsigned int=1) const;
  double GetHighestJetCSV(const unsigned int=1) const;
  double GetJetXEta(const unsigned int=1) const;
  double GetJetXPhi(const unsigned int=1) const;
  double GetJetXCSV(const unsigned int=1) const;
  double GetJetXPartonId(const unsigned int=1) const;
  double GetJetXMotherId(const unsigned int=1) const;

  void GetEntry(const unsigned int);
  void GetBeta(const std::string which="beta") const;

  bool PassesSpecificTrigger(const std::string) const;

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
  bool PassesMETCleaningCut() const;

  int GetPBNR() const;
  bool PassesBadJetFilter() const;
  bool isGoodJet(const unsigned int, const bool=true, const double=50.0,
                 const double=2.4/*, const bool=true*/) const;
  bool isCleanJet(const unsigned int, const int) const;
  bool isProblemJet(const unsigned int) const;
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

  bool isGenMuon(const int) const;
  bool isIsoGenMuon(const int) const;
  void GetGenMuons() const;
  int GetNumIgnoredGenMuons() const;
  std::pair <int, double> GetGenMuonMinDR(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenMuonMinDPt(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenMuonMinDRCSA14(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenMuonMinDPtCSA14(const int, const vector<uint> ) const;
  void SetupGenMuons() const;
  int GetGenMuonLossCode(const int) const;
  float GetMinDRMuonJet(const int) const;

  bool isGenElectron(const int) const;
  bool isIsoGenElectron(const int) const;
  void GetGenElectrons() const;
  int GetNumIgnoredGenElectrons() const;
  std::pair <int, double> GetGenElectronMinDR(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenElectronMinDPt(const int, const vector<uint> ) const;
  void SetupGenElectrons() const;
  int GetGenElectronLossCode(const int) const;
  float GetMinDRElectronJet(const int) const;

  float GetCorrespondingTopPt(const int) const;
  float GetCorrespondingDeltaRWb(const int) const;
  float GetCorrespondingDeltaRlb(const int) const;

  bool isGenTau(const int) const;
  void GetGenTaus() const;
  int GetNumIgnoredGenTaus() const;
  std::pair <int, double> GetGenTauMinDR(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenTauMinDPt(const int, const vector<uint> ) const;
  void SetupGenTaus() const;
  int GetGenTauLossCode(const int) const;

  bool hasPFMatch(const int index, const int pdgId) const;
  bool IsFromB(const int, const int, const int) const;

  vector<int> GetRecoMuons(const bool veto) const;
  bool isRecoMuon(const uint imu, const uint level=0) const;
  double GetMuonRelIso(const unsigned int imu) const;
  bool passedRecoMuonSelection(uint imu, float MuonPTThreshold=20., float MuonETAThreshold=2.4);
  bool passedRecoMuonVeto(uint imu, float MuonPTThreshold=15., float MuonETAThreshold=2.5);
  double getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex);
  bool passedBaseMuonSelection(uint imu, float MuonPTThreshold=0., float MuonETAThreshold=5.);
  float GetRecoMuonIsolation(uint imu);

  vector<int> GetRecoElectrons(const bool veto) const;
  bool isRecoElectron(const uint imu, const uint level=0) const;

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

  double GetHT(double=50.) const;
  unsigned int GetNumGoodJets(const double pt=50.) const;
  unsigned int GetNumCSVTJets(const double=50.) const;
  unsigned int GetNumCSVMJets(const double=50.) const;
  unsigned int GetNumCSVLJets(const double=50.) const;

  double GetMTW(const double, const double, const double, const double) const;

  double getMinDeltaPhiMETN(unsigned int maxjets, float mainpt, float maineta, bool mainid, float otherpt, float othereta, bool otherid, bool useArcsin=true );
  double getMinDeltaPhiMETN(unsigned int maxjets) {return getMinDeltaPhiMETN(maxjets,50.,2.4,true,30.,2.4,true,true); };
  double getDeltaPhiMETN( unsigned int goodJetI, float otherpt, float othereta, bool otherid, bool useArcsin ); //Ben
  double getDeltaPhiMETN( unsigned int goodJetI ) {return getDeltaPhiMETN(goodJetI,30,2.4,true,true); }; //Ben, overloaded

  double getDeltaPhiMETN_deltaT(unsigned int ijet, float otherpt, float othereta, bool otherid);
  double getDeltaPhiMETN_deltaT(unsigned int ijet) { return getDeltaPhiMETN_deltaT(ijet,30,2.4,true); } //overloaded
  double GetMinDeltaPhiMET(const unsigned int=3, const double=50., const double=2.4) const;

  int GetNumIsoTracks(const double=10.0) const;

  double GetTransverseMass() const;
  double GetMHT(const double=30., const double=5.) const;
  double GetMHTPhi(const double=30., const double=5.) const;

};

#endif
