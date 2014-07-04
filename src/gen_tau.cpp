#include <utility>
#include "gen_tau.hpp"
#include "TLorentzVector.h"

GenTau::GenTau(const TLorentzVector vecIn, const int mc_taus_indexIn, const int pIdIn, const int momIdIn):vec(vecIn),mc_taus_index(mc_taus_indexIn),pId(pIdIn),momId(momIdIn){
}

void GenTau::SetLorentzVector(const TLorentzVector vecIn){
  vec=vecIn;
}

void GenTau::SetPDGId(const int pIdIn){
  pId=pIdIn;
}

void GenTau::SetMotherId(const int momIdIn){
  momId=momIdIn;
}

void GenTau::SetMinDR(const std::pair <int,double> minDRIn){
  minDR=minDRIn;
}

void GenTau::SetMinDPt(const std::pair <int,double> minDPtIn){
  minDPt=minDPtIn;
}

void GenTau::SetTausMatch(const int tausMatchIn){
  taus_match=tausMatchIn;
}

void GenTau::SetLossCode(const int lossCodeIn){
  loss_code=lossCodeIn;
}

TLorentzVector GenTau::GetLorentzVector() const{
  return vec;
}

unsigned int GenTau::GetMCTausIndex() const{
  return mc_taus_index;
}

int GenTau::GetPDGId() const{
  return pId;
}

int GenTau::GetMotherId() const{
  return momId;
}

std::pair <int,double> GenTau::GetMinDR() const{
  return minDR;
}

std::pair <int,double> GenTau::GetMinDPt() const{
  return minDPt;
}

int GenTau::GetTausMatch() const{
  return taus_match;
}

int GenTau::GetLossCode() const{
  return loss_code;
}

bool GenTau::operator==(const GenTau &tau) const{
  return vec==tau.vec &&  mc_taus_index==tau.mc_taus_index;
}

bool GenTau::operator!=(const GenTau &tau) const{
  return !(*this==tau);
}

bool GenTau::operator<(const GenTau &tau) const{
  double this_pt=vec.Pt();
  double that_pt=tau.vec.Pt();
  return this_pt<that_pt;
}

bool GenTau::operator>(const GenTau &tau) const{
  double this_pt=vec.Pt();
  double that_pt=tau.vec.Pt();
  return this_pt>that_pt;
}

bool GenTau::operator<=(const GenTau &tau) const{
  return *this==tau || *this<tau;
}

bool GenTau::operator>=(const GenTau &tau) const{
  return *this==tau || *this>tau;
}

