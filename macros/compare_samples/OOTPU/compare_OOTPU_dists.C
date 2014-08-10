#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>
#include <iomanip>

void ootpu_comparison(TString files, TString var, TString title, int nbins, float low, float high, TString comments="") {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1::SetDefaultSumw2();

  TH1F* hA = new TH1F("hA",title, nbins, low, high);
  TH1F* hB = new TH1F("hB",title, nbins, low, high);
  TH1F* hC = new TH1F("hC",title, nbins, low, high);
  TH1F* hD = new TH1F("hD",title, nbins, low, high);

  TH1F* hA_l = new TH1F("hA_l",title, nbins, low, high);
  TH1F* hB_l = new TH1F("hB_l",title, nbins, low, high);
  TH1F* hC_l = new TH1F("hC_l",title, nbins, low, high);
  TH1F* hD_l = new TH1F("hD_l",title, nbins, low, high);
  hA->SetStats(0);
  hA->GetYaxis()->SetLabelSize(0.04);
  //hA->GetYaxis()->SetTitleOffset(1.3);
  hA->GetXaxis()->SetTitleOffset(1.1);
  hA->GetXaxis()->SetTitleFont(132);
  hA->GetXaxis()->SetTitleSize(0.042);
  hA->GetXaxis()->SetLabelSize(0.04);
  hA->SetLineWidth(2);

  //  hA->StatOverflows(true);
  //  hB->StatOverflows(true);
  //  hC->StatOverflows(true);
  //  hD->StatOverflows(true);

  int n1(0), n2(0), n3(0), n4(0), n5(0);
  if (files.Contains("20bx25")) {
    n1=5;
    n2=17;
    n3=21;
    n4=25;
    n5=40;
  }
  else if (files.Contains("S14")) {
    n1=0;
    n2=25;
    n3=40;
    n4=55;
    n5=120;
  }
  else { // default: 8 TeV scenario
    n1=0;
    n2=15;
    n3=22;
    n4=32;
    n5=70;
  }


  TString mu("num_gen_muons==1&&muon_reco_match>=0");
  //if (files.Contains("PU_S10")) {
  TString cuts = Form("(%s)&&(eoot_pu>=%d&&eoot_pu<%d)",mu.Data(),n1,n2);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hA", var, cuts);
  cuts = Form("(%s)&&(loot_pu>=%d&&loot_pu<%d)",mu.Data(),n1,n2);
  chain->Project("hA_l", var, cuts);
  cuts = Form("(%s)&&(eoot_pu>=%d&&eoot_pu<%d)",mu.Data(),n2,n3);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hB", var, cuts);
  cuts = Form("(%s)&&(loot_pu>=%d&&loot_pu<%d)",mu.Data(),n2,n3);
  chain->Project("hB_l", var, cuts);
  cuts = Form("(%s)&&(eoot_pu>=%d&&eoot_pu<%d)",mu.Data(),n3,n4);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hC", var, cuts);
  cuts = Form("(%s)&&(loot_pu>=%d&&loot_pu<%d)",mu.Data(),n3,n4);
  chain->Project("hC_l", var, cuts);
  cuts = Form("(%s)&&(eoot_pu>=%d)",mu.Data(),n4);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hD", var, cuts);
  cuts = Form("(%s)&&(loot_pu>=%d)",mu.Data(),n4);
  chain->Project("hD_l", var, cuts);
  // }
  // else {
  //  }
  
  float avg1(hA->GetMean());
  float avg2(hB->GetMean());
  float avg3(hC->GetMean());
  float avg4(hD->GetMean());

  hA->Scale(1/hA->Integral());
  hB->Scale(1/hB->Integral());
  hC->Scale(1/hC->Integral());
  hD->Scale(1/hD->Integral());

  hA->SetLineColor(1);
  hB->SetLineColor(2);
  hC->SetLineColor(3);
  hD->SetLineColor(4);

  hA->SetLineWidth(2);
  hB->SetLineWidth(2);
  hC->SetLineWidth(2);
  hD->SetLineWidth(2);

  float avg1_l(hA_l->GetMean());
  float avg2_l(hB_l->GetMean());
  float avg3_l(hC_l->GetMean());
  float avg4_l(hD_l->GetMean());

  hA_l->Scale(1/hA_l->Integral());
  hB_l->Scale(1/hB_l->Integral());
  hC_l->Scale(1/hC_l->Integral());
  hD_l->Scale(1/hD_l->Integral());

  hA_l->SetLineColor(12);
  hB_l->SetLineColor(46);
  hC_l->SetLineColor(8);
  hD_l->SetLineColor(7);

  hA_l->SetLineWidth(2);
  hB_l->SetLineWidth(2);
  hC_l->SetLineWidth(2);
  hD_l->SetLineWidth(2);

  TCanvas* c1 = new TCanvas();
  float max = TMath::Max(hA->GetMaximum(), hB->GetMaximum());
  if (hC->GetMaximum()>max) max = hC->GetMaximum();
  if (hD->GetMaximum()>max) max = hD->GetMaximum();

  hA->SetMaximum(max*1.1);
  hA->Draw("hist");
  hB->Draw("hist,same");
  hC->Draw("hist,same");
  hD->Draw("hist,same");
  hA_l->Draw("hist,same");
  hB_l->Draw("hist,same");
  hC_l->Draw("hist,same");
  hD_l->Draw("hist,same");

  TLegend* leg = new TLegend(0.42,0.6,0.9,0.9);
  leg->SetFillStyle(0);
  char label[1000];
  sprintf(label,"%d#leqEarly Ints.<%d (#mu=%3.3f)",n1,n2,avg1);
  leg->AddEntry(hA,label,"lp");
  sprintf(label,"%d#leqEarly Ints.<%d (#mu=%3.3f)",n2,n3,avg2);
  leg->AddEntry(hB,label,"lp");
  sprintf(label,"%d#leqEarly Ints.<%d (#mu=%3.3f)",n3,n4,avg3);
  leg->AddEntry(hC,label,"lp");
  sprintf(label,"Early Ints.>%d (#mu=%3.3f)",n4,avg4);
  leg->AddEntry(hD,label,"lp");
  // leg->Draw();

  TString plotTitle ="relIso_vs_early_and_late_OOTPU_"+var+comments+".pdf";
  c1->Print(plotTitle);

  cout << "Rejection rates" << endl;
  Double_t left(0.), lerror(0.), right(0.), rerror(0.);
  left = hA->IntegralAndError(1,12,lerror);
  right = hA->IntegralAndError(13,31,rerror);
  float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  printf("bin1: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
  left = hB->IntegralAndError(1,12,lerror);
  right = hB->IntegralAndError(13,31,rerror);
  rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  printf("bin2: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
  left = hC->IntegralAndError(1,12,lerror);
  right = hC->IntegralAndError(13,31,rerror);
  rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  printf("bin3: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
  left = hD->IntegralAndError(1,12,lerror);
  right = hD->IntegralAndError(13,31,rerror);
  rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  printf("bin4: %3.2f +/- %3.3f\n", left/(left+right),rat_error);
}
