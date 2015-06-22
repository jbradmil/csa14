#ifndef H_ITK
#define H_ITK

#include <cfloat>
#include "TLorentzVector.h"


class IsolatedTrack{
public:
  explicit IsolatedTrack(const int pfcand_indexIn=-1,
			 const int pdgIdIn=0, 
			 const TLorentzVector TLV=TLorentzVector(0.0,0.0,0.0,0.0),
			 double iso=DBL_MAX);
  void SetIso(const double isoIn);
  void SetIndex(const int pfcand_indexIn);

  TLorentzVector GetTLV() const;
  double GetPt() const;
  double GetIso() const;
  int GetIndex() const;
  void SetisLepton(const bool isLeptonIn);
  bool isLepton() const;


  bool operator==(const IsolatedTrack &jet) const;
  bool operator!=(const IsolatedTrack &jet) const;
  bool operator<(const IsolatedTrack &jet) const;
  bool operator>(const IsolatedTrack &jet) const;
  bool operator<=(const IsolatedTrack &jet) const;
  bool operator>=(const IsolatedTrack &jet) const;
private:
  int pfcand_index_;
  int pdgId_;
  TLorentzVector tlv_;
  double iso_;
  bool isLepton_;
};

#endif
