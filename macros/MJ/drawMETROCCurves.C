
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

void drawMETROCCurves(const TString sigName = "reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100*v75*.root", const TString bgName = "all", const TString plotNote="T1tttt_1500_100_allBG", const unsigned int nbins=20, const TCut cuts="", const bool killHighWeights=false) 
{
  set_plot_style();
  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos


  TH1D* hMETsig = new TH1D("hMETsig","",nbins,100.,800.);
  TH1D* hMET_OVER_SQRT_HT30sig = new TH1D("hMET_OVER_SQRT_HT30sig","",nbins,0.,50.);
  TH1D* hMET_OVER_SQRT_HT50sig = new TH1D("hMET_OVER_SQRT_HT50sig","",nbins,0.,50.);
  TH1D* hMET_OVER_SQRT_HT40sig = new TH1D("hMET_OVER_SQRT_HT40sig","",nbins,0.,50.);
  TH1D* hMETbg = new TH1D("hMETbg","",nbins,100.,800.);
  TH1D* hMET_OVER_SQRT_HT30bg = new TH1D("hMET_OVER_SQRT_HT30bg","",nbins,0.,50.);
  TH1D* hMET_OVER_SQRT_HT50bg = new TH1D("hMET_OVER_SQRT_HT50bg","",nbins,0.,50.);
  TH1D* hMET_OVER_SQRT_HT40bg = new TH1D("hMET_OVER_SQRT_HT40bg","",nbins,0.,50.);

  TCut ckillHighWeights("");
  if (killHighWeights) ckillHighWeights="weightppb<1.";

  TCut weighted_selection((cuts+"ht50>500&&jet3_pt>50.&&num_reco_veto_muons==0&&num_reco_veto_electrons==0")*"(weightppb*5000.)");

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
  double MET_sig[nbins], MET_OVER_SQRT_HT30_sig[nbins], MET_OVER_SQRT_HT50_sig[nbins], MET_OVER_SQRT_HT40_sig[nbins];
  double MET_bg[nbins], MET_OVER_SQRT_HT30_bg[nbins], MET_OVER_SQRT_HT50_bg[nbins], MET_OVER_SQRT_HT40_bg[nbins];
  sig->Project("hMETsig","met",weighted_selection);
  bg->Project("hMETbg","met",weighted_selection);
  sig->Project("hMET_OVER_SQRT_HT30sig","met_over_sqrt_ht30",weighted_selection);
  bg->Project("hMET_OVER_SQRT_HT30bg","met_over_sqrt_ht30",weighted_selection);
  sig->Project("hMET_OVER_SQRT_HT50sig","met_over_sqrt_ht50",weighted_selection);
  bg->Project("hMET_OVER_SQRT_HT50bg","met_over_sqrt_ht50",weighted_selection);
  sig->Project("hMET_OVER_SQRT_HT40sig","met_over_sqrt_ht40",weighted_selection);
  bg->Project("hMET_OVER_SQRT_HT40bg","met_over_sqrt_ht40",weighted_selection);
  for (unsigned int bin(0); bin<nbins+1; bin++) {
    MET_sig[bin] = hMETsig->Integral(bin+1,nbins+1)/hMETsig->Integral(1,nbins+1);
    MET_bg[bin] = 1-hMETbg->Integral(bin+1,nbins+1)/hMETbg->Integral(1,nbins+1);
    MET_OVER_SQRT_HT30_sig[bin] = hMET_OVER_SQRT_HT30sig->Integral(bin+1,nbins+1)/hMET_OVER_SQRT_HT30sig->Integral(1,nbins+1);
    MET_OVER_SQRT_HT30_bg[bin]  = 1-hMET_OVER_SQRT_HT30bg->Integral(bin+1,nbins+1)/hMET_OVER_SQRT_HT30bg->Integral(1,nbins+1); 
    MET_OVER_SQRT_HT50_sig[bin] = hMET_OVER_SQRT_HT50sig->Integral(bin+1,nbins+1)/hMET_OVER_SQRT_HT50sig->Integral(1,nbins+1);
    MET_OVER_SQRT_HT50_bg[bin]  = 1-hMET_OVER_SQRT_HT50bg->Integral(bin+1,nbins+1)/hMET_OVER_SQRT_HT50bg->Integral(1,nbins+1);  
    MET_OVER_SQRT_HT40_sig[bin] = hMET_OVER_SQRT_HT40sig->Integral(bin+1,nbins+1)/hMET_OVER_SQRT_HT40sig->Integral(1,nbins+1);
    MET_OVER_SQRT_HT40_bg[bin]  = 1-hMET_OVER_SQRT_HT40bg->Integral(bin+1,nbins+1)/hMET_OVER_SQRT_HT40bg->Integral(1,nbins+1); 
  }

  TGraph* grMET = new TGraph(nbins,MET_bg,MET_sig);
  TGraph* grMET_OVER_SQRT_HT30 = new TGraph(nbins,MET_OVER_SQRT_HT30_bg,MET_OVER_SQRT_HT30_sig);
  TGraph* grMET_OVER_SQRT_HT50 = new TGraph(nbins,MET_OVER_SQRT_HT50_bg,MET_OVER_SQRT_HT50_sig);
  TGraph* grMET_OVER_SQRT_HT40 = new TGraph(nbins,MET_OVER_SQRT_HT40_bg,MET_OVER_SQRT_HT40_sig);

  grMET->SetLineColor(1);
  grMET->SetMarkerColor(1);
  grMET->SetMarkerStyle(20);
  grMET->SetLineWidth(2);
  grMET->SetMarkerSize(2);

  grMET_OVER_SQRT_HT30->SetLineColor(38);
  grMET_OVER_SQRT_HT30->SetLineWidth(2);
  grMET_OVER_SQRT_HT30->SetMarkerColor(38);
  grMET_OVER_SQRT_HT30->SetMarkerStyle(20);
  grMET_OVER_SQRT_HT30->SetMarkerSize(2);


  grMET_OVER_SQRT_HT50->SetLineColor(46);
  grMET_OVER_SQRT_HT50->SetLineWidth(2);
  grMET_OVER_SQRT_HT50->SetMarkerColor(46);
  grMET_OVER_SQRT_HT50->SetMarkerStyle(20);
  grMET_OVER_SQRT_HT50->SetMarkerSize(2);

  grMET_OVER_SQRT_HT40->SetLineColor(8);
  grMET_OVER_SQRT_HT40->SetLineWidth(2);
  grMET_OVER_SQRT_HT40->SetMarkerColor(8);
  grMET_OVER_SQRT_HT40->SetMarkerStyle(20);
  grMET_OVER_SQRT_HT40->SetMarkerSize(2);


  grMET->Draw("ALP");
  grMET_OVER_SQRT_HT30->Draw("LP");
  grMET_OVER_SQRT_HT50->Draw("LP");
  grMET_OVER_SQRT_HT40->Draw("LP");

  grMET->SetTitle(";Background rejection;Signal Efficiency");

  thecanvas->Print("macros/MJ/plots/MET_MET_OVER_SQRT_HT_ROC_"+plotNote+".pdf");

}

