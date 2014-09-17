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


TChain* t1tttt_1500_100_ch;
TChain* t1tttt_1200_800_ch;
TChain* t1bbbb_1500_100_ch;
TChain* t1bbbb_1000_900_ch;
TChain* qcd_ch;
TChain* ttbar_ch;


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

void draw_from_trees(TString var, TCut other_cuts,
		     TString weights, TString title, int nbinsx, 
		     double xlow, double xup,
		     TString options="plotSig:plotLog:plotData",
		     double cut_low=-1, double cut_high=-1,
		     TString plot_title="default")
{

  bool plotLog = options.Contains("plotLog") && (!options.Contains("!plotLog"));
  //  bool highMass = options.Contains("highMass") && (!options.Contains("!highMass"));

  // Book histograms
  TH1D * httbar = new TH1D("ttbar" , title, nbinsx, xlow, xup);
  TH1D * hqcd = new TH1D("qcd" , title, nbinsx, xlow, xup);
  TH1D * ht1bbbb_1500_100 = new TH1D("t1bbbb_1500_100" , title, nbinsx, xlow, xup);
  TH1D * ht1bbbb_1000_900 = new TH1D("t1bbbb_1000_900" , title, nbinsx, xlow, xup);
  TH1D * ht1tttt_1500_100 = new TH1D("t1tttt_1500_100" , title, nbinsx, xlow, xup);
  TH1D * ht1tttt_1200_800 = new TH1D("t1tttt_1200_800" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(other_cuts);
  cout << "Filling histograms for " << var.Data() << endl;
  ttbar_ch->Project("ttbar",var,cut*weights);
  qcd_ch->Project("qcd",var,cut*weights);
  t1bbbb_1500_100_ch->Project("t1bbbb_1500_100",var,cut*weights);
  t1bbbb_1000_900_ch->Project("t1bbbb_1000_900",var,cut*weights);
  t1tttt_1500_100_ch->Project("t1tttt_1500_100",var,cut*weights);
  t1tttt_1200_800_ch->Project("t1tttt_1200_800",var,cut*weights);
  
  //Double_t ttbar_e(0.), qcd_e(0.), znn_e(0.), wjets_e(0.), other_e(0.), single_top_e(0.), bg_tot_e(0.);
  //  Double_t t1bbbb_1500_100_e(0.), t1bbbb_1000_900_e(0.), t1tttt_1500_100_e(0.),t1tttt_1200_800_e(0.) ;
  // double ttbar_n(httbar->IntegralAndError(0,nbinsx+1, ttbar_e));
  // double qcd_n(hqcd->IntegralAndError(0,nbinsx+1, qcd_e));
  // double znn_n(hznn->IntegralAndError(0,nbinsx+1, znn_e));
  // double wjets_n(hwjets->IntegralAndError(0,nbinsx+1, wjets_e));
  // double other_n(hother->IntegralAndError(0,nbinsx+1, other_e));
  // double single_top_n(hsingle_top->IntegralAndError(0,nbinsx+1, single_top_e));
  // double bg_tot(hmc_exp->IntegralAndError(0,nbinsx+1, bg_tot_e));
  // double t1bbbb_1500_100_n(ht1bbbb_1500_100->IntegralAndError(0,nbinsx+1, t1bbbb_1500_100_e));
  // double t1bbbb_1000_900_n(ht1bbbb_1000_900->IntegralAndError(0,nbinsx+1, t1bbbb_1000_900_e));
  // double t1tttt_1500_100_n(ht1tttt_1500_100->IntegralAndError(0,nbinsx+1, t1tttt_1500_100_e));
  // double t1tttt_1200_800_n(ht1tttt_1200_800->IntegralAndError(0,nbinsx+1, t1tttt_1200_800_e));

  // printf("Counts before cut: %s\n",var.Data());
  // printf("&t1bbbb_1500_100&t1bbbb_1000_900&t1tttt_1500_100&t1tttt_1200_800 \\\\ \n");
  // printf("%s & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f  \\\\\n",
  // 	 var.Data(),
  // 	 t1bbbb_1500_100_n,t1bbbb_1500_100_e,
  // 	 t1bbbb_1000_900_n,t1bbbb_1000_900_e,
  // 	 t1tttt_1500_100_n,t1tttt_1500_100_e,
  // 	 t1tttt_1200_800_n,t1tttt_1200_800_e);


  cout << "... DONE: filled histograms." << endl;

  httbar->SetLineColor(2);
  httbar->SetLineWidth(3);
  hqcd->SetLineColor(1);
  hqcd->SetLineWidth(3);
  ht1bbbb_1500_100->SetLineColor(30);
  ht1bbbb_1000_900->SetLineColor(7);
  ht1tttt_1500_100->SetLineColor(9);
  ht1tttt_1200_800->SetLineColor(kYellow+2);
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
  // hqcd->Scale(1/hqcd->Integral(1,nbinsx+1));
  // httbar->Scale(1/httbar->Integral(1,nbinsx+1));
  // ht1tttt_1500_100->Scale(1/ht1tttt_1500_100->Integral(1,nbinsx+1));
  // ht1tttt_1200_800->Scale(1/ht1tttt_1200_800->Integral(1,nbinsx+1));
  // ht1bbbb_1500_100->Scale(1/ht1bbbb_1500_100->Integral(1,nbinsx+1));
  // ht1bbbb_1000_900->Scale(1/ht1bbbb_1000_900->Integral(1,nbinsx+1));

  hqcd->Scale(1/qcd_tot);
  httbar->Scale(1/ttbar_tot);
  ht1tttt_1500_100->Scale(1/t1tttt_1500_100_tot);
  ht1tttt_1200_800->Scale(1/t1tttt_1200_800_tot);
  ht1bbbb_1500_100->Scale(1/t1bbbb_1500_100_tot);
  ht1bbbb_1000_900->Scale(1/t1bbbb_1000_900_tot);


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
  // TLatex * latex = new TLatex();
  // latex->SetNDC();
  // latex->SetTextAlign(12);
  // latex->SetTextFont(62);
  // latex->SetTextSize(0.042);
  // latex->DrawLatex(0.19, 0.89, "CMS Simulation");
  // latex->SetTextSize(0.03);
  // latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV");
  
  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  if (plot_title.EqualTo("default")) plot_title=plotdir+var;
  gPad->Print(plotdir+plot_title+".pdf");

  TFile* outrootfile = TFile::Open(plotdir+plot_title+".root", "RECREATE");
  httbar->Write();
  hqcd->Write();
  ht1bbbb_1500_100->Write();
  ht1bbbb_1000_900->Write();
  ht1tttt_1500_100->Write();
  ht1tttt_1200_800->Write();

  outrootfile->Close();

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

  cout << "MakePlots(): DONE!" << endl;

  return;
}

int main() {

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

  // ttbar_ = new TChain("reduced_tree");
  // qcd_ = new TChain("reduced_tree");
  // znn_ = new TChain("reduced_tree");
  // wjets_ = new TChain("reduced_tree");
  // single_top_ = new TChain("reduced_tree");
  // other_ = new TChain("reduced_tree");
  t1tttt_1500_100_ch = new TChain("reduced_tree");
  t1tttt_1200_800_ch = new TChain("reduced_tree");
  t1bbbb_1500_100_ch = new TChain("reduced_tree");
  t1bbbb_1000_900_ch = new TChain("reduced_tree");
  ttbar_ch = new TChain("reduced_tree");
  qcd_ch = new TChain("reduced_tree");
  
  ttbar_ch->Add("../../reduced_trees/TTJets*.root");
  qcd_ch->Add("../../reduced_trees/QCD_Pt-*.root");
  // znn_->Add("../../reduced_trees/ZJets*.root");
  // wjets_->Add("../../reduced_trees/WJets*.root");
  // single_top_->Add("../../reduced_trees/TTo*.root");
  // single_top_->Add("../../reduced_trees/TBarTo*.root");
  // single_top_->Add("../../reduced_trees/*tW*.root");
  // other_->Add("../../reduced_trees/*HToBB*.root");
  // other_->Add("../../reduced_trees/TTbarH*.root");
  t1tttt_1500_100_ch->Add("../../reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1-MINIAODSIM_cfA_v74.list.root");
  t1tttt_1200_800_ch->Add("../../reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1-MINIAODSIM_cfA_v74.list.root");
  t1bbbb_1500_100_ch->Add("../../reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v2-MINIAODSIM_cfA_v74.list.root");
  t1bbbb_1000_900_ch->Add("../../reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1-MINIAODSIM_cfA_v74.list.root");

  // quick skim
  //  TCut trigger("passesPVCut&&((ht40>350&&met>120)||(ht40>900)||(met>170)||(num_csvm_jets>0&&met>120))");
  TCut shape("weightppb<100.");
  TCut trigger(shape+"passesPVCut&&((ht40>350&&met>120)||(ht40>900)||(met>170)||( (jet1_csv>=0.7||jet2_csv>=0.7||jet3_csv>=0.7||jet4_csv>=0.7||jet5_csv>=0.7||jet6_csv>=0.7)&&met>120))");
  TCut highMass(shape+trigger+"ht50>1000.&&met>200.&&num_csvm_jets>0");
  //  TCut start("passesPVCut&&((ht40>500&&met>175)||(ht40>1000)||(met>250)||(num_csvt_jets>0&&met>175))");
  TCut start ("ht40>400&&met>175&&jet1_pt>70&&jet2_pt>70&&jet3_pt>50");
  TCut baseline(start+"ht50>400&&met>125&&num_jets_pt50>=3&&num_jets_pt70>=2");
  TCut search(baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_iso_tracks==0");
  // cout << "Applying trigger cuts: \n" << (string)start << endl;
  // ttbar_ch = (TChain*)ttbar_->CopyTree(baseline);
  // cout << "... DONE: ttbar copy tree. N=" << ttbar_ch->GetEntries() << endl;
  // qcd_ch = (TChain*)qcd_->CopyTree(baseline);
  // cout << "... DONE: qcd copy tree. N=" << qcd_ch->GetEntries() << endl;
  // znn_ch = (TChain*)znn_->CopyTree(baseline);
  // cout << "... DONE: znn copy tree. N=" << znn_ch->GetEntries() << endl;
  // wjets_ch = (TChain*)wjets_->CopyTree(baseline);
  // cout << "... DONE: wjets copy tree. N=" << wjets_ch->GetEntries() << endl;
  // single_top_ch = (TChain*)single_top_->CopyTree(baseline);
  // cout << "... DONE: single_top copy tree. N=" << single_top_ch->GetEntries() << endl;
  // other_ch = (TChain*)other_->CopyTree(baseline);
  // cout << "... DONE: other copy tree. N=" << other_ch->GetEntries() << endl;
  cout << "Generated signal events: " << endl;
  cout << "... DONE: ttbar copy tree. N=" << ttbar_ch->GetEntries() << endl;
  cout << "... DONE: qcd copy tree. N=" << qcd_ch->GetEntries() << endl;
  cout << "... DONE: t1tttt_1500_100 copy tree. N=" << t1tttt_1500_100_ch->GetEntries() << endl;
  cout << "... DONE: t1tttt_1200_800 copy tree. N=" << t1tttt_1200_800_ch->GetEntries() << endl;
  cout << "... DONE: t1bbbb_1500_100 copy tree. N=" << t1bbbb_1500_100_ch->GetEntries() << endl;
  cout << "... DONE: t1bbbb_1000_900 copy tree. N=" << t1bbbb_1000_900_ch->GetEntries() << endl;

  /*draw_from_trees(TString var, TCut other_cuts,
    TString weights, TString title, int nbinsx, 
    double xlow, double xup,
    TString options="plotSig:plotLog:plotData",
    double cut_low=-1, double cut_high=-1,
    TString plot_title="default")
  */

  // Start--just shapes, no cuts
  
  draw_from_trees("mht",shape,"(weightppb*20000)",
    		  ";H_{T}^{miss} [GeV];Events / 20 fb^{-1}", 70, 100., 1500.,
    		  "plotSig:plotLog",-1,-1,
    		  "mht_log");
  draw_from_trees("mht",shape,"(weightppb*20000)",
    		  ";H_{T}^{miss} [GeV];Events / 20 fb^{-1}", 70, 100., 1500.,
    		  "plotSig",-1,-1,
    		  "mht");
  draw_from_trees("met",shape,"(weightppb*20000)",
    		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1750,
    		  "plotSig:plotLog",-1,-1,
    		  "met_log");
  draw_from_trees("met",shape,"(weightppb*20000)",
    		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1750,
    		  "plotSig",-1,-1,
    		  "met");

  draw_from_trees("num_reco_veto_electrons",shape,"(weightppb*20000)",
    		  ";n_{e} (p_{T} > 10 GeV);Events / 20 fb^{-1}", 5, 0., 5.,
    		  "",-1,-1,
    		  "num_electrons");
  draw_from_trees("num_reco_veto_muons",shape,"(weightppb*20000)",
    		  ";n_{#mu} (p_{T} > 10 GeV);Events / 20 fb^{-1}", 5, 0., 5.,
    		  "",-1,-1,
    		  "num_muons");

  draw_from_trees("min_delta_phi_met_N",shape,"(weightppb*20000)",
    		  ";#Delta#hat{#phi}^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
    		  "",-1,-1,
    		  "min_delta_phi_met_N");
  draw_from_trees("min_delta_phi_met_N_loose_jets",shape,"(weightppb*20000)",
    		  ";#Delta#hat{#phi}^{min}_{N};Events / 20 fb^{-1}", 20, 0., 40.,
    		  "",-1,-1,
    		  "min_delta_phi_met_N_loose_jets");
  draw_from_trees("min_delta_phi_met",shape,"(weightppb*20000)",
    		  ";#Delta#phi_{min};Events / 20 fb^{-1}", 16, 0., 3.2,
    		  "",-1,-1,
    		  "min_delta_phi_met");
  draw_from_trees("min_delta_phi_met_loose_jets",shape,"(weightppb*20000)",
    		  ";#Delta#phi_{min};Events / 20 fb^{-1}", 16, 0., 3.2,
    		  "",-1,-1,
    		  "min_delta_phi_met_loose_jets");


  draw_from_trees("num_jets_pt20",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "",-1,-1,
    		  "njets20");
  draw_from_trees("num_jets_pt30",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 30 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "",-1,-1,
    		  "njets30");
  draw_from_trees("num_jets_pt40",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 40 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "",-1,-1,
    		  "njets40");
  draw_from_trees("num_jets_pt50",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "njets50");
  draw_from_trees("num_jets_pt70",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 70 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "njets70");
  draw_from_trees("num_jets_pt100",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 100 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "njets100");
  draw_from_trees("num_jets_pt150",shape,"(weightppb*20000)",
    		  ";n_{jets} (p_{T} > 150 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "njets150");
    draw_from_trees("num_csvl_jets20",shape,"(weightppb*20000)",
    		  ";n_{CSVL} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvl20");
   draw_from_trees("num_csvm_jets20",shape,"(weightppb*20000)",
    		  ";n_{CSVM} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvm20");
   draw_from_trees("num_csvt_jets20",shape,"(weightppb*20000)",
    		  ";n_{CSVT} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvt20");
    draw_from_trees("num_csvl_jets30",shape,"(weightppb*20000)",
    		  ";n_{CSVL} (p_{T} > 30 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvl30");
   draw_from_trees("num_csvm_jets30",shape,"(weightppb*20000)",
    		  ";n_{CSVM} (p_{T} > 30 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvm30");
   draw_from_trees("num_csvt_jets30",shape,"(weightppb*20000)",
    		  ";n_{CSVT} (p_{T} > 30 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvt30");
    draw_from_trees("num_csvl_jets40",shape,"(weightppb*20000)",
    		  ";n_{CSVL} (p_{T} > 40 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvl40");
   draw_from_trees("num_csvm_jets40",shape,"(weightppb*20000)",
    		  ";n_{CSVM} (p_{T} > 40 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvm40");
   draw_from_trees("num_csvt_jets40",shape,"(weightppb*20000)",
    		  ";n_{CSVT} (p_{T} > 40 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvt40");
  draw_from_trees("num_csvl_jets",shape,"(weightppb*20000)",
    		  ";n_{CSVL} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvl50");
  draw_from_trees("num_csvm_jets",shape,"(weightppb*20000)",
    		  ";n_{CSVM} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvm50");
   draw_from_trees("num_csvt_jets",shape,"(weightppb*20000)",
    		  ";n_{CSVT} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
    		  "plotSig",-1,-1,
    		  "ncsvt50");

  draw_from_trees("jet1_pt",shape+"jet1_pt>0","(weightppb*20000)",
    		  ";p_{T}(jet 1) [GeV];Events / 20 fb^{-1}", 50, 0., 1000.,
    		  "plotLog",-1,-1,
    		  "jet1_pt_log");
  draw_from_trees("jet2_pt",shape+"num_jets_pt20>1","(weightppb*20000)",
    		  ";p_{T}(jet 2) [GeV];Events / 20 fb^{-1}", 50, 0., 1000.,
    		  "plotLog",-1,-1,
    		  "jet2_pt_log");
  draw_from_trees("jet3_pt",shape+"num_jets_pt20>2","(weightppb*20000)",
    		  ";p_{T}(jet 3) [GeV];Events / 20 fb^{-1}", 50, 0., 1000.,
    		  "plotLog",-1,-1,
    		  "jet3_pt_log");
   draw_from_trees("highest_csv",shape+"highest_csv>-100","(weightppb*20000)",
  		 ";Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  		 "",0.244,0.679,
  		 "1st_csv");
   draw_from_trees("second_highest_csv",shape+"num_jets_pt20>1&&second_highest_csv>-100","(weightppb*20000)",
  		 ";2^{nd}-Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  		 "",0.244,0.679,
  		 "2nd_csv");
   draw_from_trees("third_highest_csv",shape+"num_jets_pt20>2&&third_highest_csv>-100","(weightppb*20000)",
  		 ";3^{rd}-Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  		 "",0.244,0.679,
  		 "3rd_csv");
   draw_from_trees("fourth_highest_csv",shape+"num_jets_pt20>3&&fourth_highest_csv>-100","(weightppb*20000)",
  		 ";4^{th}-Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  		 "",0.244,0.679,
  		 "4th_csv");

 draw_from_trees("m_eff30",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  		 "",-1,-1,
  		 "m_eff30");
  draw_from_trees("m_eff30",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  		 "plotLog",-1,-1,
  		 "m_eff30_log");
  draw_from_trees("m_eff50",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  		 "",-1,-1,
  		 "m_eff50");
  draw_from_trees("m_eff50",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  		 "plotLog",-1,-1,
  		 "m_eff50_log");

  draw_from_trees("met_over_m_eff30",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 50, 0., 1.,
  		 "",-1,-1,
  		 "met_over_m_eff30");
  draw_from_trees("met_over_m_eff30",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 50, 0., 1.,
  		 "plotLog",-1,-1,
  		 "met_over_m_eff30_log");
  draw_from_trees("met_over_m_eff50",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 50, 0., 1.,
  		 "",-1,-1,
  		 "met_over_m_eff50");
  draw_from_trees("met_over_m_eff50",shape,"(weightppb*20000)",
  		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 50, 0., 1.,
  		 "plotLog",-1,-1,
  		 "met_over_m_eff50_log");

  draw_from_trees("met_over_sqrt_ht30",shape,"(weightppb*20000)",
  		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 40, 0., 80.,
  		 "",-1,-1,
  		 "met_over_sqrt_ht30");
  draw_from_trees("met_over_sqrt_ht30",shape,"(weightppb*20000)",
  		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 40, 0., 80.,
  		 "plotLog",-1,-1,
  		 "met_over_sqrt_ht30_log");
  draw_from_trees("met_over_sqrt_ht50",shape,"(weightppb*20000)",
  		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 40, 0., 80.,
  		 "",-1,-1,
  		 "met_over_sqrt_ht50");
  draw_from_trees("met_over_sqrt_ht50",shape,"(weightppb*20000)",
  		 ";E^{T}_{miss}/#sqrt{H_{T}} [GeV^{1/2}];Events / 20 fb^{-1}", 40, 0., 80.,
  		 "plotLog",-1,-1,
  		 "met_over_sqrt_ht50_log");





  // Turn-on
  // draw_from_trees("jet1_pt","jet1_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 1} [GeV];Events / 20 fb^{-1}", 40, 0., 1000.,
  // 		  "plotSig",-1,-1,
  // 		  "jet1_pt_turnon");
  // draw_from_trees("jet2_pt","jet2_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 2} [GeV];Events / 20 fb^{-1}", 40, 0., 1000.,
  // 		  "plotSig",-1,-1,
  // 		  "jet2_pt_turnon");
  // draw_from_trees("jet3_pt","jet3_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 3} [GeV];Events / 20 fb^{-1}", 40, 0., 1000.,
  // 		  "plotSig",-1,-1,
  // 		  "jet3_pt_turnon");
  // draw_from_trees("jet4_pt","jet4_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 4} [GeV];Events / 20 fb^{-1}", 40, 0., 1000.,
  // 		  "plotSig",-1,-1,
  // 		  "jet4_pt_turnon");
  // draw_from_trees("jet5_pt","jet5_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 5} [GeV];Events / 20 fb^{-1}", 40, 0., 1000.,
  // 		  "plotSig",-1,-1,
  // 		  "jet5_pt_turnon");
  // draw_from_trees("jet1_pt","jet1_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 1} [GeV];Events / 20 fb^{-1}", 40, 0., 2000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "jet1_pt_turnon_log");
  // draw_from_trees("jet2_pt","jet2_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 2} [GeV];Events / 20 fb^{-1}", 40, 0., 2000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "jet2_pt_turnon_log");
  // draw_from_trees("jet3_pt","jet3_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 3} [GeV];Events / 20 fb^{-1}", 40, 0., 2000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "jet3_pt_turnon_log");
  // draw_from_trees("jet4_pt","jet4_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 4} [GeV];Events / 20 fb^{-1}", 40, 0., 2000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "jet4_pt_turnon_log");
  // draw_from_trees("jet5_pt","jet5_pt>0&&met>175&&ht50>500","(weightppb*20000)",
  // 		  ";p_{T}^{jet 5} [GeV];Events / 20 fb^{-1}", 40, 0., 2000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "jet5_pt_turnon_log");


  // draw_from_trees("ht20","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht20_turnon_log");
  // draw_from_trees("ht30","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht30_turnon_log");
  // draw_from_trees("ht40","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht40log_turnon");  
  // draw_from_trees("ht50","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht50log_turnon");
  // draw_from_trees("ht20","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plot",-1,-1,
  // 		  "ht20_turnon");
  // draw_from_trees("ht30","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plot",-1,-1,
  // 		  "ht30_turnon");
  // draw_from_trees("ht40","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plot",-1,-1,
  // 		  "ht40_turnon");  
  // draw_from_trees("ht50","met>175","(weightppb*20000)",
  // 		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  // 		  "plotSig:plot",-1,-1,
  // 		  "ht50_turnon");
  // draw_from_trees("met","ht50>500","(weightppb*20000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1750,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_turnon_log");
  // draw_from_trees("met","ht50>500","(weightppb*20000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1750,
  //  		  "plotSig",-1,-1,
  //  		  "met_turnon");
   
  
  // // trigger plateau
  //   draw_from_trees("ht50",start,"(weightppb*20000)",
  //   		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  //   		  "plotSig:plotLog",-1,-1,
  //   		  "ht50_log_trigger");
  //    draw_from_trees("ht20",start,"(weightppb*20000)",
  //   		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  //   		  "plotSig:plotLog",-1,-1,
  //   		  "ht20_log_trigger");
  //    draw_from_trees("mht",start,"(weightppb*20000)",
  //   		  ";H_{T}^{miss} [GeV];Events / 20 fb^{-1}", 150, 0., 1500.,
  //   		  "plotSig:plotLog",-1,-1,
  //   		  "mht_log_trigger");
  //   draw_from_trees("met",start,"(weightppb*20000)",
  //   		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1750,
  //   		  "plotSig:plotLog",-1,-1,
  //   		  "met_log_trigger");
  //   draw_from_trees("ht50",start,"(weightppb*20000)",
  //   		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  //   		  "plotSig",-1,-1,
  //   		  "ht50_trigger");
  //   draw_from_trees("ht20",start,"(weightppb*20000)",
  //   		  ";H_{T} [GeV];Events / 20 fb^{-1}", 50, 0., 4000.,
  //   		  "plotSig",-1,-1,
  //   		  "ht20_trigger");
  //   draw_from_trees("met",start,"(weightppb*20000)",
  //   		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1750,
  //   		  "plotSig",-1,-1,
  //   		  "met_trigger");
  //   draw_from_trees("mht",start,"(weightppb*20000)",
  //   		  ";H_{T}^{miss} [GeV];Events / 20 fb^{-1}", 150, 0., 1500.,
  //   		  "plotSig",-1,-1,
  //   		  "mht_trigger");
  //   draw_from_trees("num_jets_pt20",start,"(weightppb*20000)",
  //   		  ";n_{jets} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "",-1,-1,
  //   		  "njets20_trigger");
  //   draw_from_trees("num_jets_pt50",start,"(weightppb*20000)",
  //   		  ";n_{jets} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "njets50_trigger");
  //   draw_from_trees("num_jets_pt70",start,"(weightppb*20000)",
  //   		  ";n_{jets} (p_{T} > 70 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "njets70_trigger");
  //  draw_from_trees("num_jets_pt150",start,"(weightppb*20000)",
  //   		  ";n_{jets} (p_{T} > 150 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "njets150_trigger");
  //  draw_from_trees("num_csvl_jets",start,"(weightppb*20000)",
  //   		  ";n_{CSVL} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "ncsvl50_trigger");
  //   draw_from_trees("num_csvl_jets20",start,"(weightppb*20000)",
  //   		  ";n_{CSVL} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "ncsvl20_trigger");
  //  draw_from_trees("num_csvm_jets20",start,"(weightppb*20000)",
  //   		  ";n_{CSVM} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "ncsvm20_trigger");
  //  draw_from_trees("num_csvt_jets20",start,"(weightppb*20000)",
  //   		  ";n_{CSVT} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  //   		  "plotSig",-1,-1,
  //   		  "ncsvt20_trigger");
  // draw_from_trees("jet1_pt",start,"(weightppb*20000)",
  //   		  ";p_{T}(jet 1) [GeV];Events / 20 fb^{-1}", 50, 0., 1000.,
  //   		  "plotLog",-1,-1,
  //   		  "jet1_pt_log_trigger");
  // draw_from_trees("jet2_pt","num_jets_pt20>0","(weightppb*20000)",
  //   		  ";p_{T}(jet 2) [GeV];Events / 20 fb^{-1}", 50, 0., 1000.,
  //   		  "plotLog",-1,-1,
  //   		  "jet2_pt_log_trigger");
  // draw_from_trees("jet3_pt","num_jets_pt20>1","(weightppb*20000)",
  //   		  ";p_{T}(jet 3) [GeV];Events / 20 fb^{-1}", 50, 0., 1000.,
  //   		  "plotLog",-1,-1,
  //   		  "jet3_pt_log_trigger");
  //  draw_from_trees("highest_csv",start,"(weightppb*20000)",
  // 		 ";Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  // 		 "",0.244,0.679,
  // 		 "1st_csv_trigger");
  //  draw_from_trees("second_highest_csv",start,"(weightppb*20000)",
  // 		 ";2^{nd}-Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  // 		 "",0.244,0.679,
  // 		 "2nd_csv_trigger");
  //  draw_from_trees("third_highest_csv",start,"(weightppb*20000)",
  // 		 ";3^{rd}-Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  // 		 "",0.244,0.679,
  // 		 "3rd_csv_trigger");
  //  draw_from_trees("fourth_highest_csv",start,"(weightppb*20000)",
  // 		 ";4^{th}-Highest CSV;Events / 20 fb^{-1}", 50, 0., 1.,
  // 		 "",0.244,0.679,
  // 		 "4th_csv_trigger");
  // draw_from_trees("m_eff20",start,"(weightppb*20000)",
  // 		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  // 		 "",-1,-1,
  // 		 "m_eff20_trigger");
  // draw_from_trees("m_eff50",start,"(weightppb*20000)",
  // 		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  // 		 "",-1,-1,
  // 		 "m_eff50_trigger");
  // draw_from_trees("m_eff20",start,"(weightppb*20000)",
  // 		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  // 		 "plotLog",-1,-1,
  // 		 "m_eff20_log_trigger");
  // draw_from_trees("m_eff50",start,"(weightppb*20000)",
  // 		 ";M_{eff} [GeV];Events / 20 fb^{-1}", 75, 0., 7500.,
  // 		 "plotLog",-1,-1,
  // 		 "m_eff50_log_trigger");

  //  draw_from_trees("min_delta_phi_met_N",baseline,"(weightppb*20000)",
  //  		  ";#Delta#hat{#phi}_{N};Events / 20 fb^{-1}", 40, 0., 40.,
  //  		  "plotSig",4,-1,
  //  		  "min_delta_phi_met_N_cutflow");
  //  draw_from_trees("min_delta_phi_met_N",baseline,"(weightppb*20000)",
  //  		  ";#Delta#hat{#phi}_{N}^{min};Events / 20 fb^{-1}", 40, 0., 40.,
  //  		  "plotSig:plotLog",4,-1,
  //  		  "min_delta_phi_met_N_cutflow_log");
  //  draw_from_trees("num_reco_veto_electrons",baseline+"min_delta_phi_met_N>4","(weightppb*20000)",
  //  		  ";n_{e}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig",-1,1,
  //  		  "num_electrons_cutflow");
  //  draw_from_trees("num_reco_veto_muons",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0","(weightppb*20000)",
  //  		  ";n_{#mu}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig",-1,1,
  //  		  "num_muons_cutflow");
  //  draw_from_trees("num_reco_veto_electrons",baseline+"min_delta_phi_met_N>4","(weightppb*20000)",
  //  		  ";n_{e}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig:plotLog",-1,1,
  //  		  "num_electrons_cutflow_log");
  //  draw_from_trees("num_reco_veto_muons",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0","(weightppb*20000)",
  //  		  ";n_{#mu}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig:plotLog",-1,1,
  //  		  "num_muons_cutflow_log");
  //  draw_from_trees("num_iso_tracks",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","(weightppb*20000)",
  //  		  ";n_{isotk}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig",-1,1,
  //  		  "num_isotk_cutflow");
  //  draw_from_trees("num_iso_tracks",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","(weightppb*20000)",
  //  		  ";n_{isotk}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig:plotLog",-1,1,
  //  		  "num_isotk_cutflow_log");
  //  draw_from_trees("num_csvm_jets",search,"(weightppb*20000)",
  //  		  ";n_{CSVM}, p_{T} > 50 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig",-1,-1,
  //  		  "num_csvm_cutflow");
  //  draw_from_trees("num_jets_pt70",search+"num_csvm_jets>0","(weightppb*20000)",
  //  		  ";n_{CSVT}, p_{T} > 70 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  //  		  "plotSig",-1,-1,
  //  		  "num_jets70_1bm");
  // draw_from_trees("num_csvt_jets",search,"(weightppb*20000)",
  //  		  ";n_{CSVT}, p_{T} > 50 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig",-1,-1,
  //  		  "num_csvt_search");
  //  draw_from_trees("num_csvt_jets",search,"(weightppb*20000)",
  //  		  ";n_{CSVT}, p_{T} > 50 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "num_csvt_search_log");
  //  draw_from_trees("num_jets_pt70",search+"num_csvt_jets>=2","(weightppb*20000)",
  //  		  ";n_{CSVT}, p_{T} > 70 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  //  		  "plotSig",-1,-1,
  //  		  "num_jets70_search");
  //  draw_from_trees("num_jets_pt100",search+"num_csvm_jets>=2","(weightppb*20000)",
  //  		  ";n_{CSVT}, p_{T} > 100 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  //  		  "plotSig",-1,-1,
  //  		  "num_jets100_search");
  //  draw_from_trees("num_jets_pt150",search+"num_csvm_jets>=2","(weightppb*20000)",
  //  		  ";n_{CSVT}, p_{T} > 150 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  //  		  "plotSig",-1,-1,
  //  		  "num_jets150_search");
  // draw_from_trees("ht50/1000.",search+"num_csvt_jets>=1","(weightppb*20000)",
  //  		  ";H_{T} [TeV];Events / 20 fb^{-1}", 46, 0.4, 5.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "ht_search_1bt");
  //  draw_from_trees("met",search+"num_csvt_jets>=1","(weightppb*20000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125, 2000,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_search_1bt");
  //  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=2","(weightppb*20000)",
  //  		  ";H_{T} [TeV];Events / 20 fb^{-1}", 46, 0.4, 5.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "ht_search_2bt");
  //  draw_from_trees("met",search+"num_csvt_jets>=2","(weightppb*20000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_search_2bt");
  //  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=3","(weightppb*20000)",
  //  		  ";H_{T} [TeV];Events / 20 fb^{-1}", 46, 0.4, 5.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "ht_search_3bt");
  //  draw_from_trees("met",search+"num_csvt_jets>=3","(weightppb*20000)",
  //  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  //  		  "plotSig:plotLog",-1,-1,
  //  		  "met_search_3bt");
  return 0;
  
}

