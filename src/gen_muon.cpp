#include <utility>
#include "gen_muon.hpp"
#include "TLorentzVector.h"

GenMuon::GenMuon(const TLorentzVector vecIn, const int mc_mus_indexIn, const int pIdIn, const int momIdIn):vec(vecIn),mc_mus_index(mc_mus_indexIn),pId(pIdIn),momId(momIdIn){
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

void GenMuon::SetMinDR(const std::pair <int,double> minDRIn){
  minDR=minDRIn;
}

void GenMuon::SetMinDPt(const std::pair <int,double> minDPtIn){
  minDPt=minDPtIn;
}

void GenMuon::SetMusMatch(const int musMatchIn){
  mus_match=musMatchIn;
}

void GenMuon::SetLossCode(const int lossCodeIn){
  loss_code=lossCodeIn;
}

TLorentzVector GenMuon::GetLorentzVector() const{
  return vec;
}

unsigned int GenMuon::GetMCMusIndex() const{
  return mc_mus_index;
}

int GenMuon::GetPDGId() const{
  return pId;
}

int GenMuon::GetMotherId() const{
  return momId;
}

std::pair <int,double> GenMuon::GetMinDR() const{
  return minDR;
}

std::pair <int,double> GenMuon::GetMinDPt() const{
  return minDPt;
}

int GenMuon::GetMusMatch() const{
  return mus_match;
}

int GenMuon::GetLossCode() const{
  return loss_code;
}

bool GenMuon::operator==(const GenMuon &muon) const{
  return vec==muon.vec &&  mc_mus_index==muon.mc_mus_index;
}

bool GenMuon::operator!=(const GenMuon &muon) const{
  return !(*this==muon);
}

bool GenMuon::operator<(const GenMuon &muon) const{
  double this_pt=vec.Pt();
  double that_pt=muon.vec.Pt();
  return this_pt<that_pt;
}

bool GenMuon::operator>(const GenMuon &muon) const{
  double this_pt=vec.Pt();
  double that_pt=muon.vec.Pt();
  return this_pt>that_pt;
}

bool GenMuon::operator<=(const GenMuon &muon) const{
  return *this==muon || *this<muon;
}

bool GenMuon::operator>=(const GenMuon &muon) const{
  return *this==muon || *this>muon;
}

