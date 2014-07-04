#ifndef H_GENMUON
#define H_GENMUON

#include <cfloat>
#include <utility>
#include "TLorentzVector.h"

class GenMuon{
public:
  explicit GenMuon(const TLorentzVector=TLorentzVector(0.0,0.0,0.0,0.0), const int mc_mus_indexIn=0, const int pIdIn=0, const int momIdIn=0);

  void SetLorentzVector(const TLorentzVector vecIn);
  void SetPDGId(const int pIdIn);
  void SetMotherId(const int momIdIn);
  void SetMCMusIndex(const double  mc_mus_indexIn);
  void SetMinDR(const std::pair<int, double>);
  void SetMinDPt(const std::pair<int, double>);
  void SetMusMatch(const int);
  void SetLossCode(const int);

  TLorentzVector GetLorentzVector() const;
  unsigned int GetMCMusIndex() const;
  int GetPDGId() const;
  int GetMotherId() const;
  std::pair<int, double> GetMinDR() const;
  std::pair<int, double> GetMinDPt() const;
  int GetMusMatch() const;
  int GetLossCode() const;

  bool operator==(const GenMuon &muon) const;
  bool operator!=(const GenMuon &muon) const;
  bool operator<(const GenMuon &muon) const;
  bool operator>(const GenMuon &muon) const;
  bool operator<=(const GenMuon &muon) const;
  bool operator>=(const GenMuon &muon) const;

private:
  TLorentzVector vec;
  int mc_mus_index;
  int pId;
  int momId;
  std::pair<int, double> minDR;
  std::pair<int, double> minDPt;
  int mus_match;
  int loss_code;
};

#endif
