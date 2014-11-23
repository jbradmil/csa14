
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
#include "RooStats/NumberCountingUtils.h"
#include "macros/include/convert_to_ZBi.h"


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

void ZBiScatterPlot(const TString yvar = "met", int nbinsy = 20, double ylow=0., double yhigh=1000., const TString xvar = "ht30", int nbinsx = 20, double xlow=0., double xhigh=2000., TString axistitles="H_{T} [GeV];E_{T}^{miss} [GeV]", const TCut cuts="num_jets_pt30>=4", const TString plotNote="4j_2b") 
{
  set_plot_style();
  TH1::SetDefaultSumw2(1);
  TH2::SetDefaultSumw2(1);
  TH2D* h2 = new TH2D("h2",";"+axistitles,nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  TH2D* h2BG = new TH2D("h2BG",";"+axistitles,nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  TH2D* h2ZBi = new TH2D("h2ZBi",";"+axistitles,nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  h2->SetStats(0);
  h2ZBi->SetStats(0);
  h2ZBi->GetZaxis()->SetLabelSize(0.02);
  h2ZBi->GetZaxis()->SetTitle("Z(Bi)");
  h2ZBi->GetZaxis()->SetTitleOffset(0.7);
  h2ZBi->GetZaxis()->SetTitleFont(132);
  h2ZBi->GetYaxis()->SetLabelSize(0.03);
  h2ZBi->GetYaxis()->SetTitleOffset(1.3);
  h2ZBi->GetXaxis()->SetLabelSize(0.03);
  TCut weighted_selection((cuts+"num_jets_pt30>=4&&min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0")*"(weightppb*5000.)");

  TChain *t1tttt_1500_100_ch = new TChain("reduced_tree");
  TChain *t1tttt_1200_800_ch = new TChain("reduced_tree");
  TChain *t1bbbb_1500_100_ch = new TChain("reduced_tree");
  TChain *t1bbbb_1000_900_ch = new TChain("reduced_tree");
  TChain *t1qqqq_1400_100_ch = new TChain("reduced_tree");
  TChain *t1qqqq_1000_800_ch = new TChain("reduced_tree");
  t1tttt_1500_100_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2268_v76/*.root");
  t1tttt_1200_800_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2267_v76/*.root");
  t1bbbb_1500_100_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v3_MINIAODSIM_UCSB2266_v76/*.root");
  t1bbbb_1000_900_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v2_MINIAODSIM_UCSB2264_v76/*.root");
  t1qqqq_1400_100_ch->Add("reduced_trees/SMS-T1qqqq_2J_mGl-1400_mLSP-100_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2140_v75/*.root");
  t1qqqq_1000_800_ch->Add("reduced_trees/SMS-T1qqqq_2J_mGl-1000_mLSP-800_Tune4C_13TeV-madgraph-tauola_Spring14miniaod-PU20bx25_POSTLS170_V5-v1_MINIAODSIM_UCSB2139_v75/*.root");

  TChain * SM_ch = new TChain("reduced_tree");
  TFileCollection* ttbar_fc = new TFileCollection("ttbar_fc","","reduced_trees/tfcs/ttbar.txt");
  TFileCollection* qcd_fc = new TFileCollection("qcd_fc","","reduced_trees/tfcs/qcd.txt");
  TFileCollection* znn_fc = new TFileCollection("znn_fc","","reduced_trees/tfcs/znn.txt");
  TFileCollection* wjets_fc = new TFileCollection("wjets_fc","","reduced_trees/tfcs/wjets.txt");
  TFileCollection* single_top_fc = new TFileCollection("single_top_fc","","reduced_trees/tfcs/single_top.txt");
  TFileCollection* other_fc = new TFileCollection("other_fc","","reduced_trees/tfcs/other.txt");
  
  SM_ch->AddFileInfoList((TCollection*)ttbar_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)qcd_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)znn_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)wjets_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)single_top_fc->GetList());
  SM_ch->AddFileInfoList((TCollection*)other_fc->GetList());

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",850,800);
  SM_ch->Draw(yvar+":"+xvar+">>h2BG",weighted_selection,"colz");
  t1tttt_1500_100_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  convert_to_ZBi_2D(h2ZBi, h2, h2BG, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  h2ZBi->Draw("colz");
  thecanvas->Print("macros/cutflow/plots/scatter/ZBi/t1tttt_1500_100/"+yvar+"_vs_"+xvar+"_"+plotNote+"_t1tttt_1500_100_ZBi_1114_5ifb_3evt.pdf");
  t1tttt_1200_800_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  convert_to_ZBi_2D(h2ZBi, h2, h2BG, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  h2ZBi->Draw("colz");
  thecanvas->Print("macros/cutflow/plots/scatter/ZBi/t1tttt_1200_800/"+yvar+"_vs_"+xvar+"_"+plotNote+"_t1tttt_1200_800_ZBi_1114_5ifb_3evt.pdf");
  t1bbbb_1500_100_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  convert_to_ZBi_2D(h2ZBi, h2, h2BG, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  h2ZBi->Draw("colz");
  thecanvas->Print("macros/cutflow/plots/scatter/ZBi/t1bbbb_1500_100/"+yvar+"_vs_"+xvar+"_"+plotNote+"_t1bbbb_1500_100_ZBi_1114_5ifb_3evt.pdf");
  t1bbbb_1000_900_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  convert_to_ZBi_2D(h2ZBi, h2, h2BG, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  h2ZBi->Draw("colz");
  thecanvas->Print("macros/cutflow/plots/scatter/ZBi/t1bbbb_1000_900/"+yvar+"_vs_"+xvar+"_"+plotNote+"_t1bbbb_1000_900_ZBi_1114_5ifb_3evt.pdf");
  t1qqqq_1400_100_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  convert_to_ZBi_2D(h2ZBi, h2, h2BG, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  h2ZBi->Draw("colz");
  thecanvas->Print("macros/cutflow/plots/scatter/ZBi/t1qqqq_1400_100/"+yvar+"_vs_"+xvar+"_"+plotNote+"_t1qqqq_1400_100_ZBi_1114_5ifb_3evt.pdf");
  t1qqqq_1000_800_ch->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");
  convert_to_ZBi_2D(h2ZBi, h2, h2BG, nbinsx, xlow, xhigh, nbinsy, ylow, yhigh);
  h2ZBi->Draw("colz");
  thecanvas->Print("macros/cutflow/plots/scatter/ZBi/t1qqqq_1000_800/"+yvar+"_vs_"+xvar+"_"+plotNote+"_t1qqqq_1000_800_ZBi_1114_5ifb_3evt.pdf");


}

