#include "macros/junews/lostleptons/utils.h"
#include "macros/junews/lostleptons/RatioError.h"
using namespace std;

RatioError::RatioError(TH1D* hPred) :
  prepared_(false),
  hPred_(hPred),
  nMHTBins_(hPred->GetNbinsX()),
  // hIH_((TH1D*)hPred_->Clone()),
  vhIHL_(vector<TH1D*>(nMHTBins_, (TH1D*)hPred_->Clone())),
  vNH_(vector<int>(nMHTBins_, 0)),
  // vSH_(vector<double>(nMHTBins_, 0.)),
  // IL_(0.),
  SL_(0.),
  veRLF_(vector<vector<double> >(nMHTBins_, vector<double>(0,0.))),
  vRHL_(vector<double>(nMHTBins_, 0.)),
  verrRHL_(vector<double>(nMHTBins_, 0.))
{ // constructor
  
}


void RatioError::FillMHTPDF(TH1D* DTTPDF, double ptW, double RLF, double eRLF, bool verbose) {
  int nbinsDTT = DTTPDF->GetNbinsX();
  TH1D* hNH = (TH1D*)hPred_->Clone(); // this is because we need the raw number of events contributiing to the integral for each MHT bin
  hNH->Reset("ICES");
  int ptW_bin =  hNH->FindBin(ptW)-1; // this is because we need the contribution to the integral from each event in each ptW (and/or njets/ht) bin
  if (ptW_bin==nMHTBins_) ptW_bin=nMHTBins_-1;
  if (verbose) printf("Filling MHT PDF for ptW = %3.3f (bin %d)...\n", ptW, ptW_bin);
  for (int i_dTT_bin(0); i_dTT_bin<nbinsDTT; i_dTT_bin++) {
    hPred_->Fill(ptW*DTTPDF->GetBinCenter(i_dTT_bin+1), RLF*DTTPDF->GetBinContent(i_dTT_bin+1)); // treating this as data (no lumi weights)
    // hIH_->Fill(ptW*DTTPDF->GetBinCenter(i_dTT_bin+1), DTTPDF->GetBinContent(i_dTT_bin+1)); // need integral for each bin from each event, factoring out RLF
    hNH->Fill(ptW*DTTPDF->GetBinCenter(i_dTT_bin+1), 1); // need which bins got the event
    if (ptW_bin>=0) vhIHL_[ptW_bin]->Fill(ptW*DTTPDF->GetBinCenter(i_dTT_bin+1), DTTPDF->GetBinContent(i_dTT_bin+1)); // need integral for each bin from each event, factoring out RLF, in each ptW (and/or njets/ht) bin
  }
  // which bins got the event?
  if (verbose) cout << "...computing errors..." << endl;
  for (int i_mht_bin(0); i_mht_bin<nMHTBins_-1; i_mht_bin++) {
    if (hNH->GetBinContent(i_mht_bin+1)>0) vNH_[i_mht_bin]++;
  }
  if (hNH->GetBinContent(nMHTBins_)>0||hNH->GetBinContent(nMHTBins_+1)>0) vNH_[nMHTBins_]++;
  delete hNH;

  // now check to see if we already have this RLF for the ptW bin or if we need to store it
  bool found_eRLF(false);
  if (verbose) cout << "...checking for eRLF..." << endl;
  if (ptW_bin>=0) {
    for (unsigned int iratio(0); iratio<veRLF_[ptW_bin].size(); iratio++) {
      if (eRLF==veRLF_[ptW_bin].at(iratio)) {
	found_eRLF=true;
	break;
      }
    }
    if (!found_eRLF) veRLF_[ptW_bin].push_back(eRLF);
  }
  if (verbose) cout << "...OK" << endl;
}

void RatioError::SetPredictionsAndErrors(bool verbose) {
  if (prepared_) {
    cout << "Error: I already set the uncertainties on this hist." << endl;
    return;
  }
  FixOverflow(hPred_);
  SL_=hPred_->GetBinContent(1);
  for (int i_mht_bin(0); i_mht_bin<nMHTBins_; i_mht_bin++) {
    double SH_ = hPred_->GetBinContent(i_mht_bin+1);
    vRHL_[i_mht_bin] = (SL_>0.) ? SH_/SL_ : 0.;
    double e2CSstats = (vNH_[i_mht_bin]>0.) ? pow((vRHL_[i_mht_bin]*vRHL_[i_mht_bin])/vNH_[i_mht_bin], 2.) : 0.;
    double e2RHL_LFsyst(0.);
    // now need to loop over all ptW bins that contributed to this RHL
    for (int i_ptW_bin(0); i_ptW_bin<nMHTBins_; i_ptW_bin++) {
      double IL = vhIHL_[i_ptW_bin]->GetBinContent(1);   
      double IH = vhIHL_[i_ptW_bin]->GetBinContent(i_mht_bin);   
      // uncertainty on RLF
      for (unsigned int iratio(0); iratio<veRLF_[i_ptW_bin].size(); iratio++) {
	double LFsyst2 = (SL_>0.) ? pow(veRLF_[i_ptW_bin].at(iratio)*veRLF_[i_ptW_bin].at(iratio)*(IH/SL_-(SH_*IL)/(SL_*SL_)), 2.) : 0.;
	e2RHL_LFsyst += LFsyst2;
      }
    }
    verrRHL_[i_mht_bin] = sqrt(e2CSstats+e2RHL_LFsyst);
    if (verbose) {
      printf("Bin %d: %3.3f +/- %3.3f\n", i_mht_bin+1, SH_, hPred_->GetBinError(i_mht_bin+1));
      printf("RHL %d: %3.3f +/- %3.3f\n", i_mht_bin+1, vRHL_[i_mht_bin], verrRHL_[i_mht_bin]);
    }
  }
  prepared_ = true;
}

TH1D* RatioError::GetPrediction() const {
  if (!prepared_) cout << "Warning: prediction not fully-prepared." << endl;
  return hPred_;
}

vector<double> RatioError::GetRHL() const{
  if (!prepared_) cout << "Warning: prediction not fully-prepared." << endl;
  return vRHL_;
}

vector<double> RatioError::GetRHLError() const{
  if (!prepared_) cout << "Warning: prediction not fully-prepared." << endl;
  return verrRHL_;
}
