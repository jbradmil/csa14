#ifndef H_GENELECTRON
#define H_GENELECTRON

#include <cfloat>
#include <utility>
#include "TLorentzVector.h"

class GenElectron{
public:
  explicit GenElectron(const TLorentzVector=TLorentzVector(0.0,0.0,0.0,0.0), const int mc_els_indexIn=0, const int pIdIn=0, const int momIdIn=0);

  void SetLorentzVector(const TLorentzVector vecIn);
  void SetPDGId(const int pIdIn);
  void SetMotherId(const int momIdIn);
  void SetMCElsIndex(const double  mc_els_indexIn);
  void SetIsIso(const bool isIsoIn);
  void SetMinDR(const std::pair<int, double>);
  void SetMinDPt(const std::pair<int, double>);
  void SetElsMatch(const int);
  void SetLossCode(const int);
  void SetIsVeto(bool isVetoIn);
  void SetTopPt(float topPtIn);
  void SetDeltaRWb(float deltaRWbIn);

  TLorentzVector GetLorentzVector() const;
  unsigned int GetMCElsIndex() const;
  int GetPDGId() const;
  int GetMotherId() const;
  bool IsIso() const;
  bool IsVeto() const;
  std::pair<int, double> GetMinDR() const;
  std::pair<int, double> GetMinDPt() const;
  int GetElsMatch() const;
  float GetTopPt() const;
  float GetDeltaRWb() const;
  int GetLossCode() const;

  bool operator==(const GenElectron &electron) const;
  bool operator!=(const GenElectron &electron) const;
  bool operator<(const GenElectron &electron) const;
  bool operator>(const GenElectron &electron) const;
  bool operator<=(const GenElectron &electron) const;
  bool operator>=(const GenElectron &electron) const;

private:
  TLorentzVector vec;
  int mc_els_index;
  int pId;
  int momId;
  bool is_iso;
  bool is_veto;
  std::pair<int, double> minDR;
  std::pair<int, double> minDPt;
  int els_match;
  float top_pt;
  float deltaR_Wb;
  int loss_code;
};

#endif
