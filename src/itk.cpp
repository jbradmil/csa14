#include <utility>
#include "itk.hpp"

IsolatedTrack::IsolatedTrack(const int pfcand_indexIn, const int pdgIdIn, const TLorentzVector tlvIn, const double isoIn) : 
  pfcand_index_(pfcand_indexIn),
  pdgId_(pdgIdIn),
  tlv_(tlvIn),
  iso_(isoIn),
  isLepton_(false){
  }

void IsolatedTrack::SetIso(const double isoIn){
  iso_=isoIn;
}

void IsolatedTrack::SetIndex(const int pfcand_indexIn){
  pfcand_index_=pfcand_indexIn;
}

TLorentzVector IsolatedTrack::GetTLV() const{
  return tlv_;
}

double IsolatedTrack::GetPt() const{
  return tlv_.Pt(); 
}

double IsolatedTrack::GetIso() const{
  return iso_;
}

int IsolatedTrack::GetIndex() const{
  return pfcand_index_;
}

void IsolatedTrack::SetisLepton(const bool isLeptonIn) {
  isLepton_=isLeptonIn;
}

bool IsolatedTrack::isLepton() const {
  return isLepton_;
}

bool IsolatedTrack::operator==(const IsolatedTrack &itk) const{
  return tlv_==itk.tlv_ && tlv_.Pt()==itk.tlv_.Pt();
}

bool IsolatedTrack::operator!=(const IsolatedTrack &itk) const{
  return !(*this==itk);
}

bool IsolatedTrack::operator<(const IsolatedTrack &itk) const{
  std::pair<double, double> this_one(tlv_.Pt(), tlv_.Pt());
  std::pair<double, double> that_one(itk.tlv_.Pt(), itk.tlv_.Pt());
  return this_one<that_one;
}

bool IsolatedTrack::operator>(const IsolatedTrack &itk) const{
  std::pair<double, double> this_one(tlv_.Pt(), tlv_.Pt());
  std::pair<double, double> that_one(itk.tlv_.Pt(), itk.tlv_.Pt());
  return this_one>that_one;
}

bool IsolatedTrack::operator<=(const IsolatedTrack &itk) const{
  return *this==itk || *this<itk;
}

bool IsolatedTrack::operator>=(const IsolatedTrack &itk) const{
  return *this==itk || *this>itk;
}
