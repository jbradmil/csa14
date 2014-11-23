#include <iostream>
#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"
#include "../include/my_style.hpp"

using namespace std;

void compare_T1tttt_samples(TString var, TString xtitle, int nbinsx, float low, float high, bool T1tttt=true, TCut cuts="", TString comments="", double forceMax=-1., bool logscale=false) {

  TH1::SetDefaultSumw2();
  TChain* chain7_0_50 = new TChain("reduced_tree");
  TChain* chain7_0_25 = new TChain("reduced_tree");
  TChain* chain7_2_25 = new TChain("reduced_tree");
  TChain* chain5_3_50 = new TChain("reduced_tree");
  if (T1tttt) {
    chain7_0_50->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_duanders-Spring14dr-PU_S14_POSTLS170_V6AN1-miniAOD706p1-814812ec83fce2f620905d2bb30e9100_USER_UCSB2269_v76/*.root");
    chain7_0_25->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/*.root");
    chain7_2_25->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2194_v73/*.root");
  } else {
    chain7_0_25->Add("reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2208_v76/*.root");
    chain7_0_50->Add("reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU_S14_POSTLS170_V6-v1_MINIAODSIM_UCSB2265_v75/*root");
    chain7_2_25->Add("reduced_trees/RelValTTbar_13TeV-MINIAODSIM-PU25ns_PHYS14_25_V1_Phys14-v1_v73/RelValTTbar_13TeV-MINIAODSIM-PU25ns_PHYS14_25_V1_Phys14-v1_v73.root");
  }
    chain5_3_50->Add("reduced_trees/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1850_v71/*root");


  TH1F* h7_0_50 = new TH1F("h7_0_50",";"+xtitle+";(a.u.)", nbinsx, low, high);
  TH1F* h7_0_25 = new TH1F("h7_0_25",";"+xtitle+";(a.u.)", nbinsx, low, high);
  TH1F* h7_2_25 = new TH1F("h7_2_25",";"+xtitle+";(a.u.)", nbinsx, low, high);
  TH1F* h5_3_50 = new TH1F("h5_3_50",";"+xtitle+";(a.u.)", nbinsx, low, high);
  h7_0_50->SetStats(0);
  h7_0_50->GetYaxis()->SetLabelSize(0.04);
  //h7_0_50->GetYaxis()->SetTitleOffset(1.3);
  h7_0_50->GetXaxis()->SetTitleOffset(1.1);
  h7_0_50->GetXaxis()->SetTitleFont(132);
  h7_0_50->GetXaxis()->SetTitleSize(0.042);
  h7_0_50->GetXaxis()->SetLabelSize(0.04);
  h7_0_50->SetLineWidth(3);
  h7_0_50->SetLineColor(1004);

  h7_0_25->SetStats(0);
  h7_0_25->SetLineColor(1002);
  h7_0_25->SetLineWidth(3);

  h7_2_25->SetStats(0);
  h7_2_25->SetLineColor(1003);
  h7_2_25->SetLineWidth(3);

  h5_3_50->SetStats(0);
  h5_3_50->SetLineColor(1000);
  h5_3_50->SetLineWidth(3);

  // h7_0_50->StatOverflows(true);
  // h7_0_25->StatOverflows(true);
  // h7_2_25->StatOverflows(true);
  // h5_3_50->StatOverflows(true);

  chain7_0_50->Project("h7_0_50", var, cuts);
  chain7_0_25->Project("h7_0_25", var, cuts);
  chain7_2_25->Project("h7_2_25", var, cuts);
  chain5_3_50->Project("h5_3_50", var, cuts);


  Double_t i_overflow(0.), e_overflow(0.);
  i_overflow=h7_0_50->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
  h7_0_50->SetBinContent(nbinsx, i_overflow);
  h7_0_50->SetBinError(nbinsx, e_overflow);
  i_overflow=h7_0_25->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
  h7_0_25->SetBinContent(nbinsx, i_overflow);
  h7_0_25->SetBinError(nbinsx, e_overflow);
  i_overflow=h7_2_25->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
  h7_2_25->SetBinContent(nbinsx, i_overflow);
  h7_2_25->SetBinError(nbinsx, e_overflow);
  i_overflow=h5_3_50->IntegralAndError(nbinsx,nbinsx+1,e_overflow);
  h5_3_50->SetBinContent(nbinsx, i_overflow);
  h5_3_50->SetBinError(nbinsx, e_overflow);


  h7_0_50->Scale(1/h7_0_50->Integral(1,nbinsx));
  h7_0_25->Scale(1/h7_0_25->Integral(1,nbinsx));
  h7_2_25->Scale(1/h7_2_25->Integral(1,nbinsx));
  h5_3_50->Scale(1/h5_3_50->Integral(1,nbinsx));

  if (var.Contains("relIso")) { // efficiency
    char header[1000];
    TString dir(">");
    bool leq=true;
    int cut_bin=6;
    if (leq) dir="<";
    printf(header,"Cut efficiency (%s%s%3.2f): ",var.Data(),dir.Data(),h7_0_50->GetBinLowEdge(cut_bin+1));
    cout << header << endl;
    Double_t left(0.), lerror(0.), right(0.), rerror(0.), rat_error(0.);
    left = h7_2_25->IntegralAndError(1,cut_bin,lerror);
    right = h7_2_25->IntegralAndError(cut_bin+1,nbinsx,rerror);
    rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    printf("7_2_25: %3.3f +/- %3.3f\n", left/(left+right),rat_error);
    left = h7_0_50->IntegralAndError(1,cut_bin,lerror);
    right = h7_0_50->IntegralAndError(cut_bin+1,nbinsx,rerror);
    rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    printf("7_0_50: %3.3f +/- %3.3f\n", left/(left+right),rat_error);
    left = h7_0_25->IntegralAndError(1,cut_bin,lerror);
    right = h7_0_25->IntegralAndError(cut_bin+1,nbinsx,rerror);
    rat_error=sqrt((left*left*rerror*rerror+right*right*lerror*lerror)/((left+right)*(left+right)));
    printf("7_0_25: %3.3f +/- %3.3f\n", left/(left+right),rat_error);
  }

  TCanvas* c1 = new TCanvas();
  if (logscale) c1->SetLogy();
  float max = TMath::Max(h7_0_50->GetMaximum(), h7_0_25->GetMaximum());
  if (max<h7_2_25->GetMaximum()) max=h7_2_25->GetMaximum();
  if (max<h5_3_50->GetMaximum()) max=h5_3_50->GetMaximum();
  if (forceMax>0) h7_0_50->SetMaximum(forceMax);
  else h7_0_50->SetMaximum(max*1.3);
  h7_0_50->SetMinimum(0);
  h7_0_50->Draw("hist");
  h7_0_25->Draw("hist,same");
  h7_2_25->Draw("hist,same");
  if (!T1tttt) h5_3_50->Draw("hist,same");

  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->SetFillStyle(0);
  char label[100];
  if (!T1tttt) {
    sprintf(label,"50 ns, 5_3_X (#mu=%3.2f)",h5_3_50->GetMean());
    leg->AddEntry(h5_3_50,label,"l");
  }
  sprintf(label,"50 ns, 7_0_X (#mu=%3.2f)",h7_0_50->GetMean());
  leg->AddEntry(h7_0_50,label,"l");
  sprintf(label,"25 ns, 7_0_X (#mu=%3.2f)",h7_0_25->GetMean());
  leg->AddEntry(h7_0_25,label,"l");
  sprintf(label,"25 ns, 7_2_X (#mu=%3.2f)",h7_2_25->GetMean());
  leg->AddEntry(h7_2_25,label,"l");
  leg->Draw();

  TString tail;
  if (T1tttt) tail="_compare_T1tttt.pdf";
  else tail="_compare_ttbar.pdf";
  TString plotTitle = "macros/lost_leptons/plots/OOTPU/"+var;
  if (comments.Length()>0) plotTitle+=("_"+comments+"-");
  plotTitle+=tail;
  c1->Print(plotTitle);
}
