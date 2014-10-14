
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

void optimize_n_jets(const TString sigName = "save/SMS-T1tttt_2J_mGl-1500_mLSP-100*v75*.root", const TString bgName = "all", const TString plotNote="T1tttt_1500_100_allBG", const TCut cuts="", const bool killHighWeights=false) 
{
  set_plot_style();
  TH1::StatOverflows(true);
  TH1::SetDefaultSumw2(); //trick to turn on Sumw2 for all histos
  //TH1::SetStats(0);

  const unsigned int nbins = 16;

  TH1D* h_n30sig = new TH1D("h_n30sig",";n_{jets};Events / 1",nbins,0.,16.);
  TH1D* h_n30bg = new TH1D("h_n30bg","",nbins,0.,16.);
  TH1D* h_n50sig = new TH1D("h_n50sig","",nbins,0.,16.);
  TH1D* h_n50bg = new TH1D("h_n50bg","",nbins,0.,16.);
  TH1D* h_n70sig = new TH1D("h_n70sig","",nbins,0.,16.);
  TH1D* h_n70bg = new TH1D("h_n70bg","",nbins,0.,16.);
  TH1D* h_n100sig = new TH1D("h_n100sig","",nbins,0.,16.);
  TH1D* h_n100bg = new TH1D("h_n100bg","",nbins,0.,16.);
  TH1D* h_n150sig = new TH1D("h_n150sig","",nbins,0.,16.);
  TH1D* h_n150bg = new TH1D("h_n150bg","",nbins,0.,16.);

  TH1D* h_n30sig_pass = new TH1D("h_n30sig_pass",";min n_{jets} cut;EventsPassing cut",nbins,0.,16.);
  TH1D* h_n50sig_pass = new TH1D("h_n50sig_pass","",nbins,0.,16.);
  TH1D* h_n70sig_pass = new TH1D("h_n70sig_pass","",nbins,0.,16.);
  TH1D* h_n100sig_pass = new TH1D("h_n100sig_pass","",nbins,0.,16.);
  TH1D* h_n150sig_pass = new TH1D("h_n150sig_pass","",nbins,0.,16.);

  TH1D* h_n30s_over_sqrt_b = new TH1D("h_n30s_over_sqrt_b",";min. n_{jets} cut;S/#sqrt{B}",nbins,0.,16.);
  TH1D* h_n50s_over_sqrt_b = new TH1D("h_n50s_over_sqrt_b","",nbins,0.,16.);
  TH1D* h_n70s_over_sqrt_b = new TH1D("h_n70s_over_sqrt_b","",nbins,0.,16.);
  TH1D* h_n100s_over_sqrt_b = new TH1D("h_n100s_over_sqrt_b","",nbins,0.,16.);
  TH1D* h_n150s_over_sqrt_b = new TH1D("h_n150s_over_sqrt_b","",nbins,0.,16.);

  h_n30sig->SetStats(0);
  h_n30sig_pass->SetStats(0);
  h_n30s_over_sqrt_b->SetStats(0);

  TCut ckillHighWeights("");
  if (killHighWeights) ckillHighWeights="weightppb<1.";

  TCut weighted_selection((cuts+"met>200&&num_reco_veto_muons==0&&num_reco_veto_electrons==0&&min_delta_phi_met_N>4.&&num_csvm_jets30>=2")*"(weightppb*5000.)");

  TChain * bg = new TChain("reduced_tree");
  TChain * sig = new TChain("reduced_tree");
  if (!bgName.EqualTo("all")) bg->Add(bgName);// treestring is passed as an argument
  else {
    bg->Add("save/TTJets*v75*.root");
    bg->Add("save/QCD*v75*.root");
    bg->Add("save/WJets*v75*.root");
    bg->Add("save/ZJets*v75*.root");
    bg->Add("save/TTo*v75*.root");
    bg->Add("save/TBarTo*v75*.root");
    bg->Add("save/*tW*v75*.root");
    bg->Add("save/*HToBB*v75*.root");
    bg->Add("save/TTbarH*v75*.root");
  }
  sig->Add(sigName);

  TCanvas * thecanvas= new TCanvas("thecanvas","the canvas",1000,800);
  
  sig->Project("h_n30sig","num_jets_pt30",weighted_selection);
  bg->Project("h_n30bg","num_jets_pt30",weighted_selection);
  sig->Project("h_n50sig","num_jets_pt50",weighted_selection);
  bg->Project("h_n50bg","num_jets_pt50",weighted_selection);
  sig->Project("h_n70sig","num_jets_pt70",weighted_selection);
  bg->Project("h_n70bg","num_jets_pt70",weighted_selection);
  sig->Project("h_n100sig","num_jets_pt100",weighted_selection);
  bg->Project("h_n100bg","num_jets_pt100",weighted_selection);
  sig->Project("h_n150sig","num_jets_pt150",weighted_selection);
  bg->Project("h_n150bg","num_jets_pt150",weighted_selection);
  for (unsigned int bin(0); bin<nbins+1; bin++) {
    if (h_n30bg->Integral(bin+1,nbins+1)>0) h_n30s_over_sqrt_b->SetBinContent(bin+1, h_n30sig->Integral(bin+1,nbins+1)/sqrt(h_n30bg->Integral(bin+1,nbins+1)));
    else h_n30s_over_sqrt_b->SetBinContent(bin+1,100.);
    if (h_n50bg->Integral(bin+1,nbins+1)>0) h_n50s_over_sqrt_b->SetBinContent(bin+1, h_n50sig->Integral(bin+1,nbins+1)/sqrt(h_n50bg->Integral(bin+1,nbins+1)));
    else h_n50s_over_sqrt_b->SetBinContent(bin+1,100.);
    if (h_n70bg->Integral(bin+1,nbins+1)>0) h_n70s_over_sqrt_b->SetBinContent(bin+1, h_n70sig->Integral(bin+1,nbins+1)/sqrt(h_n70bg->Integral(bin+1,nbins+1)));
    else h_n70s_over_sqrt_b->SetBinContent(bin+1,100.);
    if (h_n100bg->Integral(bin+1,nbins+1)>0) h_n100s_over_sqrt_b->SetBinContent(bin+1, h_n100sig->Integral(bin+1,nbins+1)/sqrt(h_n100bg->Integral(bin+1,nbins+1)));
    else h_n100s_over_sqrt_b->SetBinContent(bin+1,100.);
    if (h_n150bg->Integral(bin+1,nbins+1)>0) h_n150s_over_sqrt_b->SetBinContent(bin+1, h_n150sig->Integral(bin+1,nbins+1)/sqrt(h_n150bg->Integral(bin+1,nbins+1)));
    else h_n150s_over_sqrt_b->SetBinContent(bin+1,100.);
    h_n30sig_pass->SetBinContent(bin+1, h_n30sig->Integral(bin+1,nbins+1));
    h_n50sig_pass->SetBinContent(bin+1, h_n50sig->Integral(bin+1,nbins+1));
    h_n70sig_pass->SetBinContent(bin+1, h_n70sig->Integral(bin+1,nbins+1));
    h_n100sig_pass->SetBinContent(bin+1, h_n100sig->Integral(bin+1,nbins+1));
    h_n150sig_pass->SetBinContent(bin+1, h_n150sig->Integral(bin+1,nbins+1));
  }

  h_n30s_over_sqrt_b->SetLineColor(1);
  h_n50s_over_sqrt_b->SetLineColor(46);
  h_n70s_over_sqrt_b->SetLineColor(8);
  h_n100s_over_sqrt_b->SetLineColor(38);
  h_n150s_over_sqrt_b->SetLineColor(kMagenta-3);
  h_n30s_over_sqrt_b->SetLineWidth(3);
  h_n50s_over_sqrt_b->SetLineWidth(3);
  h_n70s_over_sqrt_b->SetLineWidth(3);
  h_n100s_over_sqrt_b->SetLineWidth(3);
  h_n150s_over_sqrt_b->SetLineWidth(3);
 

  h_n30s_over_sqrt_b->Draw("hist");
  h_n50s_over_sqrt_b->Draw("hist,same");
  h_n70s_over_sqrt_b->Draw("hist,same");
  h_n100s_over_sqrt_b->Draw("hist,same");
  h_n150s_over_sqrt_b->Draw("hist,same");

  h_n30s_over_sqrt_b->GetYaxis()->SetRangeUser(1,3);
  thecanvas->Modified();

  TLegend* leg = new TLegend(0.65,0.65,0.85,0.85);
  leg->SetFillStyle(0);
  leg->SetLineStyle(0);
  leg->SetTextFont(132);
  leg->AddEntry(h_n30s_over_sqrt_b,"p_{T} > 30 GeV","l");
  leg->AddEntry(h_n50s_over_sqrt_b,"p_{T} > 50 GeV","l");
  leg->AddEntry(h_n70s_over_sqrt_b,"p_{T} > 70 GeV","l");
  leg->AddEntry(h_n100s_over_sqrt_b,"p_{T} > 100 GeV","l");
  leg->AddEntry(h_n150s_over_sqrt_b,"p_{T} > 150 GeV","l");
  leg->Draw();

  // thecanvas->Print("macros/MJ/plots/n_jets_optimization_"+plotNote+".pdf");

  thecanvas->Clear();

  h_n30sig_pass->SetLineColor(1);
  h_n50sig_pass->SetLineColor(46);
  h_n70sig_pass->SetLineColor(8);
  h_n100sig_pass->SetLineColor(38);
  h_n150sig_pass->SetLineColor(kMagenta-3);
  h_n30sig_pass->SetLineWidth(3);
  h_n50sig_pass->SetLineWidth(3);
  h_n70sig_pass->SetLineWidth(3);
  h_n100sig_pass->SetLineWidth(3);
  h_n150sig_pass->SetLineWidth(3);

  double max = TMath::Max(h_n30sig_pass->GetMaximum(),h_n50sig_pass->GetMaximum());
  if (h_n70sig_pass->GetMaximum()>max) max = h_n70sig_pass->GetMaximum();
  if (h_n100sig_pass->GetMaximum()>max) max = h_n100sig_pass->GetMaximum();
  if (h_n150sig_pass->GetMaximum()>max) max = h_n150sig_pass->GetMaximum();
  h_n30sig_pass->SetMaximum(max*1.1);

  h_n30sig_pass->Draw("hist");
  h_n50sig_pass->Draw("hist,same");
  h_n70sig_pass->Draw("hist,same");
  h_n100sig_pass->Draw("hist,same");
  h_n150sig_pass->Draw("hist,same");
  leg->Draw();

  thecanvas->Print("macros/MJ/plots/n_jets_pass_"+plotNote+".pdf");


}

