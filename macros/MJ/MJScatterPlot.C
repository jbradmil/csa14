
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

void MJScatterPlot(const TString yvar = "MJ_pt100", int nbinsy = 200, double ylow=0., double yhigh=1500., const TString xvar = "ht50", int nbinsx = 200, double xlow=0., double xhigh=3000., TString axistitles="H_{T} [GeV];M_{J} [GeV]", const TCut cuts="", const TString treestring = "reduced_trees/QCD_Pt*v75*.root", const TString plotNote="qcd", bool logz = true) 
{
  set_plot_style();
  TH2D* h2 = new TH2D("h2",";"+axistitles,nbinsx,xlow,xhigh,nbinsy,ylow,yhigh);
  h2->SetStats(0);
  h2->GetZaxis()->SetLabelSize(0.02);
  h2->GetYaxis()->SetLabelSize(0.03);
  h2->GetYaxis()->SetTitleOffset(1.3);
  h2->GetXaxis()->SetLabelSize(0.03);
  TCut weighted_selection((cuts+"MJ_pt100>0&&ht50>0")*"(weightppb*5000.)");

  TChain * pypu = new TChain("reduced_tree");
  pypu->Add(treestring);// treestring is passed as an argument
  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",850,800);
  pypu->Draw(yvar+":"+xvar+">>h2",weighted_selection,"colz");

  thecanvas->SetLogz(logz);
  thecanvas->Print("macros/MJ/plots/"+yvar+"_vs_"+xvar+"_"+plotNote+"_scatter.pdf");

}

