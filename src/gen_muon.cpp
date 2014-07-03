#include <utility>
#include "gen_muon.hpp"
#include "TLorentzVector.h"

GenMuon::GenMuon(const TLorentzVector vecIn, const int mc_doc_indexIn, const int pIdIn, const int momIdIn):vec(vecIn),mc_doc_index(mc_doc_indexIn),pId(pIdIn),momId(momIdIn){
}

void GenMuon::SetLorentzVector(const TLorentzVector vecIn){
  vec=vecIn;
}

void GenMuon::SetPDGId(const int pIdIn){
  pId=pIdIn;
}

void GenMuon::SetMotherId(const int momIdIn){
  momId=momIdIn;
}

void GenMuon::SetMinDR(const double minDRIn){
  minDR=minDRIn;
}

TLorentzVector GenMuon::GetLorentzVector() const{
  return vec;
}

unsigned int GenMuon::GetMCDocIndex() const{
  return mc_doc_index;
}

int GenMuon::GetPDGId() const{
  return pId;
}

int GenMuon::GetMotherId() const{
  return momId;
}

double
 GenMuon::GetMinDR() const{
  return minDR;
}

