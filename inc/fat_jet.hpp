#ifndef H_FATJET
#define H_FATJET

#include <cfloat>
#include "TLorentzVector.h"

class FatJet{
public:
  explicit FatJet(const TLorentzVector=TLorentzVector(0.0,0.0,0.0,0.0), const int nConstIn=-1, const int fastjets_AK4_R1p2_R0p5pT30_indexIn=0);

  void SetLorentzVector(const TLorentzVector vecIn);
  void SetNConst(const int nConstIn);
  void SetIndex(const double fastjets_AK4_R1p2_R0p5pT30_indexIn);
  void SetmJ();

  TLorentzVector GetLorentzVector() const;
  int GetNConst() const;
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
  int nConst;
  int fastjets_AK4_R1p2_R0p5pT30_index;
  double mJ;
};

#endif
