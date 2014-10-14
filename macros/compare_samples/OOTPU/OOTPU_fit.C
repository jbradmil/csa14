#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>
#include <iomanip>

void ootpu_fit(TString files, TString comments="") {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1::SetDefaultSumw2();

  TH1F* hrelIso = new TH1F("hrelIso","", 30, 0, 0.5);
  TH1F* hOOTPU = new TH1F("hOOTPU","", 75, 0, 75);
  TH1F* hOOTPUvsIso = new TH1F("hOOTPUvsIso",";Out-of-time ints.;Isolation cut efficiency", 15, 0, 75);

  hrelIso->StatOverflows(true);
  
  int n1(0), n2(0);
  if (files.Contains("20bx25")) {
    n1=15;
    n2=75;
  }
  else if (files.Contains("S14")) {
    n1=0;
    n2=120;
  }
  else { // default: 8 TeV scenario
    n1=0;
    n2=70;
  }


  TString mu("num_gen_muons==1&&muon_reco_match>=0");
  int low = 15; 
  for (int bin(1); bin<hOOTPUvsIso->GetNbinsX()+1; bin++) {
    if (bin<4) continue;
    if (bin>4) low=low+5;
    if (low>hOOTPUvsIso->GetBinLowEdge(hOOTPUvsIso->GetNbinsX())) break;
    TString cuts = Form("(%s)&&(oot_pu>=%d&&oot_pu<%d)",mu.Data(),low,low+4);
    cout << "Cuts: " << cuts.Data() << endl;
    chain->Project(hrelIso->GetName(), "muon_relIso", cuts);
    chain->Project(hOOTPU->GetName(), "oot_pu", cuts);
    hrelIso->Scale(1/hrelIso->GetEntries());
    Double_t left(0.), lerror(0.), right(0.), rerror(0.);
    left = hrelIso->IntegralAndError(1,12,lerror);
    right = hrelIso->IntegralAndError(13,31,rerror);
    float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    printf("bin1: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
    hOOTPUvsIso->SetBinContent(bin,left/(left+right));
    hOOTPUvsIso->SetBinError(bin,rat_error);
  }
  
  hOOTPUvsIso->SetLineColor(1);
  hOOTPUvsIso->SetMarkerColor(1);
  hOOTPUvsIso->SetMarkerStyle(20);

  TCanvas* c1 = new TCanvas();

  hOOTPUvsIso->Draw("e1");

  // do a linear fit
  TF1 *flin = new TF1("flin", "1 ++ x", 15, 75);
  hOOTPUvsIso->Fit(flin);
  flin=hOOTPUvsIso->GetFunction("flin");
  flin->SetLineColor(kBlue);
  flin->SetLineWidth(2);

  TString plotTitle ="OOTPU_fit"+comments+".pdf";
  c1->Print(plotTitle);

  // cout << "Rejection rates" << endl;
  // Double_t left(0.), lerror(0.), right(0.), rerror(0.);
  // left = hA->IntegralAndError(1,12,lerror);
  // right = hA->IntegralAndError(13,31,rerror);
  // float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  // printf("bin1: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
  // left = hB->IntegralAndError(1,12,lerror);
  // right = hB->IntegralAndError(13,31,rerror);
  // rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  // printf("bin2: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
  // left = hC->IntegralAndError(1,12,lerror);
  // right = hC->IntegralAndError(13,31,rerror);
  // rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  // printf("bin3: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
  // left = hD->IntegralAndError(1,12,lerror);
  // right = hD->IntegralAndError(13,31,rerror);
  // rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  // printf("bin4: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
}
