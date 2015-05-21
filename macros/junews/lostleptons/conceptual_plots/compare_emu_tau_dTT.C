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

void compare_emu_tau_dTT(const TString treestring = "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", const TString plotNote="ttbar_2b", bool logy = false) {

  
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  TH1::SetDefaultSumw2(1);
  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);

  TH1D* hMu = new TH1D("hMu", ";#frac{1}{2}(1-cos#Delta#theta_{T});Fraction of events", 20, 0., 1.);
  TH1D* hTAUMU = new TH1D("hTAUMU", ";#frac{1}{2}(1-cos#Delta#theta_{T});Fraction of events", 20, 0., 1.);
  TH1D* hSUM = new TH1D("hSUM", ";#frac{1}{2}(1-cos#Delta#theta_{T});Fraction of events", 20, 0., 1.);
  set_style(hMu, "qcd");
  set_style(hTAUMU, "other");

  TCut SL = "num_true_mus==1&&num_true_els+num_true_had_taus==0&&num_veto_mus==0";
  TCut baseline = "";
  //TCut baseline = "num_jets_pt30>=4&&min_delta_phi_met_N>6";
  TCut weight = "weightppb*10000";
  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  pypu->Project("hMu", "0.5*(1-cos(mu_dTT))", (SL+baseline+"num_true_lep_taus==0")*weight);
  pypu->Project("hTAUMU", "0.5*(1-cos(mu_dTT))", (SL+baseline+"num_true_lep_taus==1")*weight);
  // pypu->Project("hSUM", "mu_ptW_MHT", (SL+baseline+"mu_pt>10")*weight);
  hSUM->Add(hMu, hTAUMU, 1., 1.);

  TLegend * leg1 = new TLegend(0.2, 0.7, 0.8, 0.9);
  set_style(leg1,0.025);
  leg1->AddEntry(hMu,"W#rightarrow#mu#nu","f");
  leg1->AddEntry(hTAUMU,"W#rightarrow#tau#nu#rightarrow#mu#nu#nu","f");
  // leg1->AddEntry(hSUM,"e/#mu+H_{T}^{miss}","l");
  // leg1->Draw();

  FixOverflow(hMu);
  FixOverflow(hTAUMU);
  FixOverflow(hSUM);

  double norm = hSUM->Integral(1, 20);
  hMu->Scale(1/norm);
  hTAUMU->Scale(1/norm);
  hSUM->Scale(1/norm);

  THStack* hs = new THStack("hs", hMu->GetTitle());
  hs->Add(hTAUMU);
  hs->Add(hMu);

  

  
  // double max = hMu->GetMaximum();
  // if (hTAUMU->GetMaximum()>max) max = hTAUMU->GetMaximum();
  // if (hSUM->GetMaximum()>max) max = hSUM->GetMaximum();

  // if (logy) {
  //   hMu->SetMaximum(30);
  //   hMu->SetMinimum(0.00005);
  // } else {
  //   hMu->SetMaximum(1.1);
  //   hMu->SetMinimum(0.);
  // }
  // hMu->Draw("hist");
  // hTAUMU->Draw("hist,same");
  // hSUM->Draw("hist,same");
  hs->Draw("hist");
  c1->SetLogy(logy);

  hs->GetXaxis()->SetLabelSize(0.03);
  hs->GetXaxis()->SetTitleSize(0.03);
  hs->GetXaxis()->SetTitleOffset(1.3);
  hs->GetYaxis()->SetLabelSize(0.03);
  hs->GetYaxis()->SetTitleSize(0.03);
  hs->GetYaxis()->SetTitleOffset(1.3);
  hs->GetXaxis()->SetTitle(hMu->GetXaxis()->GetTitle());
  hs->GetYaxis()->SetTitle(hMu->GetYaxis()->GetTitle());
  c1->Modified();
  
  leg1->Draw();

  // TLatex * latex = new TLatex();
  // latex->SetNDC();
  // latex->SetTextAlign(12);
  // latex->SetTextFont(62);
  // latex->SetTextSize(0.037);
  // latex->DrawLatex(0.12, 0.87, "CMS Simulation");

  c1->Print("macros/junews/lostleptons/conceptual_plots/compare_emu_tau_dTT.pdf");

  delete leg1;
  delete c1;
  //  delete latex;
  delete hMu;
  delete hTAUMU;
  delete hSUM;
  delete hs;

}
