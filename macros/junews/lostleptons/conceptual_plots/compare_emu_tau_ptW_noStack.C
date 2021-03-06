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

void compare_emu_tau_ptW_noStack(const TString treestring = "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", const TString plotNote="ttbar_2b", bool logy = true) {

  
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  TH1::SetDefaultSumw2(1);
  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);

  const int nbinsx(12); // ptW
  double binsx[nbinsx+1] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 500, 600, 750, 1050};
  TH1D* hMu = new TH1D("hMu", ";p_{T}^{W} [GeV];Fraction of events", nbinsx, binsx);
  TH1D* hTAUMU = new TH1D("hTAUMU", ";Reconstructed p_{T}^{W} [GeV];Fraction of events", nbinsx, binsx);
  hMu->SetLineWidth(3);
  hMu->SetLineColor(1001);
  hMu->SetLineStyle(9);
  hTAUMU->SetLineWidth(3);
  hTAUMU->SetLineStyle(7);
  hTAUMU->SetLineColor(1003);
  TCut SL = "num_true_mus==1&&num_true_els+num_true_had_taus==0";
  TCut baseline = "";
  //TCut baseline = "num_jets_pt30>=4&&min_delta_phi_met_N>6";
  TCut weight = "weightppb*10000";
  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  pypu->Project("hMu", "mu_ptW", (SL+baseline+"mu_pt>10&&mu_vid&&mu_mini_iso_tr02<0.2&&mu_mT<100&&num_true_lep_taus==0")*weight);
  pypu->Project("hTAUMU", "mu_ptW", (SL+baseline+"mu_pt>10&&mu_vid&&mu_mini_iso_tr02<0.2&&mu_mT<100&&num_true_lep_taus==1")*weight);
  // pypu->Project("hSUM", "mu_ptW_MHT", (SL+baseline+"mu_pt>10")*weight);

  TLegend * leg1 = new TLegend(0.2, 0.7, 0.8, 0.9);
  set_style(leg1,0.025);
  leg1->AddEntry(hMu,"W#rightarrow#mu#nu","l");
  leg1->AddEntry(hTAUMU,"W#rightarrow#tau#nu#rightarrow#mu#nu#nu","l");
  // leg1->AddEntry(hSUM,"e/#mu+H_{T}^{miss}","l");
  // leg1->Draw();

  FixOverflow(hMu);
  FixOverflow(hTAUMU);

  //double norm = hSUM->Integral(1, 20);
  hMu->Scale(1/hMu->Integral(1, 20));
  hTAUMU->Scale(1/hTAUMU->Integral(1, 20));
  //hSUM->Scale(1/norm);

  // THStack* hs = new THStack("hs", hMu->GetTitle());
  // hs->Add(hTAUMU);
  // hs->Add(hMu);

  

  
  double max = hMu->GetMaximum();
  if (hTAUMU->GetMaximum()>max) max = hTAUMU->GetMaximum();
  // if (hSUM->GetMaximum()>max) max = hSUM->GetMaximum();

  if (logy) {
    hMu->SetMaximum(30);
    hMu->SetMinimum(0.00005);
  } else {
    hMu->SetMaximum(1.1);
    hMu->SetMinimum(0.);
  }
  hMu->Draw("hist");
  hTAUMU->Draw("hist,same");

  c1->SetLogy(logy);

  hMu->GetXaxis()->SetLabelSize(0.03);
  hMu->GetXaxis()->SetTitleSize(0.03);
  hMu->GetXaxis()->SetTitleOffset(1.3);
  hMu->GetYaxis()->SetLabelSize(0.03);
  hMu->GetYaxis()->SetTitleSize(0.03);
  hMu->GetYaxis()->SetTitleOffset(1.3);
  c1->Modified();
  
  leg1->Draw();

  // TLatex * latex = new TLatex();
  // latex->SetNDC();
  // latex->SetTextAlign(12);
  // latex->SetTextFont(62);
  // latex->SetTextSize(0.037);
  // latex->DrawLatex(0.12, 0.87, "CMS Simulation");

  c1->Print("macros/junews/lostleptons/conceptual_plots/compare_emu_tau_ptW_log_noStack.pdf");

  delete leg1;
  delete c1;
  //  delete latex;
  delete hMu;
  delete hTAUMU;
 

}
