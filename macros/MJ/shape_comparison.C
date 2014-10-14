/* 
   Script for drawing a branch from reduec trees with a set of cuts.
*/

#include <iostream>
#include <vector>
#include <math.h> 
#include "TROOT.h"
#include "TFile.h"
#include "TSystem.h"
#include "TChain.h"
#include "TTree.h"
#include "TCut.h"
#include "THStack.h"
#include "TLine.h"
#include "../include/my_style.hpp"


double t1tttt_1500_100_tot(2.83805938720703125e+02);
double t1tttt_1200_800_tot(1.71379772949218750e+03);
double t1bbbb_1500_100_tot(2.83805938720703125e+02);
double t1bbbb_1000_900_tot(6.51358203125000000e+03);
double ttbar_tot(1.59261660000000000e+07);
//double qcd_tot(1.97939043919462400e+15);
double qcd_tot(3.39473989632000000e+12);
double qcd_high_pt(7.23519815680000000e+10);

using namespace std;

TString plotdir = "plots/";

void shape_comparison(TString var, TCut other_cuts,
		     TCut weights, TString title, int nbinsx, 
		     double xlow=0., double xup=100.,
		     bool plotLog=false,
		     double cut_low=-1, double cut_high=-1,
		     TString plot_title="default")
{


  TH1::SetDefaultSumw2(1);
  TH1::StatOverflows(true);
  //gROOT->SetBatch(1);


  if (gSystem->AccessPathName(plotdir))
    gSystem->mkdir(plotdir);

  // Setup style
  cout << "Setting tdr style."  << endl;
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  TChain* t1tttt_1500_100_ch = new TChain("reduced_tree");
  TChain* t1tttt_1200_800_ch = new TChain("reduced_tree");
  TChain* t1bbbb_1500_100_ch = new TChain("reduced_tree");
  TChain* t1bbbb_1000_900_ch = new TChain("reduced_tree");
  TChain* ttbar_ch = new TChain("reduced_tree");
  TChain* qcd_ch = new TChain("reduced_tree");
  
  ttbar_ch->Add("reduced_trees/TTJets*v75*.root");
  qcd_ch->Add("reduced_trees/QCD_Pt-*v75*.root");
  // znn_->Add("reduced_trees/ZJets*.root");
  // wjets_->Add("reduced_trees/WJets*.root");
  // single_top_->Add("reduced_trees/TTo*.root");
  // single_top_->Add("reduced_trees/TBarTo*.root");
  // single_top_->Add("reduced_trees/*tW*.root");
  // other_->Add("reduced_trees/*HToBB*.root");
  // other_->Add("reduced_trees/TTbarH*.root");
  t1tttt_1500_100_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1500*v75*root");
  t1tttt_1200_800_ch->Add("reduced_trees/SMS-T1tttt_2J_mGl-1200*v75*root");
  t1bbbb_1500_100_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1500*v75*root");
  t1bbbb_1000_900_ch->Add("reduced_trees/SMS-T1bbbb_2J_mGl-1000*v75*root");

  //TCut start ("ht50>500&&met>200");
  TCut start ("");

  // Book histograms
  TH1D * httbar = new TH1D("ttbar" , title, nbinsx, xlow, xup);
  TH1D * hqcd = new TH1D("qcd" , title, nbinsx, xlow, xup);
  TH1D * ht1bbbb_1500_100 = new TH1D("t1bbbb_1500_100" , title, nbinsx, xlow, xup);
  TH1D * ht1bbbb_1000_900 = new TH1D("t1bbbb_1000_900" , title, nbinsx, xlow, xup);
  TH1D * ht1tttt_1500_100 = new TH1D("t1tttt_1500_100" , title, nbinsx, xlow, xup);
  TH1D * ht1tttt_1200_800 = new TH1D("t1tttt_1200_800" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(start+other_cuts/*+"weightppb<1"*/);
  cout << "Filling histograms for " << var.Data() << endl;
  cout << "with selection " << (string)cut << endl;
  ttbar_ch->Project("ttbar",var,cut*weights);
  qcd_ch->Project("qcd",var,cut*weights);
  t1bbbb_1500_100_ch->Project("t1bbbb_1500_100",var,cut*weights);
  t1bbbb_1000_900_ch->Project("t1bbbb_1000_900",var,cut*weights);
  t1tttt_1500_100_ch->Project("t1tttt_1500_100",var,cut*weights);
  t1tttt_1200_800_ch->Project("t1tttt_1200_800",var,cut*weights);

  cout << "... DONE: filled histograms." << endl;

  httbar->SetLineColor(2);
  httbar->SetLineWidth(3);
  hqcd->SetLineColor(1);
  hqcd->SetLineWidth(3);
  ht1bbbb_1500_100->SetLineColor(kGreen+3);
  ht1bbbb_1000_900->SetLineColor(7);
  ht1tttt_1500_100->SetLineColor(9);
  ht1tttt_1200_800->SetLineColor(kOrange-3);
  ht1bbbb_1500_100->SetLineWidth(3);
  ht1bbbb_1000_900->SetLineWidth(3);
  ht1tttt_1500_100->SetLineWidth(3);
  ht1tttt_1200_800->SetLineWidth(3);
  ht1bbbb_1500_100->SetLineStyle(7);
  ht1bbbb_1000_900->SetLineStyle(7);
  ht1tttt_1500_100->SetLineStyle(7);
  ht1tttt_1200_800->SetLineStyle(7);
  // Setup legends
  TLegend * leg1 = new TLegend(0.5, 0.68, 0.9, 0.92);
  set_style(leg1,0.025);
  char label[100];
  sprintf(label,"QCD (#mu=%3.2f)",hqcd->GetMean());
  leg1->AddEntry(hqcd, label, "l");
  sprintf(label,"t#bar{t} (#mu=%3.2f)",httbar->GetMean());
  leg1->AddEntry(httbar, label, "l");
  sprintf(label,"T1bbbb(1500,100) (#mu=%3.2f)",ht1bbbb_1500_100->GetMean());
  leg1->AddEntry(ht1bbbb_1500_100, label, "l");
  sprintf(label,"T1bbbb(1000,900) (#mu=%3.2f)",ht1bbbb_1000_900->GetMean());
  leg1->AddEntry(ht1bbbb_1000_900, label, "l");
  sprintf(label,"T1tttt(1500,100) (#mu=%3.2f)",ht1tttt_1500_100->GetMean());
  leg1->AddEntry(ht1tttt_1500_100, label, "l");
  sprintf(label,"T1tttt(1200,800) (#mu=%3.2f)",ht1tttt_1200_800->GetMean());
  leg1->AddEntry(ht1tttt_1200_800, label, "l");


  // Normalize
  hqcd->Scale(1/hqcd->Integral(1,nbinsx+1));
  httbar->Scale(1/httbar->Integral(1,nbinsx+1));
  ht1tttt_1500_100->Scale(1/ht1tttt_1500_100->Integral(1,nbinsx+1));
  ht1tttt_1200_800->Scale(1/ht1tttt_1200_800->Integral(1,nbinsx+1));
  ht1bbbb_1500_100->Scale(1/ht1bbbb_1500_100->Integral(1,nbinsx+1));
  ht1bbbb_1000_900->Scale(1/ht1bbbb_1000_900->Integral(1,nbinsx+1));

  // hqcd->Scale(1/qcd_tot);
  // httbar->Scale(1/ttbar_tot);
  // ht1tttt_1500_100->Scale(1/t1tttt_1500_100_tot);
  // ht1tttt_1200_800->Scale(1/t1tttt_1200_800_tot);
  // ht1bbbb_1500_100->Scale(1/t1bbbb_1500_100_tot);
  // ht1bbbb_1000_900->Scale(1/t1bbbb_1000_900_tot);


  double ymax = TMath::Max(hqcd->GetMaximum(),ht1bbbb_1000_900->GetMaximum());
  if (httbar->GetMaximum()>ymax) ymax = httbar->GetMaximum();
  if (ht1tttt_1500_100->GetMaximum()>ymax) ymax = ht1tttt_1500_100->GetMaximum();
  // double ymin = TMath::Min(ht1tttt_1200_800->GetMinimum(),ht1bbbb_1000_900->GetMinimum());
  //  double binwidth = (xup - xlow) / nbinsx;
  TString ytitle = Form("Fraction of events");
  hqcd->GetYaxis()->SetTitle(ytitle);
  if(plotLog) {
    hqcd->SetMaximum(11*ymax);
    hqcd->SetMinimum(0.0001);
  }
  else {
    if (1.5*ymax>1.) hqcd->SetMaximum(1.);
    else hqcd->SetMaximum(1.5*ymax);
    hqcd->SetMinimum(0);
  }
  // Vertical lines for cuts
  TLine* line_low = new TLine(cut_low,0,cut_low,1.5*ymax);
  TLine* line_high = new TLine(cut_high,0,cut_high,1.5*ymax);
  set_style(line_low);
  set_style(line_high);

  // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  c1->cd();
  c1->SetLogy(plotLog);


  // Draw hists
  hqcd->Draw("hist");
  hqcd->GetXaxis()->SetLabelSize(0.04);
  httbar->Draw("hist same");
  ht1bbbb_1500_100->Draw("hist same");
  ht1tttt_1500_100->Draw("hist same");
  ht1tttt_1200_800->Draw("hist same");
  ht1bbbb_1000_900->Draw("hist same");

  if (cut_low>0) line_low->Draw("same");
  if (cut_high>0) line_high->Draw("same");


  // Draw legends
  leg1->Draw();
  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.042);
  latex->DrawLatex(0.19, 0.89, "CMS Simulation");
  latex->SetTextSize(0.03);
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV");
  
  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  TString plotDir("macros/MJ/plots/talk1013/shapes/");
  if (plot_title.EqualTo("default")) plot_title=var;
  gPad->Print(plotDir+plot_title+"_shapes.pdf");

  
  // Clean up
  delete leg1;
  // delete latex;
  delete c1;

  delete hqcd;
  delete httbar;
  delete ht1bbbb_1500_100;
  delete ht1bbbb_1000_900;
  delete ht1tttt_1500_100;
  delete ht1tttt_1200_800;

  delete qcd_ch;
  delete ttbar_ch;
  delete t1bbbb_1500_100_ch;
  delete t1bbbb_1000_900_ch;
  delete t1tttt_1500_100_ch;
  delete t1tttt_1200_800_ch;

  cout << "MakePlots(): DONE!" << endl;

  return;
}

  

  // draw_from_trees("met_over_sqrt_ht30",shape,"(weightppb*20000)",
  // 		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 30, 0., 30.,
  // 		 "",-1,-1,
  // 		 "met_over_sqrt_ht30");
  // draw_from_trees("met_over_sqrt_ht30",shape,"(weightppb*20000)",
  // 		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 40, 0., 80.,
  // 		 "plotLog",-1,-1,
  // 		 "met_over_sqrt_ht30_log");
  // draw_from_trees("met_over_sqrt_ht50",shape,"(weightppb*20000)",
  // 		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 30, 0., 30.,
  // 		 "",-1,-1,
  // 		 "met_over_sqrt_ht50");
  // draw_from_trees("met_over_sqrt_ht50",shape,"(weightppb*20000)",
  // 		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 40, 0., 80.,
  // 		 "plotLog",-1,-1,
  // 		 "met_over_sqrt_ht50_log");



