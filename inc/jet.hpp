#ifndef H_JET
#define H_JET

#include <cfloat>
#include "TLorentzVector.h"

enum jec_type_t { DEF=-1, RAW=0, CORR=1 };

class Jet{
public:
  explicit Jet(const int jets_AKPF_indexIn=0, const double bTagIn=-DBL_MAX, 
	       const TLorentzVector tlvIn=TLorentzVector(0.0,0.0,0.0,0.0), const double corrOldIn=0., 
	       const TLorentzVector tlvRaw=TLorentzVector(0.0,0.0,0.0,0.0), const double corrNewIn=0., 
	       const TLorentzVector tlvCorr=TLorentzVector(0.0,0.0,0.0,0.0), const std::vector<float> vSubCorrIn = std::vector<float>());
  void SetBTag(const double bTagIn);
  void SetIndex(const double jets_AKPF_indexIn);

  TLorentzVector GetTLV(const jec_type_t=DEF) const;
  double GetPt(const jec_type_t=DEF) const;
  double GetCorr(const jec_type_t=DEF) const;
  double GetSubCorr(const unsigned int) const;
  double GetBTag() const;
  unsigned int GetIndex() const;


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
};

#endif
