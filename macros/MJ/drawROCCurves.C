
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
#include "TGraph.h"

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

void drawROCCurves(const TString sigName = "reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100*v75*.root", const TString bgName = "all", const TString plotNote="T1tttt_1500_100_allBG", const unsigned int nbins=20, const TCut cuts="", const bool killHighWeights=false) 
{
  set_plot_style();
  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


  TH1D* hMJ_pt100sig = new TH1D("hMJ_pt100sig","",nbins,0.,1500.);
  TH1D* hMJ_pt100bg = new TH1D("hMJ_pt100bg","",nbins,0.,1500.);
  TH1D* hMJ_pt150sig = new TH1D("hMJ_pt150sig","",nbins,0.,1500.);
  TH1D* hMJ_pt150bg = new TH1D("hMJ_pt150bg","",nbins,0.,1500.);
  TH1D* hMJ_pt200sig = new TH1D("hMJ_pt200sig","",nbins,0.,1500.);
  TH1D* hMJ_pt200bg = new TH1D("hMJ_pt200bg","",nbins,0.,1500.);
  TH1D* hHTsig = new TH1D("hHTsig","",nbins,500.,3500.);
  TH1D* hHTbg = new TH1D("hHTbg","",nbins,500.,3500.);

  TCut ckillHighWeights("");
  if (killHighWeights) ckillHighWeights="weightppb<1.";

  TCut weighted_selection((cuts+"met>200&&ht50>500&&jet1_pt>80&&jet3_pt>50.&&num_reco_veto_muons==0&&num_reco_veto_electrons==0")*"(weightppb*5000.)");

  TChain * bg = new TChain("reduced_tree");
  TChain * sig = new TChain("reduced_tree");
  if (!bgName.EqualTo("all")) bg->Add(bgName);// treestring is passed as an argument
  else {
    bg->Add("reduced_trees/TTJets*v75*.root");
    bg->Add("reduced_trees/QCD*v75*.root");
    bg->Add("reduced_trees/WJets*v75*.root");
    bg->Add("reduced_trees/ZJets*v75*.root");
    bg->Add("reduced_trees/TTo*v75*.root");
    bg->Add("reduced_trees/TBarTo*v75*.root");
    bg->Add("reduced_trees/*tW*v75*.root");
    bg->Add("reduced_trees/*HToBB*v75*.root");
    bg->Add("reduced_trees/TTbarH*v75*.root");
  }
  sig->Add(sigName);

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",850,800);
  double MJ_pt100_sig[nbins], MJ_pt100_bg[nbins];
  double MJ_pt150_sig[nbins], MJ_pt150_bg[nbins];
  double MJ_pt200_sig[nbins], MJ_pt200_bg[nbins];
  double HT_sig[nbins], HT_bg[nbins];
  sig->Project("hMJ_pt100sig","MJ_pt100",weighted_selection);
  bg->Project("hMJ_pt100bg","MJ_pt100",weighted_selection);
  sig->Project("hMJ_pt150sig","MJ_pt150",weighted_selection);
  bg->Project("hMJ_pt150bg","MJ_pt150",weighted_selection);
  sig->Project("hMJ_pt200sig","MJ_pt200",weighted_selection);
  bg->Project("hMJ_pt200bg","MJ_pt200",weighted_selection);
  sig->Project("hHTsig","ht50",weighted_selection);
  bg->Project("hHTbg","ht50",weighted_selection);
  for (unsigned int bin(0); bin<nbins+1; bin++) {
    MJ_pt100_sig[bin] = hMJ_pt100sig->Integral(bin+1,nbins+1)/hMJ_pt100sig->Integral(1,nbins+1);
    MJ_pt100_bg[bin] = 1-hMJ_pt100bg->Integral(bin+1,nbins+1)/hMJ_pt100bg->Integral(1,nbins+1);
    MJ_pt150_sig[bin] = hMJ_pt150sig->Integral(bin+1,nbins+1)/hMJ_pt150sig->Integral(1,nbins+1);
    MJ_pt150_bg[bin] = 1-hMJ_pt150bg->Integral(bin+1,nbins+1)/hMJ_pt150bg->Integral(1,nbins+1);
    MJ_pt200_sig[bin] = hMJ_pt200sig->Integral(bin+1,nbins+1)/hMJ_pt200sig->Integral(1,nbins+1);
    MJ_pt200_bg[bin] = 1-hMJ_pt200bg->Integral(bin+1,nbins+1)/hMJ_pt200bg->Integral(1,nbins+1);
    HT_sig[bin] = hHTsig->Integral(bin+1,nbins+1)/hHTsig->Integral(1,nbins+1);
    HT_bg[bin] = 1-hHTbg->Integral(bin+1,nbins+1)/hHTbg->Integral(1,nbins+1);  
  }

  TGraph* grMJ_pt100 = new TGraph(nbins,MJ_pt100_bg,MJ_pt100_sig);
  TGraph* grMJ_pt150 = new TGraph(nbins,MJ_pt150_bg,MJ_pt150_sig);
  TGraph* grMJ_pt200 = new TGraph(nbins,MJ_pt200_bg,MJ_pt200_sig);
  TGraph* grHT = new TGraph(nbins,HT_bg,HT_sig);

  grMJ_pt100->SetLineColor(38);
  grMJ_pt100->SetMarkerColor(38);
  grMJ_pt100->SetMarkerStyle(20);
  grMJ_pt100->SetLineWidth(2);
  grMJ_pt100->SetMarkerSize(2);
  grMJ_pt150->SetLineColor(8);
  grMJ_pt150->SetMarkerColor(8);
  grMJ_pt150->SetMarkerStyle(20);
  grMJ_pt150->SetLineWidth(2);
  grMJ_pt150->SetMarkerSize(2);
  grMJ_pt200->SetLineColor(46);
  grMJ_pt200->SetMarkerColor(46);
  grMJ_pt200->SetMarkerStyle(20);
  grMJ_pt200->SetLineWidth(2);
  grMJ_pt200->SetMarkerSize(2);
  grHT->SetLineColor(1);
  grHT->SetLineWidth(2);
  grHT->SetMarkerColor(1);
  grHT->SetMarkerStyle(20);
  grHT->SetMarkerSize(2);

  grMJ_pt100->Draw("ALP");
  grMJ_pt150->Draw("LP");
  grMJ_pt200->Draw("LP");
  grHT->Draw("LP");

  grMJ_pt100->SetTitle(";Background rejection;Signal Efficiency");

  thecanvas->Print("macros/MJ/plots/MJ_HT_ROC_"+plotNote+".pdf");

}

