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
#include "TGraph.h"
#include "my_style.hpp"


TGraph* getROCCurve(TH1D* hbg, TH1D* hsig, const int color=1000) {

  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos

  const unsigned int nbinsx(hsig->GetNbinsX());
  double sig[nbinsx], bg[nbinsx];
  for (unsigned int bin(0); bin<nbinsx+1; bin++) {
   sig[bin] = hsig->Integral(bin+1,nbinsx+1)/hsig->Integral(1,nbinsx+1);
   bg[bin] = 1-hbg->Integral(bin+1,nbinsx+1)/hbg->Integral(1,nbinsx+1);
   // bg[bin] = hbg->Integral(bin+1,nbinsx+1)/hbg->Integral(1,nbinsx+1);
  }

  TGraph* gr = new TGraph(nbinsx,bg,sig);
  gr->SetLineColor(color);
  gr->SetLineWidth(3);

  return gr;
}
