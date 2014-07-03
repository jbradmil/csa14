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
#include "cfa.hpp"
#include "b_jet.hpp"
#include "event_number.hpp"
#include "gen_muon.hpp"

typedef unsigned int uint;

class EventHandler : public cfA{
public:
  EventHandler(const std::string &, const bool, const double=1.0, const bool=false);

  void SetScaleFactor(const double);
  void SetScaleFactor(const double, const double, const int);

protected:
  mutable std::vector<GenMuon>genMuonCache;//caching for efficiency
  mutable bool genMuonsUpToDate;
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

  int GetNumRA2bElectrons(const unsigned short=0, const bool=true) const;
  int GetNumRA2bMuons(const unsigned short=0, const bool=true) const;
  int GetNumRA2bTaus(const unsigned short=0, const bool=true) const;

  bool isIsoTrack(const unsigned int, const double=10.0) const;
  bool isQualityTrack(const unsigned int) const;

  int NewGetNumIsoTracks(const double=10.0) const;

  double GetElectronRelIso(const unsigned int) const;

  double GetTopPtWeight() const;
  double GetTopPt() const;
  double GetTopPtWeightOfficial() const;

  int GetNGenParticles(const int, const bool=false) const;
  int GetGenParticleIndex(const int, const int=0) const;

  bool isGenMuon(const int) const;
  void GetGenMuons() const;
  double GetGenMuonMinDR(const int, const std::vector<float>, const std::vector<float>) const;
  void SetupGenMuons() const;
  double ReadMuonCacheDR(uint) const;

};

#endif
