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

void MakePlot(TString yvar = "mht30", int nbinsy = 100, double ylow=0., double yhigh=1000., TString xvar = "true_mu_ptW", int nbinsx = 100, double xlow=0., double xhigh=1000., TString axistitles="p_{T}^{W} [GeV];H_{T}^{miss} [GeV]", const TCut cuts="true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", const TString treestring = "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", const TString plotNote="ttbar_1mu", bool logz = true, TString xvar_name="", TString yvar_name="") 
{
  set_plot_style();
  TH2D* h2 = new TH2D("h2",";"+axistitles,nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  h2->SetStats(0);
  h2->GetZaxis()->SetLabelSize(0.02);
  h2->GetYaxis()->SetLabelSize(0.03);
  h2->GetYaxis()->SetTitleOffset(1.3);
  h2->GetXaxis()->SetLabelSize(0.03);
  TCut weighted_selection((cuts)*"(weightppb*10000.)");

  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",850,800);
  pypu->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");

  h2->Scale(1/h2->Integral(1,nbinsx+1,1,nbinsy+1));

  thecanvas->SetLogz(logz);
  if (xvar_name.Length()>0) xvar=xvar_name;
  if (yvar_name.Length()>0) yvar=yvar_name;
  thecanvas->Print("macros/junews/lostleptons/conceptual_plots/"+yvar+"_vs_"+xvar+"_"+plotNote+"_scatter.pdf");
  
    
  delete h2;
  delete thecanvas;

}

void draw_scatter_plot() {
  MakePlot("mht30",  150, 0., 750., "true_mu_ptW",  150, 0., 750.,"p_{T}^{W} [GeV];H_{T}^{miss} [GeV]","true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", "ttbar_1mu", true);
  MakePlot("ht30",  150, 0., 1500., "true_mu_ptW",  150, 0., 750.,"p_{T}^{W} [GeV];H_{T} [GeV]","true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", "ttbar_1mu", true);
    MakePlot("true_mu_pt",  150, 0., 750., "true_mu_ptW",  150, 0., 750.,"p_{T}^{W} [GeV];p_{T}^{#mu} [GeV]","true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", "ttbar_1mu", true);
    MakePlot("mht30",  150, 0., 750., "0.5*(1-cos(true_mu_dTT))",  150, 0., 1.,"#frac{1}{2}(1-cos#Delta#theta_{T});H_{T}^{miss} [GeV]","true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", "ttbar_1mu", true, "true_mu_dTT");
        MakePlot("true_mu_ptW",  150, 0., 750., "0.5*(1-cos(true_mu_dTT))",  150, 0., 1.,"#frac{1}{2}(1-cos#Delta#theta_{T});p_{T}^{W} [GeV]","true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", "ttbar_1mu", true, "true_mu_dTT");
        MakePlot("true_mu_pt",  100, 0., 200., "0.5*(1-cos(true_mu_dTT))",  150, 0., 1.,"#frac{1}{2}(1-cos#Delta#theta_{T});p_{T}^{#mu} [GeV]","true_mu_pt>0&&num_true_mus==1&&num_true_els+num_true_had_taus==0", "reduced_trees/13TeV/TTJets_MSDecaysCKM_central_Tune4C_13TeV-madgraph-tauola_Phys14DR-PU20bx25_PHYS14_25_V1-v1_MINIAODSIM_UCSB2406_v78/*.root", "ttbar_1mu", true, "true_mu_dTT");
}

