#include <TH1.h>
#include <TH2.h>
#include <TH3.h>
#include <THStack.h>
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TFile.h"
#include "TCut.h"
#include "TChain.h"
#include "TSystem.h"
#include "TCanvas.h"
#include "TLegend.h"
#include "macros/include/my_style.hpp"
#include "macros/junews/lostleptons/utils.h"

void plot_ptW_vs_triggers(const TString treestring = "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", const TString plotNote="ttbar_1mu", bool logy = true) {

  
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  TH1::SetDefaultSumw2(1);
  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);

  const int nbinsx(12); // ptW
  double binsx[nbinsx+1] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 500, 600, 750, 1050};
  TH1D* hAll = new TH1D("hAll", ";Reconstructed p_{T}^{W} [GeV];Fraction of all 1-e/#mu events", nbinsx, binsx);
  TH1D* hHT350MET120 = new TH1D("hHT350MET120", ";Reconstructed p_{T}^{W} [GeV];Fraction of all 1-e/#mu events", nbinsx, binsx);
  TH1D* hHT900 = new TH1D("hHT900", ";Reconstructed p_{T}^{W} [GeV];Fraction of all 1-e/#mu events", nbinsx, binsx);
  TH1D* hL15HT600 = new TH1D("hL15HT600", ";Reconstructed p_{T}^{W} [GeV];Fraction of all 1-e/#mu events", nbinsx, binsx);
  TH1D* hL15HT400MET70 = new TH1D("hL15HT400MET70", ";Reconstructed p_{T}^{W} [GeV];Fraction of all 1-e/#mu events", nbinsx, binsx);
  hAll->SetLineWidth(3);
  hAll->SetLineStyle(1);
  hAll->SetLineColor(1);
  hHT350MET120->SetLineWidth(3);
  hHT350MET120->SetLineColor(1000);
  hHT350MET120->SetLineStyle(9);
  hHT900->SetLineWidth(3);
  hHT900->SetLineStyle(7);
  hHT900->SetLineColor(1004);
  hL15HT600->SetLineWidth(3);
  hL15HT600->SetLineColor(1002);
  hL15HT600->SetLineStyle(2);
  hL15HT400MET70->SetLineWidth(3);
  hL15HT400MET70->SetLineColor(1003);
  hL15HT400MET70->SetLineStyle(3);

  TCut SL = "num_true_mus==1&&num_true_els+num_true_had_taus==0";
  TCut baseline = "num_jets_pt30>=4&&min_delta_phi_met_N>6";
  //TCut baseline = "num_jets_pt30>=4";
  TCut weight = "weightppb*10000";
  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  pypu->Project("hAll", "true_mu_ptW", (SL+baseline+"true_mu_pt>0")*weight);
  pypu->Project("hHT350MET120", "mu_ptW", (SL+baseline+"mu_pt>10&&ht30>500&&mht30>200")*weight);
  pypu->Project("hHT900", "mu_ptW", (SL+baseline+"mu_pt>10&&ht30>1000")*weight);
  pypu->Project("hL15HT600", "mu_ptW", (SL+baseline+"mu_pt>20&&ht30>750")*weight);
  pypu->Project("hL15HT400MET70", "mu_ptW", (SL+baseline+"mu_pt>20&&ht30>550&&mht30>125")*weight);

  TLegend * leg1 = new TLegend(0.2, 0.7, 0.8, 0.9);
  set_style(leg1,0.025);
  leg1->AddEntry(hAll,"All 1-e/#mu events (gen lepton)","l");
  leg1->AddEntry(hHT350MET120,"H_{T} > 500 GeV, H_{T}^{miss} > 200 GeV, p_{T}^{e/#mu} > 10 GeV","l");
  leg1->AddEntry(hHT900,"H_{T} > 1000 GeV, p_{T}^{e/#mu} > 10 GeV","l");
  leg1->AddEntry(hL15HT600,"H_{T} > 750 GeV, p_{T}^{e/#mu} > 20 GeV","l");
  leg1->AddEntry(hL15HT400MET70,"H_{T} > 550 GeV, H_{T}^{miss} > 125 GeV, p_{T}^{e/#mu} > 20 GeV","l");
  leg1->Draw();

  FixOverflow(hAll);
  FixOverflow(hHT350MET120);
  FixOverflow(hHT900);
  FixOverflow(hL15HT600);
  FixOverflow(hL15HT400MET70);
  double norm = hAll->Integral(1, nbinsx);
  hAll->Scale(1/norm);
  hHT350MET120->Scale(1/norm);
  hHT900->Scale(1/norm);
  hL15HT600->Scale(1/norm);
  hL15HT400MET70->Scale(1/norm);
  
  double max = hAll->GetMaximum();
  if (hHT350MET120->GetMaximum()>max) max = hHT350MET120->GetMaximum();
  if (hHT900->GetMaximum()>max) max = hHT900->GetMaximum();
  if (hL15HT600->GetMaximum()>max) max = hL15HT600->GetMaximum();
  if (hL15HT400MET70->GetMaximum()>max) max = hL15HT400MET70->GetMaximum();

  if (logy) {
    hAll->SetMaximum(30);
    hAll->SetMinimum(0.00005);
  } else {
    hAll->SetMaximum(1.1);
    hAll->SetMinimum(0.);
  }
  hAll->Draw("hist");
  hHT350MET120->Draw("hist,same");
  hHT900->Draw("hist,same");
  hL15HT600->Draw("hist,same");
  hL15HT400MET70->Draw("hist,same");
  c1->SetLogy(logy);

  hAll->GetXaxis()->SetLabelSize(0.03);
  hAll->GetXaxis()->SetTitleSize(0.03);
  hAll->GetXaxis()->SetTitleOffset(1.3);
  hAll->GetYaxis()->SetLabelSize(0.03);
  hAll->GetYaxis()->SetTitleSize(0.03);
  hAll->GetYaxis()->SetTitleOffset(1.3);
  c1->Modified();
  
  leg1->Draw();

  // TLatex * latex = new TLatex();
  // latex->SetNDC();
  // latex->SetTextAlign(12);
  // latex->SetTextFont(62);
  // latex->SetTextSize(0.037);
  // latex->DrawLatex(0.12, 0.87, "CMS Simulation");

  c1->Print("macros/junews/lostleptons/conceptual_plots/ptW_vs_triggers.pdf");

  delete leg1;
  delete c1;
  //  delete latex;
  delete hAll;
  delete hHT350MET120;
  delete hHT900;
  delete hL15HT600;
  delete hL15HT400MET70;

}
