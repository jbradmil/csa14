#include <utility>
#include "jet.hpp"

Jet::Jet(const int jets_AKPF_indexIn, const double bTagIn, const TLorentzVector tlvIn, const double corrOldIn, 
		 const TLorentzVector tlvRaw, const double corrNewIn, const TLorentzVector tlvCorr) : 
		 jets_AKPF_index_(jets_AKPF_indexIn),
		 bTag_(bTagIn),
		 vecIn_(tlvIn),
		 corrOld_(corrOldIn),
		 vecRaw_(tlvRaw),
		 corrNew_(corrNewIn),
		 vecCorr_(tlvCorr){
		 }

void Jet::SetBTag(const double bTagIn){
	bTag_=bTagIn;
}

TLorentzVector Jet::GetTLV(const jec_type_t jec_type) const{
	if (jec_type==DEF) return vecIn_;
	else if (jec_type==RAW) return vecRaw_;
	else return vecCorr_; // return corrected vector by default
}

double Jet::GetPt(const jec_type_t jec_type) const{
	if (jec_type==DEF) return vecIn_.Pt();
	else if (jec_type==RAW) return vecRaw_.Pt();
	else return vecCorr_.Pt(); // return corrected pt by default
}

double Jet::GetCorr(const jec_type_t jec_type) const{
	if (jec_type==DEF) return corrOld_;
	else if (jec_type==RAW) return 1.;
	else return corrNew_; // return latest correction by default
}

double Jet::GetBTag() const{
	return bTag_;
}

unsigned int Jet::GetIndex() const{
	return jets_AKPF_index_;
}

bool Jet::operator==(const Jet &jet) const{
	return vecCorr_==jet.vecCorr_ && bTag_==jet.bTag_;
}

bool Jet::operator!=(const Jet &jet) const{
	return !(*this==jet);
}

// Note: have changed these relational operators to sort jets by corrected pt!
// If there are no new corrections applied (i.e. for pre-v78), sorts by input pt

bool Jet::operator<(const Jet &jet) const{
	std::pair<double, double> this_one(vecCorr_.Pt(), bTag_);
	std::pair<double, double> that_one(jet.vecCorr_.Pt(), jet.bTag_);
	return this_one<that_one;
}

bool Jet::operator>(const Jet &jet) const{
	std::pair<double, double> this_one(vecCorr_.Pt(), bTag_);
	std::pair<double, double> that_one(jet.vecCorr_.Pt(), jet.bTag_);
	return this_one>that_one;
}

bool Jet::operator<=(const Jet &jet) const{
	return *this==jet || *this<jet;
}

bool Jet::operator>=(const Jet &jet) const{
	return *this==jet || *this>jet;
}
