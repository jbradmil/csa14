#ifndef H_FATJET
#define H_FATJET

#include <cfloat>
#include "TLorentzVector.h"

class FatJet{
public:
  explicit FatJet(const TLorentzVector=TLorentzVector(0.0,0.0,0.0,0.0), const double nConstIn=-DBL_MAX, const int fastjets_AK4_R1p2_R0p5pT30_indexIn=0);

  void SetLorentzVector(const TLorentzVector vecIn);
  void SetNConst(const double nConstIn);
  void SetIndex(const double fastjets_AK4_R1p2_R0p5pT30_indexIn);
  void SetmJ();

  TLorentzVector GetLorentzVector() const;
  double GetNConst() const;
  unsigned int GetIndex() const;
  double GetmJ() const;

  bool operator==(const FatJet &jet) const;
  bool operator!=(const FatJet &jet) const;
  bool operator<(const FatJet &jet) const;
  bool operator>(const FatJet &jet) const;
  bool operator<=(const FatJet &jet) const;
  bool operator>=(const FatJet &jet) const;
private:
  TLorentzVector vec;
  double nConst;
  int fastjets_AK4_R1p2_R0p5pT30_index;
  double mJ;
};

#endif
