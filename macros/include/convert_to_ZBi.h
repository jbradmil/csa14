#include <iostream>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "TH1.h"
#include "TH2.h"
#include "../include/my_style.hpp"
#include "RooStats/NumberCountingUtils.h"



void convert_to_ZBi_2D(TH2D* h2ZBi, TH2D* h2, TH2D* h2BG, int nbinsx, double xlow, double xhigh, int nbinsy, double ylow, double yhigh) {
  h2ZBi->Reset();
  TH2D* h2int = new TH2D("h2int","",    nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  TH2D* h2BGint = new TH2D("h2BGint","",nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  for (int x(0); x<nbinsx; x++) {
    for (int y(0); y<nbinsy; y++) {
      h2int->SetBinContent(x+1,y+1, h2->Integral(x+1,1000,y+1,1000));
      Double_t e_SM(0.), n_SM(0.);
      n_SM=h2BG->IntegralAndError(x+1,1000,y+1,1000,e_SM);
      double fstat_bg = e_SM/n_SM;
      double fsyst_bg=0.2;
      double ferr_bg = TMath::Sqrt(fstat_bg*fstat_bg+fsyst_bg*fsyst_bg);
      h2BGint->SetBinContent(x+1,y+1, n_SM);
      if (h2int->GetBinContent(x+1,y+1)>=3.0) h2ZBi->SetBinContent(x+1,y+1, RooStats::NumberCountingUtils::BinomialObsZ(h2int->GetBinContent(x+1,y+1)+n_SM,n_SM,ferr_bg));
    }
  }
  h2ZBi->SetMaximum(5);
  h2ZBi->SetMinimum(0);

  delete h2int;
  delete h2BGint;
}

void convert_to_ZBi_with_tau_2D(TH2D* h2ZBi, TH2D* h2, TH2D* h2BG, int nbinsx, double xlow, double xhigh, int nbinsy, double ylow, double yhigh, double tau=1.0) {
  h2ZBi->Reset();
  TH2D* h2int = new TH2D("h2int","",    nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  TH2D* h2BGint = new TH2D("h2BGint","",nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  for (int x(0); x<nbinsx; x++) {
    for (int y(0); y<nbinsy; y++) {
      h2int->SetBinContent(x+1,y+1, h2->Integral(x+1,1000,y+1,1000));
      Double_t e_SM(0.), n_SM(0.);
      n_SM=h2BG->IntegralAndError(x+1,1000,y+1,1000,e_SM);
      h2BGint->SetBinContent(x+1,y+1, n_SM);
      if (h2int->GetBinContent(x+1,y+1)>=3.0) h2ZBi->SetBinContent(x+1,y+1, RooStats::NumberCountingUtils::BinomialWithTauExpZ(h2int->GetBinContent(x+1,y+1),n_SM,tau)); // tau * Search = SB
    }
  }

  // if (tau>=5) h2ZBi->SetMaximum(10);
  h2ZBi->SetMaximum(5);
  h2ZBi->SetMinimum(0);

  delete h2int;
  delete h2BGint;
}
