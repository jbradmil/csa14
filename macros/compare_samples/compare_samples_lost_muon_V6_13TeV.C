#include "TH1.h"
#include "TFile.h"
#include "TChain.h"
#include "TCanvas.h"
#include "TStyle.h"
#include "TMath.h"
#include "TLegend.h"
#include "TCut.h"

void compare_samples_lost_muon(TString var, TCut cuts, TString title, int nbinsx, float low, float high, bool logscale=false, TString comments="") {

  TChain* chain8 = new TChain("reduced_tree");
  chain8->Add("../../reduced_trees/TTJets_MassiveBinDECAY_TuneZ2star_8TeV-madgraph-tauola_Summer12_DR53X-PU_S10_START53_V7A-v1_AODSIM_UCSB1850_v71.list.root");
  TChain* chain13 = new TChain("reduced_tree");
  chain13->Add("../../reduced_trees/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU_S14_POSTLS170_V6-v1-MINIAODSIM_10k.root");

  TH1F* h8 = new TH1F("h8",title, nbinsx, low, high);
  TH1F* h13 = new TH1F("h13",title, nbinsx, low, high);
  h8->SetStats(0);
  h8->GetYaxis()->SetLabelSize(0.04);
  //h8->GetYaxis()->SetTitleOffset(1.3);
  h8->GetXaxis()->SetTitleOffset(1.1);
  h8->GetXaxis()->SetTitleFont(132);
  h8->GetXaxis()->SetTitleSize(0.042);
  h8->GetXaxis()->SetLabelSize(0.04);
  h8->SetLineWidth(2);

  h13->SetStats(0);
  h13->SetLineColor(2);
  h13->SetLineWidth(2);

  cuts+="num_gen_muons==1&&gen_mu1.mus_match>=0";
  //  cuts+="gen_mu1.veto_muon<1";

  chain8->Project("h8", var, cuts);
  chain13->Project("h13", var, cuts);

  h8->Scale(1/h8->Integral());
  h13->Scale(1/h13->Integral());

  TCanvas* c1 = new TCanvas();
  if (logscale) c1->SetLogy();
  float max = TMath::Max(h8->GetMaximum(), h13->GetMaximum());
  h8->SetMaximum(max*1.1);
  h8->Draw("hist");
  h13->Draw("hist,same");

  TLegend* leg = new TLegend(0.75,0.8,0.9,0.9);
  leg->AddEntry(h8,"8 TeV","l");
  leg->AddEntry(h13,"13 TeV","l");
  leg->Draw();

  TString plotTitle = var+"lost_muon"+comments+"_compare8and13TeV_V6ttbar.pdf";
  c1->Print(plotTitle);
}
