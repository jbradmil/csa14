
#include "TMath.h"
#include "TH1D.h"
#include "TH2D.h"
#include "TChain.h"
#include "TColor.h"
#include "TLine.h"
#include "TLegend.h"
#include "TString.h"
#include "TLatex.h"
#include "TCut.h"
#include "TStyle.h"
#include "TPad.h"
#include "TROOT.h"
#include "TCanvas.h"
#include "TFileCollection.h"

// #include "macros/include/cms_style.h"

#include <iostream>

void set_plot_style()
{
  const Int_t NRGBs = 5;
  const Int_t NCont = 255;

  Double_t stops[NRGBs] = { 0.00, 0.34, 0.61, 0.84, 1.00 };
  Double_t red[NRGBs]   = { 0.00, 0.00, 0.87, 1.00, 0.51 };
  Double_t green[NRGBs] = { 0.00, 0.81, 1.00, 0.20, 0.00 };
  Double_t blue[NRGBs]  = { 0.51, 1.00, 0.12, 0.00, 0.00 };
  TColor::CreateGradientColorTable(NRGBs, stops, red, green, blue, NCont);
  gStyle->SetNumberContours(NCont);
}

void ScatterPlot(const TString yvar = "met", int nbinsy = 150, double ylow=0., double yhigh=1500., const TString xvar = "ht30", int nbinsx = 200, double xlow=0., double xhigh=3000., TString axistitles="H_{T} [GeV];E_{T}^{miss} [GeV]", const TCut cuts="num_csvm_jets30>=2", const TString plotNote="geq2b", bool logz = true) 
{
  set_plot_style();
  TH2D* h2 = new TH2D("h2",";"+axistitles,nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  h2->SetStats(0);
  h2->GetZaxis()->SetLabelSize(0.02);
  h2->GetYaxis()->SetLabelSize(0.03);
  h2->GetYaxis()->SetTitleOffset(1.3);
  h2->GetXaxis()->SetLabelSize(0.03);
  TCut weighted_selection((cuts+"num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4")*"(weightppb*5000.)");

  TChain *ttbar_ch = new TChain("reduced_tree");
  TChain *qcd_ch = new TChain("reduced_tree");
  TChain *znn_ch = new TChain("reduced_tree");
  TChain *wjets_ch = new TChain("reduced_tree");
  TChain *t1tttt_1500_100_ch = new TChain("reduced_tree");
  TChain *t1tttt_1200_800_ch = new TChain("reduced_tree");
  TChain *t1bbbb_1500_100_ch = new TChain("reduced_tree");
  TChain *t1bbbb_1000_900_ch = new TChain("reduced_tree");

  TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","reduced_trees/tfcs/ttbar.txt");
  TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","reduced_trees/tfcs/qcd.txt");
  TFileCollection* znn_fc = new TFileCollection("znn_fc","","reduced_trees/tfcs/znn.txt");
  TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","reduced_trees/tfcs/wjets.txt");

  ttbar_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  qcd_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  znn_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  wjets_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());

  t1tttt_1500_100_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/*.root");
  t1tttt_1200_800_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2267_v76/*.root");
  t1bbbb_1500_100_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v3_MINIAODSIM_UCSB2266_v76/*.root");
  t1bbbb_1000_900_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2264_v76/*.root");

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",850,800);
  thecanvas->SetLogz(logz);

  t1tttt_1500_100_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_T1tttt_1500_100_scatter_1111.pdf");
  t1tttt_1200_800_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();  
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_T1tttt_1200_800_scatter_1111.pdf");
  t1bbbb_1500_100_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_T1bbbb_1500_100_scatter_1111.pdf");
  t1bbbb_1000_900_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_T1bbbb_1000_900_scatter_1111.pdf");

  ttbar_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_ttbar_scatter_1111.pdf");
  qcd_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_qcd_scatter_1111.pdf");
  znn_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_znn_scatter_1111.pdf");
  wjets_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  if (logz) {h2->SetMinimum(0.0001); h2->SetMaximum(20000);}
  thecanvas->Update();
  thecanvas->Print("macros/cutflow/plots/scatter/"+yvar+"_vs_"+xvar+"_"+plotNote+"_wjets_scatter_1111.pdf");

}

