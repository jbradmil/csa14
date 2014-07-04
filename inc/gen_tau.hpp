#ifndef H_GENTAU
#define H_GENTAU

#include <cfloat>
#include <utility>
#include "TLorentzVector.h"

class GenTau{
public:
  explicit GenTau(const TLorentzVector=TLorentzVector(0.0,0.0,0.0,0.0), const int mc_taus_indexIn=0, const int pIdIn=0, const int momIdIn=0);

  void SetLorentzVector(const TLorentzVector vecIn);
  void SetPDGId(const int pIdIn);
  void SetMotherId(const int momIdIn);
  void SetMCTausIndex(const double  mc_taus_indexIn);
  void SetMinDR(const std::pair<int, double>);
  void SetMinDPt(const std::pair<int, double>);
  void SetTausMatch(const int);
  void SetLossCode(const int);

  TLorentzVector GetLorentzVector() const;
  unsigned int GetMCTausIndex() const;
  int GetPDGId() const;
  int GetMotherId() const;
  std::pair<int, double> GetMinDR() const;
  std::pair<int, double> GetMinDPt() const;
  int GetTausMatch() const;
  int GetLossCode() const;

  bool operator==(const GenTau &tau) const;
  bool operator!=(const GenTau &tau) const;
  bool operator<(const GenTau &tau) const;
  bool operator>(const GenTau &tau) const;
  bool operator<=(const GenTau &tau) const;
  bool operator>=(const GenTau &tau) const;

private:
  TLorentzVector vec;
  int mc_taus_index;
  int pId;
  int momId;
  std::pair<int, double> minDR;
  std::pair<int, double> minDPt;
  int taus_match;
  int loss_code;
};

#endif
