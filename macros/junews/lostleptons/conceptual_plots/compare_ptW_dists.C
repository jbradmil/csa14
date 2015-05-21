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

void compare_ptW_dists(const TString treestring = "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", const TString plotNote="ttbar_1mu", bool logy = true) {

  
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  TH1::SetDefaultSumw2(1);
  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->SetFillStyle(4000);

  const int nbinsx(12); // ptW
  double binsx[nbinsx+1] = {0, 50, 100, 150, 200, 250, 300, 350, 400, 500, 600, 750, 1050};
  TH1D* hGen = new TH1D("hGen", ";p_{T}^{W} [GeV];Fraction of events", nbinsx, binsx);
  TH1D* hLMET = new TH1D("hLMET", ";Reconstructed p_{T}^{W} [GeV];Fraction of events", nbinsx, binsx);
  TH1D* hLMHT = new TH1D("hLMHT", ";Reconstructed p_{T}^{W} [GeV];Fraction of events", nbinsx, binsx);
  hGen->SetLineWidth(3);
  hGen->SetLineStyle(1);
  hGen->SetLineColor(1);
  hLMET->SetLineWidth(3);
  hLMET->SetLineColor(1000);
  hLMET->SetLineStyle(9);
  hLMHT->SetLineWidth(3);
  hLMHT->SetLineStyle(7);
  hLMHT->SetLineColor(1002);

  TCut SL = "num_true_mus==1&&num_true_els+num_true_had_taus==0";
  TCut baseline = "";
  //TCut baseline = "num_jets_pt30>=4&&min_delta_phi_met_N>6";
  TCut weight = "weightppb*10000";
  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  pypu->Project("hGen", "genW_pt", (SL+baseline+"genW_lep&&genW_pt>0")*weight);
  pypu->Project("hLMET", "mu_ptW", (SL+baseline+"mu_pt>10")*weight);
  pypu->Project("hLMHT", "mu_ptW_MHT", (SL+baseline+"mu_pt>10")*weight);

  TLegend * leg1 = new TLegend(0.2, 0.7, 0.8, 0.9);
  set_style(leg1,0.025);
  leg1->AddEntry(hGen,"Gen W","l");
  leg1->AddEntry(hLMET,"e/#mu+E_{T}^{miss}","l");
  leg1->AddEntry(hLMHT,"e/#mu+H_{T}^{miss}","l");
  leg1->Draw();

  FixOverflow(hGen);
  FixOverflow(hLMET);
  FixOverflow(hLMHT);
  hGen->Scale(1/hGen->Integral(1, nbinsx));
  hLMET->Scale(1/hLMET->Integral(1, nbinsx));
  hLMHT->Scale(1/hLMHT->Integral(1, nbinsx));
  
  double max = hGen->GetMaximum();
  if (hLMET->GetMaximum()>max) max = hLMET->GetMaximum();
  if (hLMHT->GetMaximum()>max) max = hLMHT->GetMaximum();

  if (logy) {
    hGen->SetMaximum(30);
    hGen->SetMinimum(0.00005);
  } else {
    hGen->SetMaximum(1.1);
    hGen->SetMinimum(0.);
  }
  hGen->Draw("hist");
  hLMET->Draw("hist,same");
  hLMHT->Draw("hist,same");
  c1->SetLogy(logy);

  hGen->GetXaxis()->SetLabelSize(0.03);
  hGen->GetXaxis()->SetTitleSize(0.03);
  hGen->GetXaxis()->SetTitleOffset(1.3);
  hGen->GetYaxis()->SetLabelSize(0.03);
  hGen->GetYaxis()->SetTitleSize(0.03);
  hGen->GetYaxis()->SetTitleOffset(1.3);
  c1->Modified();
  
  leg1->Draw();

  // TLatex * latex = new TLatex();
  // latex->SetNDC();
  // latex->SetTextAlign(12);
  // latex->SetTextFont(62);
  // latex->SetTextSize(0.037);
  // latex->DrawLatex(0.12, 0.87, "CMS Simulation");

  c1->Print("macros/junews/lostleptons/conceptual_plots/compare_ptW_dists.pdf");

  delete leg1;
  delete c1;
  //  delete latex;
  delete hGen;
  delete hLMET;
  delete hLMHT;

}
