
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

void drawSJpTROCCurves_v2(const TString sigName = "reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100*v75*.root", const TString bgName = "all", const TString plotNote="T1tttt_1500_100_allBG", const unsigned int nbins=10, const TCut cuts="", const bool killHighWeights=false) 
{
  set_plot_style();
  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


  TH1D* hSJ_pT10sig = new TH1D("hSJ_pT10sig","",nbins,0.,20.);
  TH1D* hSJ_pT10bg = new TH1D("hSJ_pT10bg","",nbins,0.,20.);
  TH1D* hSJ_pT20sig = new TH1D("hSJ_pT20sig","",nbins,0.,10.);
  TH1D* hSJ_pT20bg = new TH1D("hSJ_pT20bg","",nbins,0.,10.);
  TH1D* hSJ_pT30sig = new TH1D("hSJ_pT30sig","",nbins,0.,10.);
  TH1D* hSJ_pT30bg = new TH1D("hSJ_pT30bg","",nbins,0.,10.);
 

  TCut ckillHighWeights("");
  if (killHighWeights) ckillHighWeights="weightppb<1.";

  TCut weighted_selection((cuts+"met>200&&jet3_pt>50.&&num_csvm_jets30>0")*"(weightppb*5000.)");

  TChain * bg = new TChain("reduced_tree");
  TChain * sig = new TChain("reduced_tree");
  if (!bgName.EqualTo("all")) bg->Add(bgName);// treestring is passed as an argument
  else {
    bg->Add("reduced_trees/TTJets*PU20*v75*.root");
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
  double SJ_pT10_sig[nbins], SJ_pT10_bg[nbins];
  double SJ_pT20_sig[nbins], SJ_pT20_bg[nbins];
  double SJ_pT30_sig[nbins], SJ_pT30_bg[nbins];
  sig->Project("hSJ_pT10sig","num_fatpT10_jets_pt100",weighted_selection);
  bg->Project("hSJ_pT10bg","num_fatpT10_jets_pt100",weighted_selection);
  sig->Project("hSJ_pT20sig","num_fatpT20_jets_pt100",weighted_selection);
  bg->Project("hSJ_pT20bg","num_fatpT20_jets_pt100",weighted_selection);
  sig->Project("hSJ_pT30sig","num_fatpT30_jets_pt100",weighted_selection);
  bg->Project("hSJ_pT30bg","num_fatpT30_jets_pt100",weighted_selection);
  for (unsigned int bin(0); bin<nbins+1; bin++) {
    SJ_pT10_sig[bin] = hSJ_pT10sig->Integral(bin+1,nbins+1)/hSJ_pT10sig->Integral(1,nbins+1);
    SJ_pT10_bg[bin] = 1-hSJ_pT10bg->Integral(bin+1,nbins+1)/hSJ_pT10bg->Integral(1,nbins+1);
    SJ_pT20_sig[bin] = hSJ_pT20sig->Integral(bin+1,nbins+1)/hSJ_pT20sig->Integral(1,nbins+1);
    SJ_pT20_bg[bin] = 1-hSJ_pT20bg->Integral(bin+1,nbins+1)/hSJ_pT20bg->Integral(1,nbins+1);
    SJ_pT30_sig[bin] = hSJ_pT30sig->Integral(bin+1,nbins+1)/hSJ_pT30sig->Integral(1,nbins+1);
    SJ_pT30_bg[bin] = 1-hSJ_pT30bg->Integral(bin+1,nbins+1)/hSJ_pT30bg->Integral(1,nbins+1);
  }

  TGraph* grSJ_pT10 = new TGraph(nbins,SJ_pT10_bg,SJ_pT10_sig);
  TGraph* grSJ_pT20 = new TGraph(nbins,SJ_pT20_bg,SJ_pT20_sig);
  TGraph* grSJ_pT30 = new TGraph(nbins,SJ_pT30_bg,SJ_pT30_sig);

  grSJ_pT10->SetLineColor(38);
  grSJ_pT10->SetMarkerColor(38);
  grSJ_pT10->SetMarkerStyle(20);
  grSJ_pT10->SetLineWidth(2);
  grSJ_pT10->SetMarkerSize(2);
  grSJ_pT20->SetLineColor(8);
  grSJ_pT20->SetMarkerColor(8);
  grSJ_pT20->SetMarkerStyle(20);
  grSJ_pT20->SetLineWidth(2);
  grSJ_pT20->SetMarkerSize(2);
  grSJ_pT30->SetLineColor(46);
  grSJ_pT30->SetMarkerColor(46);
  grSJ_pT30->SetMarkerStyle(20);
  grSJ_pT30->SetLineWidth(2);
  grSJ_pT30->SetMarkerSize(2);

  grSJ_pT10->Draw("ALP");
  grSJ_pT20->Draw("LP");
  grSJ_pT30->Draw("LP");

  grSJ_pT10->SetTitle(";Background rejection;Signal Efficiency");

  thecanvas->Print("macros/MJ/plots/talk1013/MJ_ROC_"+plotNote+".pdf");

}

