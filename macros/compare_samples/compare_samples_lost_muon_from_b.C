#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"

void compare_samples(TString var, TCut cuts, TString title, int nbinsx, float low, float high, bool logscale=false, TString comments="") {

  TChain* chain8 = new TChain("reduced_tree");
  chain8->Add("../../reduced_trees/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1850_v71_mu_from_b.list.root");
  TChain* chain13_25 = new TChain("reduced_tree");
  chain13_25->Add("../../reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v2-MINIAODSIM_mu_from_b.list.root");
  TChain* chain13_50 = new TChain("reduced_tree");
  chain13_50->Add("../../reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU_S14_POSTLS170_V6-v1_cfA_v72_mu_from_b.list.root");

  TH1F* h8 = new TH1F("h8",title, nbinsx, low, high);
  TH1F* h13_25 = new TH1F("h13_25",title, nbinsx, low, high);
  TH1F* h13_50 = new TH1F("h13_50",title, nbinsx, low, high);
  h8->SetStats(0);
  h8->GetYaxis()->SetLabelSize(0.04);
  //h8->GetYaxis()->SetTitleOffset(1.3);
  h8->GetXaxis()->SetTitleOffset(1.1);
  h8->GetXaxis()->SetTitleFont(132);
  h8->GetXaxis()->SetTitleSize(0.042);
  h8->GetXaxis()->SetLabelSize(0.04);
  h8->SetLineWidth(2);

  h13_25->SetStats(0);
  h13_25->SetLineColor(2);
  h13_25->SetLineWidth(2);

  h13_50->SetStats(0);
  h13_50->SetLineColor(8);
  h13_50->SetLineWidth(2);

  h8->StatOverflows(true);
  h13_25->StatOverflows(true);
  h13_50->StatOverflows(true);

  chain8->Project("h8", var, cuts);
  chain13_25->Project("h13_25", var, cuts);
  chain13_50->Project("h13_50", var, cuts);

  h8->Scale(1/h8->Integral());
  h13_25->Scale(1/h13_25->Integral());
  h13_50->Scale(1/h13_50->Integral());

  TCanvas* c1 = new TCanvas();
  if (logscale) c1->SetLogy();
  float max = TMath::Max(h8->GetMaximum(), h13_25->GetMaximum());
  if (max<h13_50->GetMaximum()) max=h13_50->GetMaximum();
  h8->SetMaximum(max*1.1);
  h8->Draw("hist");
  h13_25->Draw("hist,same");
  h13_50->Draw("hist,same");

  TLegend* leg = new TLegend(0.6,0.7,0.9,0.9);
  leg->SetFillStyle(0);
  char label[100];
  sprintf(label,"8 TeV (#mu=%3.2f)",h8->GetMean());
  leg->AddEntry(h8,label,"l");
  sprintf(label,"13 TeV, 25 ns (#mu=%3.2f)",h13_25->GetMean());
  leg->AddEntry(h13_25,label,"l");
  sprintf(label,"13 TeV, 50 ns (#mu=%3.2f)",h13_50->GetMean());
  leg->AddEntry(h13_50,label,"l");
  leg->Draw();

  TString plotTitle = var+"_"+comments+"compare8and13TeV_mu_from_b.pdf";
  c1->Print(plotTitle);
}
