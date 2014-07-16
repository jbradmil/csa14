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
  mutable std::vector<GenMuon> genMuonCache;
  mutable bool genMuonsUpToDate;
  mutable std::vector<GenElectron> genElectronCache;
  mutable bool genElectronsUpToDate; 
  mutable std::vector<GenTau>genTauCache;
  mutable bool genTausUpToDate;
  mutable bool betaUpToDate;
  static const double CSVTCut, CSVMCut, CSVLCut;
  double scaleFactor;
  mutable std::vector<double> beta;

  int GetcfAVersion() const;

  void GetEntry(const unsigned int);
  void GetBeta(const std::string which="beta") const;


  double GetNumInteractions() const;
  unsigned short GetNumVertices() const;
  double GetPUWeight(reweight::LumiReWeighting &) const;
  bool isGoodVertex(const unsigned int) const;

  bool PassesPVCut() const;
  bool PassesMETCleaningCut() const;
  bool PassesSpecificTrigger(const std::string) const;

  int GetPBNR() const;
  bool PassesBadJetFilter() const;
  bool isGoodJet(const unsigned int, const bool=true, const double=20.0,
                 const double=2.4, const bool=true) const;
  bool isProblemJet(const unsigned int) const;
  bool jetPassLooseID(const unsigned int) const;

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

  int GetNGenParticles(const int, const float=0., const bool=false) const;
  int GetGenParticleIndex(const int, const int=0) const;

  int GetJetGenId(const int) const;

  bool isGenMuon(const int) const;
  bool isIsoGenMuon(const int) const;
  void GetGenMuons() const;
  int GetNumIgnoredGenMuons() const;
  std::pair <int, double> GetGenMuonMinDR(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenMuonMinDPt(const int, const vector<uint> ) const;
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

  bool isGenTau(const int) const;
  void GetGenTaus() const;
  int GetNumIgnoredGenTaus() const;
  std::pair <int, double> GetGenTauMinDR(const int, const vector<uint> ) const;
  std::pair <int, double> GetGenTauMinDPt(const int, const vector<uint> ) const;
  void SetupGenTaus() const;
  int GetGenTauLossCode(const int) const;

  bool hasPFMatch(int index, int pdgId, int &pfIdx);

  vector<int> GetRecoMuons(bool veto);
  bool isRecoMuon(const uint imu, const uint level=0) const;
  double GetMuonRelIso(const unsigned int imu) const;
  bool passedRecoMuonSelection(uint imu, float MuonPTThreshold=20., float MuonETAThreshold=2.4);
  bool passedRecoMuonVeto(uint imu, float MuonPTThreshold=15., float MuonETAThreshold=2.5);
  double getDZ(double vx, double vy, double vz, double px, double py, double pz, int firstGoodVertex);
  bool passedBaseMuonSelection(uint imu, float MuonPTThreshold=0., float MuonETAThreshold=5.);
  float GetRecoMuonIsolation(uint imu);

  vector<int> GetRecoElectrons(bool veto);
  bool isRecoElectron(const uint imu, const uint level=0) const;

  bool passedRA4ElectronSelection(uint iel, float ElectronPTThreshold=20.);
  bool passedRA4ElectronVeto(uint iel, float ElectronPTThreshold=15.);
  bool passedBaseElectronSelection(uint iel, float ElectronPTThreshold=20.);
  float GetRA4ElectronIsolation(uint iel);
  float GetEffectiveArea(float SCEta, bool isMC);

  vector<int> GetCSAElectrons(bool veto);
  bool passedCSAElectronSelection(uint iel, float ElectronPTThreshold=20.);
  bool passedCSAElectronVeto(uint iel, float ElectronPTThreshold=15.);
  float GetCSAElectronIsolation(const uint iel) const;
  bool passedCSABaseElectronSelection(uint iel, float ElectronPTThreshold=20.);

 
  bool IsMC();

  double GetHT() const;
  int GetNumGoodJets() const;
  int GetNumCSVTJets() const;
  int GetNumCSVMJets() const;
  int GetNumCSVLJets() const;

};

#endif
