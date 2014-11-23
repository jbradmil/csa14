#include <utility>
#include "fat_jet.hpp"
#include "TLorentzVector.h"

FatJet::FatJet(const TLorentzVector vecIn, const double nConstIn, const int fastjets_AK4_R1p2_R0p5pT30_indexIn):vec(vecIn),nConst(nConstIn),fastjets_AK4_R1p2_R0p5pT30_index(fastjets_AK4_R1p2_R0p5pT30_indexIn){
  mJ=vecIn.M();
}

void FatJet::SetLorentzVector(const TLorentzVector vecIn){
  vec=vecIn;
}

void FatJet::SetNConst(const double nConstIn){
  nConst=nConstIn;
}

void FatJet::SetmJ(){
  mJ=vec.M();
}

TLorentzVector FatJet::GetLorentzVector() const{
  return vec;
}

double FatJet::GetNConst() const{
  return nConst;
}

unsigned int FatJet::GetIndex() const{
  return fastjets_AK4_R1p2_R0p5pT30_index;
}

double FatJet::GetmJ() const{
  return mJ;
}

bool FatJet::operator==(const FatJet &jet) const{
  return vec==jet.vec && mJ==jet.mJ;
}

bool FatJet::operator!=(const FatJet &jet) const{
  return !(*this==jet);
}

// Note: sort by mJ!

bool FatJet::operator<(const FatJet &jet) const{
  std::pair<double, double> this_one(mJ,vec.Pt());
  std::pair<double, double> that_one(jet.mJ,jet.vec.Pt());
  return this_one<that_one;
}

bool FatJet::operator>(const FatJet &jet) const{
  std::pair<double, double> this_one(mJ,vec.Pt());
  std::pair<double, double> that_one(jet.mJ,jet.vec.Pt());
  return this_one>that_one;
}

bool FatJet::operator<=(const FatJet &jet) const{
  return *this==jet || *this<jet;
}

bool FatJet::operator>=(const FatJet &jet) const{
  return *this==jet || *this>jet;
}
