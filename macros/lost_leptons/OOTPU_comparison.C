#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"
#include <iostream>
#include <fstream>
#include <iomanip>

void ootpu_comparison(TString files, TString var, TString addCuts, TString title, int nbins, float low, float high, int cut_bin=1, TString comments="", bool leq=false) {

  TChain* chain = new TChain("reduced_tree");
  chain->Add(files);

  TH1::SetDefaultSumw2();

  TH1F* hA = new TH1F("hA",title, nbins, low, high);
  TH1F* hB = new TH1F("hB",title, nbins, low, high);
  TH1F* hC = new TH1F("hC",title, nbins, low, high);
  TH1F* hD = new TH1F("hD",title, nbins, low, high);
  hA->SetStats(0);
  hA->GetYaxis()->SetLabelSize(0.04);
  //hA->GetYaxis()->SetTitleOffset(1.3);
  hA->GetXaxis()->SetTitleOffset(1.1);
  hA->GetXaxis()->SetTitleFont(132);
  hA->GetXaxis()->SetTitleSize(0.042);
  hA->GetXaxis()->SetLabelSize(0.04);
  hA->SetLineWidth(2);

  hA->StatOverflows(true);
  hB->StatOverflows(true);
  hC->StatOverflows(true);
  hD->StatOverflows(true);

  int n1(0), n2(0), n3(0), n4(0), n5(0);
  if (files.Contains("20bx25")) {
    n1=20;
    n2=35;
    n3=40;
    n4=45;
    n5=60;
  }
  else if (files.Contains("S14")) {
    n1=20;
    n2=60;
    n3=75;
    n4=85;
    n5=120;
  }
  else { // default: 8 TeV scenario
    n1=0;
    n2=30;
    n3=40;
    n4=60;
    n5=140;
  }


  // TString mu("num_gen_muons==1&&muon_reco_match>=0");
  //if (files.Contains("PU_S10")) {
  TString addCuts_form("");
  if (addCuts.Length()>0) addCuts_form = Form("(%s)&&",addCuts.Data());
  TString cuts = Form("%s(oot_pu>=%d&&oot_pu<%d)",addCuts_form.Data(),n1,n2);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hA", var, cuts);
  cuts = Form("%s(oot_pu>=%d&&oot_pu<%d)",addCuts_form.Data(),n2,n3);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hB", var, cuts);
  cuts = Form("%s(oot_pu>=%d&&oot_pu<%d)",addCuts_form.Data(),n3,n4);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hC", var, cuts);
  cuts = Form("%s(oot_pu>=%d)",addCuts_form.Data(),n4);
  cout << "Cuts: " << cuts.Data() << endl;
  chain->Project("hD", var, cuts);
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

  TCanvas* c1 = new TCanvas();
  float max = TMath::Max(hA->GetMaximum(), hB->GetMaximum());
  if (hC->GetMaximum()>max) max = hC->GetMaximum();
  if (hD->GetMaximum()>max) max = hD->GetMaximum();

  hA->SetMaximum(max*1.1);
  hA->Draw("e1");
  hB->Draw("e1,same");
  hC->Draw("e1,same");
  hD->Draw("e1,same");

  TLegend* leg = new TLegend(0.42,0.6,0.9,0.9);
  leg->SetFillStyle(0);
  char label[1000];
  sprintf(label,"%d#leqInts.<%d (#mu=%3.3f)",n1,n2,avg1);
  leg->AddEntry(hA,label,"lp");
  sprintf(label,"%d#leqInts.<%d (#mu=%3.3f)",n2,n3,avg2);
  leg->AddEntry(hB,label,"lp");
  sprintf(label,"%d#leqInts.<%d (#mu=%3.3f)",n3,n4,avg3);
  leg->AddEntry(hC,label,"lp");
  sprintf(label,"Ints.>%d (#mu=%3.3f)",n4,avg4);
  leg->AddEntry(hD,label,"lp");
  leg->Draw();

  TString plotTitle ="relIso_vs_OOTPU_"+var+comments;
  c1->Print(plotTitle+".pdf");

  std::ofstream ofs;
  ofs.open(plotTitle+".txt", std::ofstream::out);
  char header[1000];
  TString dir(">");
  if (leq) dir="<";
  sprintf(header,"Cut efficiency (%s%s%3.2f): ",var.Data(),dir.Data(),hA->GetBinLowEdge(cut_bin+1));
  cout << header << endl;
  ofs << header << endl;
  Double_t left(0.), lerror(0.), right(0.), rerror(0.);
  left = hA->IntegralAndError(1,cut_bin,lerror);
  right = hA->IntegralAndError(cut_bin+1,nbins+1,rerror);
  float rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  char line[1000];
  if (leq) sprintf(line,"bin1: %3.3f +/- %3.3f", left/(left+right),rat_error);
  else sprintf(line,"bin1: %3.3f +/- %3.3f", right/(left+right),rat_error);
  cout << line << endl;
  ofs << line << endl;
  left = hB->IntegralAndError(1,cut_bin,lerror);
  right = hB->IntegralAndError(cut_bin+1,nbins+1,rerror);
  rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  if (leq) sprintf(line,"bin2: %3.3f +/- %3.3f", left/(left+right),rat_error);
  else sprintf(line,"bin2: %3.3f +/- %3.3f", right/(left+right),rat_error);
  cout << line << endl;
  ofs << line << endl;  left = hC->IntegralAndError(1,cut_bin,lerror);
  right = hC->IntegralAndError(cut_bin+1,nbins+1,rerror);
  rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  if (leq) sprintf(line,"bin3: %3.3f +/- %3.3f", left/(left+right),rat_error);
  else sprintf(line,"bin3: %3.3f +/- %3.3f", right/(left+right),rat_error);
  cout << line << endl;
  ofs << line << endl;  left = hD->IntegralAndError(1,cut_bin,lerror);
  right = hD->IntegralAndError(cut_bin+1,nbins+1,rerror);
  rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
  if (leq) sprintf(line,"bin4: %3.3f +/- %3.3f", left/(left+right),rat_error);
  else sprintf(line,"bin4: %3.3f +/- %3.3f", right/(left+right),rat_error);
  cout << line << endl;
  ofs << line << endl;
  ofs.close();
}
