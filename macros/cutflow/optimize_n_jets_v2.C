
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
#include "macros/include/convert_to_int.h"

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

void optimize_n_jets_v2(const TString sigName = "reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100*v75*.root", const TString bgName = "all", const TString plotNote="T1tttt_1500_100_met_400_ht30_1000", const TCut cuts="") 
{
  set_plot_style();
  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos
  //TH1::SetStats(0);

  const unsigned int nbins = 16;

  TH1D* h_n30sig = new TH1D("h_n30sig",";n_{jets};Events after cut (#int L dt = 5 fb^{-1})",nbins,0.,16.);
  TH1D* h_n30bg = new TH1D("h_n30bg","",nbins,0.,16.);
  //  TH1D* h_n40sig = new TH1D("h_n40sig","",nbins,0.,16.);
  // TH1D* h_n40bg = new TH1D("h_n40bg","",nbins,0.,16.);
  TH1D* h_n50sig = new TH1D("h_n50sig","",nbins,0.,16.);
  TH1D* h_n50bg = new TH1D("h_n50bg","",nbins,0.,16.);
  //  TH1D* h_n70sig = new TH1D("h_n70sig","",nbins,0.,16.);
  //  TH1D* h_n70bg = new TH1D("h_n70bg","",nbins,0.,16.);
  TH1D* h_n100sig = new TH1D("h_n100sig","",nbins,0.,16.);
  TH1D* h_n100bg = new TH1D("h_n100bg","",nbins,0.,16.);

  // TH1D* h_n30sig_pass = new TH1D("h_n30sig_pass",";n_{jets};EventsPassing cut",nbins,0.,16.);
  // TH1D* h_n40sig_pass = new TH1D("h_n40sig_pass","",nbins,0.,16.);
  // TH1D* h_n50sig_pass = new TH1D("h_n50sig_pass","",nbins,0.,16.);
  // TH1D* h_n70sig_pass = new TH1D("h_n70sig_pass","",nbins,0.,16.);
  // TH1D* h_n100sig_pass = new TH1D("h_n100sig_pass","",nbins,0.,16.);

  TH1D* h_n30FOM = new TH1D("h_n30FOM",";n_{jets} cut;Q",nbins,0.,16.);
  //  TH1D* h_n40FOM = new TH1D("h_n40FOM","",nbins,0.,16.);
  TH1D* h_n50FOM = new TH1D("h_n50FOM","",nbins,0.,16.);
  //  TH1D* h_n70FOM = new TH1D("h_n70FOM","",nbins,0.,16.);
  TH1D* h_n100FOM = new TH1D("h_n100FOM","",nbins,0.,16.);

  h_n30sig->SetStats(0);
  h_n30FOM->SetStats(0);


  TCut start(cuts+"met>400&&ht30>1000&&min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&num_csvm_jets30>1");
  TCut weighted_selection(start*"(weightppb*5000.)");

  TChain * bg = new TChain("reduced_tree");
  TChain * sig = new TChain("reduced_tree");
  if (!bgName.EqualTo("all")) bg->Add(bgName);// treestring is passed as an argument
  else {
    bg->Add("reduced_trees/skimmed/TTJets*v76*.root");
    bg->Add("reduced_trees/skimmed/QCD*v75*.root");
    bg->Add("reduced_trees/skimmed/WJets*v75*.root");
    bg->Add("reduced_trees/skimmed/ZJets*v75*.root");
    bg->Add("reduced_trees/skimmed/TTo*v75*.root");
    bg->Add("reduced_trees/skimmed/TBarTo*v75*.root");
    bg->Add("reduced_trees/skimmed/*tW*v75*.root");
    bg->Add("reduced_trees/skimmed/*HToBB*v75*.root");
    bg->Add("reduced_trees/skimmed/TTbarH*v75*.root");
  }
  sig->Add(sigName);

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",800,600);
  // TPad* pad1 = new TPad("pad1","This is pad1",0.,0.5,1,1);
  // TPad* pad2 = new TPad("pad1","This is pad2",0.,0.,1,0.5);
  // pad1->Draw();
  // pad2->Draw();

  sig->Project("h_n30sig","num_jets_pt30",weighted_selection);
  bg->Project("h_n30bg","num_jets_pt30",weighted_selection);
  // sig->Project("h_n40sig","num_jets_pt40",weighted_selection);
  // bg->Project("h_n40bg","num_jets_pt40",weighted_selection);
  sig->Project("h_n50sig","num_jets_pt50",weighted_selection);
  bg->Project("h_n50bg","num_jets_pt50",weighted_selection);
  // sig->Project("h_n70sig","num_jets_pt70",weighted_selection);
  // bg->Project("h_n70bg","num_jets_pt70",weighted_selection);
  sig->Project("h_n100sig","num_jets_pt100",weighted_selection);
  bg->Project("h_n100bg","num_jets_pt100",weighted_selection);

  convert_to_int(h_n30sig);
  //  convert_to_int(h_n40sig);
  convert_to_int(h_n50sig);
  // convert_to_int(h_n70sig);
  convert_to_int(h_n100sig);
  convert_to_int(h_n30bg);
  //  convert_to_int(h_n40bg);
  convert_to_int(h_n50bg);
  // convert_to_int(h_n70bg);
  convert_to_int(h_n100bg);

  for (unsigned int bin(0); bin<nbins+1; bin++) {
    if (h_n30sig->GetBinContent(bin+1)>10) h_n30FOM->SetBinContent(bin+1, 2*(TMath::Sqrt(h_n30sig->GetBinContent(bin+1)+h_n30bg->GetBinContent(bin+1)) - TMath::Sqrt(h_n30bg->GetBinContent(bin+1))));
    if (h_n50sig->GetBinContent(bin+1)>10) h_n50FOM->SetBinContent(bin+1, 2*(TMath::Sqrt(h_n50sig->GetBinContent(bin+1)+h_n50bg->GetBinContent(bin+1)) - TMath::Sqrt(h_n50bg->GetBinContent(bin+1))));
    if (h_n100sig->GetBinContent(bin+1)>10) h_n100FOM->SetBinContent(bin+1, 2*(TMath::Sqrt(h_n100sig->GetBinContent(bin+1)+h_n100bg->GetBinContent(bin+1)) - TMath::Sqrt(h_n100bg->GetBinContent(bin+1))));
  }

  h_n30FOM->SetLineColor(1);
  //  h_n40FOM->SetLineColor(2);
  h_n50FOM->SetLineColor(3);
  //  h_n70FOM->SetLineColor(kOrange);
  h_n100FOM->SetLineColor(kCyan);
  h_n30FOM->SetLineWidth(3);
  //  h_n40FOM->SetLineWidth(3);
  h_n50FOM->SetLineWidth(3);
  //  h_n70FOM->SetLineWidth(3);
  h_n100FOM->SetLineWidth(3);

  h_n30sig->SetLineColor(1);
  //  h_n40sig->SetLineColor(2);
  h_n50sig->SetLineColor(3);
  //  h_n70sig->SetLineColor(kOrange);
  h_n100sig->SetLineColor(kCyan);
  h_n30sig->SetLineWidth(3);
  //  h_n40sig->SetLineWidth(3);
  h_n50sig->SetLineWidth(3);
  //  h_n70sig->SetLineWidth(3);
  h_n100sig->SetLineWidth(3);
 
  h_n30sig->Draw("hist");
  //  h_n40sig->Draw("hist,same");
  h_n50sig->Draw("hist,same");
  //  h_n70sig->Draw("hist,same");
  h_n100sig->Draw("hist,same");

  TLegend* leg = new TLegend(0.6,0.6,0.9,0.9);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(132);
  leg->AddEntry(h_n30FOM,"p_{T} > 30 GeV","l");
  //  leg->AddEntry(h_n40FOM,"p_{T} > 40 GeV","l");
  leg->AddEntry(h_n50FOM,"p_{T} > 50 GeV","l");
  //  leg->AddEntry(h_n70FOM,"p_{T} > 70 GeV","l");
  leg->AddEntry(h_n100FOM,"p_{T} > 100 GeV","l");
  leg->Draw();

  thecanvas->Print("macros/cutflow/plots/optimize_n_jets/n_jets_yields_"+plotNote+".pdf");

  thecanvas->Clear();


  h_n30FOM->SetMinimum(0);
  if (sigName.Contains("tttt")||sigName.Contains("1000")) h_n30FOM->SetMaximum(5);
  else h_n30FOM->SetMaximum(3.5);
  h_n30FOM->Draw("hist");
  //  h_n40FOM->Draw("hist,same");
  h_n50FOM->Draw("hist,same");
  //  h_n70FOM->Draw("hist,same");
  h_n100FOM->Draw("hist,same");


  leg->Draw();
  thecanvas->Print("macros/cutflow/plots/optimize_n_jets/n_jets_FOM_"+plotNote+".pdf");


}

