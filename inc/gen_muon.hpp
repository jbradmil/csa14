#ifndef H_GENMUON
#define H_GENMUON

#include <cfloat>
#include "TLorentzVector.h"

class GenMuon{
public:
  explicit GenMuon(const TLorentzVector=TLorentzVector(0.0,0.0,0.0,0.0), const int mc_doc_indexIn=0, const int pIdIn=0, const int momIdIn=0);

  void SetLorentzVector(const TLorentzVector vecIn);
  void SetPDGId(const int pIdIn);
  void SetMotherId(const int momIdIn);
  void SetMCDocIndex(const double  mc_doc_indexIn);
  void SetMinDR(const double);

  TLorentzVector GetLorentzVector() const;
  unsigned int GetMCDocIndex() const;
  int GetPDGId() const;
  int GetMotherId() const;
  double GetMinDR() const;

private:
  TLorentzVector vec;
  int mc_doc_index;
  int pId;
  int momId;
  double minDR;
};

#endif
