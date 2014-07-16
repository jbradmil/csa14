#include <utility>
#include "gen_electron.hpp"
#include "TLorentzVector.h"

GenElectron::GenElectron(const TLorentzVector vecIn, const int mc_els_indexIn, const int pIdIn, const int momIdIn):vec(vecIn),mc_els_index(mc_els_indexIn),pId(pIdIn),momId(momIdIn){
}

void GenElectron::SetLorentzVector(const TLorentzVector vecIn){
  vec=vecIn;
}

void GenElectron::SetPDGId(const int pIdIn){
  pId=pIdIn;
}

void GenElectron::SetMotherId(const int momIdIn){
  momId=momIdIn;
}

unsigned int GenElectron::GetMCElsIndex() const{
  return mc_els_index;
}

void GenElectron::SetIsIso(const bool isIsoIn){
  is_iso=isIsoIn;
}

void GenElectron::SetMinDR(const std::pair <int,double> minDRIn){
  minDR=minDRIn;
}

void GenElectron::SetMinDPt(const std::pair <int,double> minDPtIn){
  minDPt=minDPtIn;
}

void GenElectron::SetElsMatch(const int elsMatchIn){
  els_match=elsMatchIn;
}

void GenElectron::SetLossCode(const int lossCodeIn){
  loss_code=lossCodeIn;
}

void GenElectron::SetIsVeto(bool isVetoIn){
  is_veto=isVetoIn;
}

void GenElectron::SetTopPt(float topPtIn){
  top_pt=topPtIn;
}
void GenElectron::SetDeltaRWb(float deltaRWbIn){
  deltaR_Wb=deltaRWbIn;
}

TLorentzVector GenElectron::GetLorentzVector() const{
  return vec;
}

int GenElectron::GetPDGId() const{
  return pId;
}

int GenElectron::GetMotherId() const{
  return momId;
}

bool GenElectron::IsIso() const{
  return is_iso;
}

bool GenElectron::IsVeto() const{
  return is_veto;
}

float GenElectron::GetTopPt() const{
  return top_pt;
}

float GenElectron::GetDeltaRWb() const{
  return deltaR_Wb;
}

std::pair <int,double> GenElectron::GetMinDR() const{
  return minDR;
}

std::pair <int,double> GenElectron::GetMinDPt() const{
  return minDPt;
}

int GenElectron::GetElsMatch() const{
  return els_match;
}

int GenElectron::GetLossCode() const{
  return loss_code;
}

bool GenElectron::operator==(const GenElectron &Electron) const{
  return vec==Electron.vec &&  mc_els_index==Electron.mc_els_index;
}

bool GenElectron::operator!=(const GenElectron &Electron) const{
  return !(*this==Electron);
}

bool GenElectron::operator<(const GenElectron &Electron) const{
  double this_pt=vec.Pt();
  double that_pt=Electron.vec.Pt();
  return this_pt<that_pt;
}

bool GenElectron::operator>(const GenElectron &Electron) const{
  double this_pt=vec.Pt();
  double that_pt=Electron.vec.Pt();
  return this_pt>that_pt;
}

bool GenElectron::operator<=(const GenElectron &Electron) const{
  return *this==Electron || *this<Electron;
}

bool GenElectron::operator>=(const GenElectron &Electron) const{
  return *this==Electron || *this>Electron;
}

