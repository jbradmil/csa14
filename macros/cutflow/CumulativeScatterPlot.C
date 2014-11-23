
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

void CumulativeScatterPlot(const TString treestring = "reduced_trees/skimmed/SMS-T1tttt_2J_mGl-1500_mLSP-100*v76*.root", const TString plotNote="T1tttt_1500_100_4j_2b", const TCut cuts="") 
{
  set_plot_style();
  const int htbins(20), metbins(20);
  TH2D* h2 = new TH2D("h2","",htbins,0,2000,metbins,0,1000);
  TH2D* h2int = new TH2D("h2int",";H_{T} [GeV];E_{T}^{miss}",htbins,0,2000,metbins,0,1000);
  h2->SetStats(0);
  h2int->SetStats(0);
  h2int->GetZaxis()->SetLabelSize(0.02);
  h2int->GetZaxis()->SetTitle("Events passing cuts");
  h2int->GetZaxis()->SetTitleOffset(0.7);
  h2int->GetYaxis()->SetLabelSize(0.03);
  h2int->GetYaxis()->SetTitleOffset(1.3);
  h2int->GetXaxis()->SetLabelSize(0.03);
  TCut weighted_selection((cuts+"num_jets_pt30>=4&&num_csvm_jets30>1&&min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0")*"(weightppb*5000.)");

  TChain * pypu = new TChain("reduced_tree");
  if (treestring.Contains("root")) pypu->Add(treestring);// treestring is passed as an argument
  else {
    TFileCollection* fc = new TFileCollection("fc","",treestring);
    pypu->AddFileInfoList((TCollection*)fc->GetList());
  }
  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",850,800);
  pypu->Draw("met:ht30>>h2",weighted_selection,"colz");

  for (int x(0); x<htbins; x++) {
   for (int y(0); y<metbins; y++) {
     h2int->SetBinContent(x+1,y+1, h2->Integral(x+1,1000,y+1,1000));
   }
  }

  h2int->SetMaximum(50);
  h2int->SetMinimum(0);

  h2int->Draw("colz");

  thecanvas->Print("macros/cutflow/plots/scatter/met_vs_ht30_"+plotNote+"_yields.pdf");

}

