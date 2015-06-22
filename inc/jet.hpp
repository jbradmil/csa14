#ifndef H_JET
#define H_JET

#include <cfloat>
#include "TLorentzVector.h"

enum jec_type_t { DEF=-1, RAW=0, CORR=1 };
enum obj_type_t { MUON=0, ELECTRON=1, PHOTON=2, MUTK=3, ELTK=4, HADTK=5 };

class Jet{
public:
  explicit Jet(const int jets_AKPF_indexIn=0, const double bTagIn=-DBL_MAX, 
	       const TLorentzVector tlvIn=TLorentzVector(0.0,0.0,0.0,0.0), const double corrOldIn=0., 
	       const TLorentzVector tlvRaw=TLorentzVector(0.0,0.0,0.0,0.0), const double corrNewIn=0., 
	       const TLorentzVector tlvCorr=TLorentzVector(0.0,0.0,0.0,0.0), const std::vector<float> vSubCorrIn = std::vector<float>());
  void SetBTag(const double bTagIn);
  void SetIndex(const double jets_AKPF_indexIn);
  void SetisLepton(const bool isLeptonIn);
  void SetisPhoton(const bool isPhotonIn);

  TLorentzVector GetTLV(const jec_type_t=DEF) const;
  double GetPt(const jec_type_t=DEF) const;
  double GetCorr(const jec_type_t=DEF) const;
  double GetSubCorr(const unsigned int) const;
  double GetBTag() const;
  unsigned int GetIndex() const;
  bool isLepton() const;
  bool isPhoton() const;
  // std::vector<int> GetOverlaps() const;
  // void SetOverlapIndex(const obj_type_t, const int);



  bool operator==(const Jet &jet) const;
  bool operator!=(const Jet &jet) const;
  bool operator<(const Jet &jet) const;
  bool operator>(const Jet &jet) const;
  bool operator<=(const Jet &jet) const;
  bool operator>=(const Jet &jet) const;
private:
  int jets_AKPF_index_;
  double bTag_;
  TLorentzVector vecIn_;
  double corrOld_;
  TLorentzVector vecRaw_;
  double corrNew_;
  TLorentzVector vecCorr_;
  std::vector<float> vSubCorr_;
  bool isLepton_;
  bool isPhoton_;
  //std::vector<int> jet_overlaps_;

};

#endif
