#include "TH1.h"
#include "TF1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TGraphErrors.h"
#include "TCut.h"
#include <iostream>
#include <iomanip>
#include "macros/include/my_style.hpp"



void ootpu_fit() {

  TChain* chain_7_0 = new TChain("reduced_tree");
  chain_7_0->Add("reduced_trees/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_PLS170_V7AN1-v1_MINIAODSIM_UCSB2281_v76/SMS-T2tt_2J_mStop-850_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_PLS170_V7AN1-v1_MINIAODSIM_UCSB2281_v76.root");
  TChain* chain_7_2 = new TChain("reduced_tree");
  chain_7_2->Add("reduced_trees/SMS-T2tt_13TeV_PHYS14_v73/SMS-T2tt_13TeV_PHYS14_v73.root");
  TChain* chain_50 = new TChain("reduced_tree");
  chain_50->Add("reduced_trees/SMS-T2tt_13TeV_50ns_v73/SMS-T2tt_13TeV_50ns_v73.root");

  TH1::SetDefaultSumw2();

  TH1F* hrelIso = new TH1F("hrelIso","", 30, 0, 0.5);
  TH1F* hOOTPU = new TH1F("hOOTPU","", 75, 0, 75);

  const int n = 12;
  Double_t x[n]  = {0,0,0,0,0,0,0,0,0};
  Double_t y[n]  = {0,0,0,0,0,0,0,0,0};
  Double_t ex[n] = {0,0,0,0,0,0,0,0,0};
  Double_t ey[n] = {0,0,0,0,0,0,0,0,0};

  hrelIso->StatOverflows(true);
  hOOTPU->StatOverflows(true);

  // TH1F* hOOTPUvsIso_7_0 = new TH1F("hOOTPUvsIso_7_0",";Out-of-time ints.;Isolation cut efficiency", 15, 0, 75);
  // TH1F* hOOTPUvsIso_7_2 = new TH1F("hOOTPUvsIso_7_2",";Out-of-time ints.;Isolation cut efficiency", 15, 0, 75);

  hrelIso->StatOverflows(true);
  
  int n1(0), n2(0);
  n1=15;
  n2=75;


  TString mu("muon_pt>10&&muon_true");
  int low = 20; 
  cout << "*****7_0*****" << endl;
  for (int bin(0); bin<9; bin++) {
    if (bin>0) low=low+5;
    TString cuts = Form("(%s)&&(oot_pu>=%d&&oot_pu<%d)",mu.Data(),low,low+5);
    cout << "Cuts: " << cuts.Data() << endl;
    chain_7_0->Project(hrelIso->GetName(), "muon_relIso", cuts);
    chain_7_0->Project(hOOTPU->GetName(), "oot_pu", cuts);
    hrelIso->Scale(1/hrelIso->GetEntries());
    hOOTPU->Scale(1/hrelIso->GetEntries());
    Double_t left(0.), lerror(0.), right(0.), rerror(0.);
    left = hrelIso->IntegralAndError(1,12,lerror);
    right = hrelIso->IntegralAndError(13,31,rerror);
    float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    y[bin]=left/(left+right);
    ey[bin]=rat_error;
    x[bin]=hOOTPU->GetMean();
    ex[bin]=hOOTPU->GetMeanError();
    printf("x: %3.2f +/- %3.3f\n", x[bin],ex[bin]);
    printf("y: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
 }
  TGraphErrors* gr_7_0 = new TGraphErrors(n,x,y,ex,ey);
  low = 20; 
  cout << "*****7_2*****" << endl;
  for (int bin(0); bin<9; bin++) {
    if (bin>0) low=low+5;
    TString cuts = Form("(%s)&&(oot_pu>=%d&&oot_pu<%d)",mu.Data(),low,low+5);
    cout << "Cuts: " << cuts.Data() << endl;
    chain_7_2->Project(hrelIso->GetName(), "muon_relIso", cuts);
    chain_7_2->Project(hOOTPU->GetName(), "oot_pu", cuts);
    hrelIso->Scale(1/hrelIso->GetEntries());
    hOOTPU->Scale(1/hrelIso->GetEntries());
    Double_t left(0.), lerror(0.), right(0.), rerror(0.);
    left = hrelIso->IntegralAndError(1,12,lerror);
    right = hrelIso->IntegralAndError(13,31,rerror);
    float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    y[bin]=left/(left+right);
    ey[bin]=rat_error;
    x[bin]=hOOTPU->GetMean();
    ex[bin]=hOOTPU->GetMeanError();
    printf("x: %3.2f +/- %3.3f\n", x[bin],ex[bin]);
    printf("y: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
 }
  TGraphErrors* gr_7_2 = new TGraphErrors(n,x,y,ex,ey);
  Double_t x_50[12]  = {0,0,0,0,0,0,0,0,0,0,0,0};
  Double_t y_50[12]  = {0,0,0,0,0,0,0,0,0,0,0,0};
  Double_t ex_50[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  Double_t ey_50[12] = {0,0,0,0,0,0,0,0,0,0,0,0};
  low = 40; 
  cout << "*****50 ns*****" << endl;
  for (int bin(0); bin<12; bin++) {
    if (bin>0) low=low+5;
    TString cuts = Form("(%s)&&(oot_pu>=%d&&oot_pu<%d)",mu.Data(),low,low+5);
    cout << "Cuts: " << cuts.Data() << endl;
    chain_50->Project(hrelIso->GetName(), "muon_relIso", cuts);
    chain_50->Project(hOOTPU->GetName(), "oot_pu", cuts);
    hrelIso->Scale(1/hrelIso->GetEntries());
    hOOTPU->Scale(1/hrelIso->GetEntries());
    Double_t left(0.), lerror(0.), right(0.), rerror(0.);
    left = hrelIso->IntegralAndError(1,12,lerror);
    right = hrelIso->IntegralAndError(13,31,rerror);
    float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    y_50[bin]=left/(left+right);
    ey_50[bin]=rat_error;
    x_50[bin]=hOOTPU->GetMean();
    ex_50[bin]=hOOTPU->GetMeanError();
    printf("x: %3.2f +/- %3.3f\n", x_50[bin],ex_50[bin]);
    printf("y: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
 }
  TGraphErrors* gr_50 = new TGraphErrors(12,x_50,y_50,ex_50,ey_50);
 
  gr_7_0->SetLineColor(1002);
  gr_7_0->SetMarkerColor(1002);
  gr_7_0->SetMarkerStyle(20);
  gr_7_2->SetLineColor(1005);
  gr_7_2->SetMarkerColor(1005);
  gr_7_2->SetMarkerStyle(21);
  gr_50->SetLineColor(1004);
  gr_50->SetMarkerColor(1004);
  gr_50->SetMarkerStyle(22);

  TCanvas* c1 = new TCanvas();
  TH1F* hdummy = new TH1F("hdummy",";Out-of-time interactions;Isolation cut efficiency (I_{rel}<0.2/I_{rel}>0.2)",110,0,110);
  hdummy->SetStats(0);
  hdummy->Draw();

  // do a linear fit
  TF1 *flin_7_0 = new TF1("flin_7_0", "1 ++ x", 20, 65);
  gr_7_0->Fit(flin_7_0);
  flin_7_0=gr_7_0->GetFunction("flin_7_0");
  flin_7_0->SetLineColor(1002);
  flin_7_0->SetLineWidth(2);

  // do a linear fit
  TF1 *flin_7_2 = new TF1("flin_7_2", "1 ++ x", 20, 65);
  gr_7_2->Fit(flin_7_2);
  flin_7_2=gr_7_2->GetFunction("flin_7_2");
  flin_7_2->SetLineColor(1005);
  flin_7_2->SetLineWidth(2);

  // do a linear fit
  TF1 *flin_50 = new TF1("flin_50", "1 ++ x", 40, 100);
  gr_50->Fit(flin_50);
  flin_50=gr_50->GetFunction("flin_50");
  flin_50->SetLineColor(1004);
  flin_50->SetLineWidth(2);

  gr_7_0->Draw("P");
  gr_7_2->Draw("P");
  gr_50->Draw("P");

  flin_7_0->SetRange(0,100);
  flin_7_2->SetRange(0,100);
  flin_50->SetRange(0,100);

  c1->Update();

  TString plotTitle ="macros/lost_leptons/plots/OOTPU_fit.pdf";
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
