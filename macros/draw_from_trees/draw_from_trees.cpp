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

TChain* ttbar_ch;
TChain* qcd_ch;
TChain* znn_ch;
TChain* wjets_ch;
TChain* single_top_ch;
TChain* other_ch;
TChain* t1tttt_1500_100_ch;
TChain* t1tttt_1200_800_ch;
TChain* t1bbbb_1500_100_ch;
TChain* t1bbbb_1000_900_ch;

TChain* ttbar_;
TChain* qcd_;
TChain* znn_;
TChain* wjets_;
TChain* single_top_;
TChain* other_;
TChain* t1tttt_1500_100_;
TChain* t1tttt_1200_800_;
TChain* t1bbbb_1500_100_;
TChain* t1bbbb_1000_900_;

using namespace std;

TString plotdir = "plots/";

void draw_from_trees(TString var, TCut other_cuts,
		     TString weights, TString title, int nbinsx, 
		     double xlow, double xup,
		     TString options="plotSig:plotLog:plotData",
		     double cut_low=-1, double cut_high=-1,
		     TString plot_title="default")
{

  bool plotSig = options.Contains("plotSig") && (!options.Contains("!plotSig"));
  bool plotLog = options.Contains("plotLog") && (!options.Contains("!plotLog"));
  bool plotData = options.Contains("plotData") && (!options.Contains("!plotData"));

  // Book histograms
  TH1D * httbar = new TH1D("ttbar" , title, nbinsx, xlow, xup);
  TH1D * hqcd = new TH1D("qcd" , title, nbinsx, xlow, xup);
  TH1D * hznn = new TH1D("znn" , title, nbinsx, xlow, xup);
  TH1D * hwjets = new TH1D("wjets" , title, nbinsx, xlow, xup);
  TH1D * hother = new TH1D("other" , title, nbinsx, xlow, xup);
  TH1D * hmc_exp = new TH1D("mc_exp" , title, nbinsx, xlow, xup);
  TH1D * hsingle_top = new TH1D("single_top" , title, nbinsx, xlow, xup);  
  TH1D * ht1bbbb_1500_100 = new TH1D("t1bbbb_1500_100" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(other_cuts);
  cout << "Filling histograms for " << var.Data() << endl;
  ttbar_ch->Project("ttbar",var,cut*weights);
  qcd_ch->Project("qcd",var,cut*weights);
  znn_ch->Project("znn",var,cut*weights);
  wjets_ch->Project("wjets",var,cut*weights);
  other_ch->Project("other",var,cut*weights);
  single_top_ch->Project("single_top",var,cut*weights);
  t1bbbb_1500_100_ch->Project("t1bbbb_1500_100",var,cut*weights);

  
  
  // Add up MC histograms
  hmc_exp->Add(httbar);
  hmc_exp->Add(hqcd);
  hmc_exp->Add(hznn);
  hmc_exp->Add(hwjets);
  hmc_exp->Add(hsingle_top);
  hmc_exp->Add(hother);
  cout << "... DONE: add all backgrounds to mc_exp." << endl;
  
  Double_t ttbar_e(0.), qcd_e(0.), znn_e(0.), wjets_e(0.), other_e(0.), single_top_e(0.), bg_tot_e(0.), t1bbbb_1500_100_e(0.);
  double ttbar_n(httbar->IntegralAndError(0,nbinsx+1, ttbar_e));
  double qcd_n(hqcd->IntegralAndError(0,nbinsx+1, qcd_e));
  double znn_n(hznn->IntegralAndError(0,nbinsx+1, znn_e));
  double wjets_n(hwjets->IntegralAndError(0,nbinsx+1, wjets_e));
  double other_n(hother->IntegralAndError(0,nbinsx+1, other_e));
  double single_top_n(hsingle_top->IntegralAndError(0,nbinsx+1, single_top_e));
  double bg_tot(hmc_exp->IntegralAndError(0,nbinsx+1, bg_tot_e));
  double t1bbbb_1500_100_n(ht1bbbb_1500_100->IntegralAndError(0,nbinsx+1, t1bbbb_1500_100_e));


  printf("Counts before cut: %s\n",var.Data());
  printf("&ttbar&qcd&znn&wjets&single top&other&t1bbbb_1500_100\\\\ \n");
  printf("%s & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f \\\\\n",
	 var.Data(),
	 ttbar_n,ttbar_e,
	 qcd_n,qcd_e,
	 znn_n,znn_e,
	 wjets_n,wjets_e,
	 single_top_n,single_top_e,
	 other_n,other_e,
	 bg_tot,bg_tot_e,
	 t1bbbb_1500_100_n,t1bbbb_1500_100_e);


  cout << "... DONE: filled histograms." << endl;
  
  THStack * hs = new THStack("hs", "");
  hs->Add(hother);
  hs->Add(hznn);
  hs->Add(hsingle_top);
  hs->Add(hwjets);
  hs->Add(httbar);
  hs->Add(hqcd);

  //hs->GetYaxis()->SetTitle("Events / 5 fb^{-1}");
  //hs->GetXaxis()->SetTitle(httbar->GetXaxis()->GetTitle());

  // Setup histogram styles
  set_style(httbar, "ttbar");
  set_style(hqcd, "qcd");
  set_style(hznn, "znn");
  set_style(hwjets, "wjets");
  set_style(hother, "other");
  set_style(hsingle_top, "single_top");
  set_style(ht1bbbb_1500_100, "t1bbbb_1500_100");

  // Setup auxiliary histograms (ratios, errors, etc)
  TH1D * staterr = (TH1D *) hmc_exp->Clone("staterr");
  staterr->Sumw2();
  //staterr->SetFillColor(kRed);
  staterr->SetFillColor(kGray+3);
  staterr->SetMarkerSize(0);
  staterr->SetFillStyle(3013);

 
  // Setup legends
  TLegend * leg1 = new TLegend(0.50, 0.68, 0.72, 0.92);
  set_style(leg1,0.035);
  if (plotData) leg1->AddEntry(hsingle_top, "Data", "pel");
  if (plotSig) leg1->AddEntry(ht1bbbb_1500_100, "T1bbbb", "l");
  leg1->AddEntry(httbar, "t#bar{t}", "f");
  leg1->AddEntry(hqcd, "QCD", "f");
  leg1->AddEntry(hznn, "Z+jets", "f");

  TLegend * leg2 = new TLegend(0.72, 0.68, 0.94, 0.92);
  set_style(leg2,0.035);
  leg2->AddEntry(hwjets, "W+jets", "f");
  leg2->AddEntry(hsingle_top, "Single Top", "f");
  leg2->AddEntry(hother, "Other", "f");
  leg2->AddEntry(staterr, "MC uncert.", "f");

  double ymax = hs->GetMaximum();
  if(plotLog) {
    hs->SetMaximum(11*ymax);
    hs->SetMinimum(0.1);
  }
  else hs->SetMaximum(1.1*ymax);
  // Vertical lines for cuts
  TLine* line_low = new TLine(cut_low,0,cut_low,1.5*ymax);
  TLine* line_high = new TLine(cut_high,0,cut_high,1.5*ymax);
  set_style(line_low);
  set_style(line_high);

  // Setup canvas and pads  
  TCanvas * c1 = new TCanvas("c1", "c1", 700, 700);
  TPad * pad1 = new TPad("pad1", "top pad" , 0.0, 0.3, 1.0, 1.0);
  TPad * pad2 = new TPad("pad2", "bottom pad", 0.0, 0.0, 1.0, 0.3);
  if(plotData) {
    pad1->SetBottomMargin(0.0);
    pad1->Draw();
    pad2->SetTopMargin(0.0);
    pad2->SetBottomMargin(0.35);
    pad2->Draw();
    pad1->cd();
    pad1->SetLogy(plotLog);
  }
  else {
    c1->cd();
    c1->SetLogy(plotLog);
  }
  // Draw hists
  hs->Draw("hist");
  if (plotData) 
    {
      //  hsingle_top->Draw("e1 same");
      hs->GetXaxis()->SetLabelSize(0);
    }
  staterr->Draw("e2 same");
  if (plotSig) {
    ht1bbbb_1500_100->SetLineColor(2);
    ht1bbbb_1500_100->SetLineWidth(3);
    ht1bbbb_1500_100->SetFillColor(0);
    ht1bbbb_1500_100->Draw("hist same");
  }
  if (cut_low>0) line_low->Draw("same");
  if (cut_high>0) line_high->Draw("same");


  // Draw legends
  leg1->Draw();
  leg2->Draw();
  TLatex * latex = new TLatex();
  latex->SetNDC();
  latex->SetTextAlign(12);
  latex->SetTextFont(62);
  latex->SetTextSize(0.042);
  latex->DrawLatex(0.19, 0.89, "CMS Simulation");
  latex->SetTextSize(0.03);
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 5 fb^{-1}");
  
  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  if (plot_title.EqualTo("default")) plot_title=plotdir+var;
  gPad->Print(plotdir+plot_title+".pdf");

  TFile* outrootfile = TFile::Open(plotdir+plot_title+".root", "RECREATE");
  httbar->Write();
  hqcd->Write();
  hznn->Write();
  hwjets->Write();
  hother->Write();
  ht1bbbb_1500_100->Write();
  hmc_exp->Write();
  hsingle_top->Write();
  outrootfile->Close();

  // Clean up
  delete staterr;
  delete leg1;
  delete leg2;
  delete latex;
  // delete pave;
  delete hs;
  delete pad1;
  delete pad2;
  delete c1;

  delete httbar;
  delete hqcd;
  delete hznn;
  delete hwjets;
  delete hsingle_top;
  delete hother;
  delete hmc_exp;
  delete ht1bbbb_1500_100;

  cout << "MakePlots(): DONE!" << endl;

  return;
}

int main() {

  TH1::SetDefaultSumw2(1);
  //gROOT->SetBatch(1);


  if (gSystem->AccessPathName(plotdir))
    gSystem->mkdir(plotdir);

  // Setup style
  cout << "Setting tdr style."  << endl;
  TStyle *tdrStyle = new TStyle("tdrStyle","Style for P-TDR");
  setTDRStyle(tdrStyle);
  tdrStyle->cd();

  ttbar_ = new TChain("reduced_tree");
  qcd_ = new TChain("reduced_tree");
  znn_ = new TChain("reduced_tree");
  wjets_ = new TChain("reduced_tree");
  single_top_ = new TChain("reduced_tree");
  other_ = new TChain("reduced_tree");
  t1tttt_1500_100_ = new TChain("reduced_tree");
  t1tttt_1200_800_ = new TChain("reduced_tree");
  t1bbbb_1500_100_ = new TChain("reduced_tree");
  t1bbbb_1000_900_ = new TChain("reduced_tree");

  ttbar_->Add("../../reduced_trees/TTJets*.root");
  qcd_->Add("../../reduced_trees/QCD_Pt-*.root");
  znn_->Add("../../reduced_trees/ZJets*.root");
  wjets_->Add("../../reduced_trees/WJets*.root");
  single_top_->Add("../../reduced_trees/TTo*.root");
  single_top_->Add("../../reduced_trees/TBarTo*.root");
  single_top_->Add("../../reduced_trees/*tW*.root");
  other_->Add("../../reduced_trees/*HToBB*.root");
  other_->Add("../../reduced_trees/TTbarH*.root");
  t1tttt_1500_100_->Add("../../reduced_trees/SMS-T1tttt_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1-MINIAODSIM_cfA_v74.list.root");
  t1tttt_1200_800_->Add("../../reduced_trees/SMS-T1tttt_2J_mGl-1200_mLSP-800_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1-MINIAODSIM_cfA_v74.list.root");
  t1bbbb_1500_100_->Add("../../reduced_trees/SMS-T1bbbb_2J_mGl-1500_mLSP-100_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v2-MINIAODSIM_cfA_v74.list.root");
  t1bbbb_1000_900_->Add("../../reduced_trees/SMS-T1bbbb_2J_mGl-1000_mLSP-900_Tune4C_13TeV-madgraph-tauola-Spring14miniaod-PU20bx25_POSTLS170_V5-v1-MINIAODSIM_cfA_v74.list.root");

  // quick skim
  TCut start("passesPVCut&&((ht40>350&&met>100)||(ht40>650)||(num_jets_pt50>1&&met>80))");
  TCut baseline(start+"ht50>400&&met>125&&num_jets_pt50>=3&&num_jets_pt70>=2");
  TCut search(baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_iso_tracks==0");
  // cout << "Applying trigger cuts: \n" << (string)start << endl;
  cout << "Applying trigger cuts: \n" << (string)baseline << endl;
  ttbar_ch = (TChain*)ttbar_->CopyTree(baseline);
  cout << "... DONE: ttbar copy tree. N=" << ttbar_ch->GetEntries() << endl;
  qcd_ch = (TChain*)qcd_->CopyTree(baseline);
  cout << "... DONE: qcd copy tree. N=" << qcd_ch->GetEntries() << endl;
  znn_ch = (TChain*)znn_->CopyTree(baseline);
  cout << "... DONE: znn copy tree. N=" << znn_ch->GetEntries() << endl;
  wjets_ch = (TChain*)wjets_->CopyTree(baseline);
  cout << "... DONE: wjets copy tree. N=" << wjets_ch->GetEntries() << endl;
  single_top_ch = (TChain*)single_top_->CopyTree(baseline);
  cout << "... DONE: single_top copy tree. N=" << single_top_ch->GetEntries() << endl;
  other_ch = (TChain*)other_->CopyTree(baseline);
  cout << "... DONE: other copy tree. N=" << other_ch->GetEntries() << endl;
  t1tttt_1500_100_ch = (TChain*)t1tttt_1500_100_->CopyTree(baseline);
  cout << "... DONE: t1tttt_1500_100 copy tree. N=" << t1tttt_1500_100_ch->GetEntries() << endl;
  t1tttt_1200_800_ch = (TChain*)t1tttt_1200_800_->CopyTree(baseline);
  cout << "... DONE: t1tttt_1200_800 copy tree. N=" << t1tttt_1200_800_ch->GetEntries() << endl;
  t1bbbb_1500_100_ch = (TChain*)t1bbbb_1500_100_->CopyTree(baseline);
  cout << "... DONE: t1bbbb_1500_100 copy tree. N=" << t1bbbb_1500_100_ch->GetEntries() << endl;
  t1bbbb_1000_900_ch = (TChain*)t1bbbb_1000_900_->CopyTree(baseline);
  cout << "... DONE: t1bbbb_1000_900 copy tree. N=" << t1bbbb_1000_900_ch->GetEntries() << endl;

  /*draw_from_trees(TString var, TCut other_cuts,
    TString weights, TString title, int nbinsx, 
    double xlow, double xup,
    TString options="plotSig:plotLog:plotData",
    double cut_low=-1, double cut_high=-1,
    TString plot_title="default")
  */


  // draw_from_trees("ht50",start,"(weightppb*5000)",
  // 		  ";H_{T} [GeV];Events / 5 fb^{-1}", 120, 400., 3400.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "ht_cutflow");
  // draw_from_trees("met",start+"ht50>400","(weightppb*5000)",
  // 		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 35, 125., 1000.,
  // 		  "plotSig:plotLog",-1,-1,
  // 		  "met_cutflow");
  // draw_from_trees("num_jets_pt50",start+"ht50>400&&met>125","(weightppb*5000)",
  // 		  ";n_{jets} (p_{T} > 50 GeV);Events / 5 fb^{-1}", 13, 0., 13.,
  // 		  "plotSig",-1,-1,
  // 		  "njets50_cutflow");
  // draw_from_trees("num_jets_pt70",start+"ht50>400&&met>125&&num_jets_pt50>=3","(weightppb*5000)",
  // 		  ";n_{jets} (p_{T} > 70 GeV);Events / 5 fb^{-1}", 13, 0., 13.,
  // 		  "plotSig",-1,-1,
  // 		  "njets70_cutflow");
  draw_from_trees("min_delta_phi_met_N",baseline,"(weightppb*5000)",
  		  ";#Delta#hat{Phi}_{N});Events / 5 fb^{-1}", 20, 0., 40.,
  		  "plotSig:plotLog",4,-1,
  		  "min_delta_phi_met_N_cutflow");
  draw_from_trees("num_reco_veto_electrons",baseline+"min_delta_phi_met_N>4","(weightppb*5000)",
  		  ";n_{e}, p_{T} > 10 GeV;Events", 5, 0., 5.,
  		  "plotSig:plotLog",-1,1,
  		  "num_electrons_cutflow");
  draw_from_trees("num_reco_veto_muons",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0","(weightppb*5000)",
  		  ";n_{#mu}, p_{T} > 10 GeV;Events", 5, 0., 5.,
  		  "plotSig:plotLog",-1,1,
  		  "num_muons_cutflow");
  draw_from_trees("num_iso_tracks",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","(weightppb*5000)",
  		  ";n_{isotk}, p_{T} > 10 GeV;Events", 5, 0., 5.,
  		  "plotSig:plotLog",-1,1,
  		  "num_isotk_cutflow");
  draw_from_trees("num_csvm_jets",search,"(weightppb*5000)",
  		  ";n_{CSVM}, p_{T} > 50 GeV;Events", 5, 0., 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_csvm_cutflow");
  draw_from_trees("num_csvt_jets",search+"num_csvm_jets>=1","(weightppb*5000)",
  		  ";n_{CSVT}, p_{T} > 50 GeV;Events", 5, 0., 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_csvt_search");
  draw_from_trees("num_jets_pt70",search+"num_csvm_jets>=1","(weightppb*5000)",
  		  ";n_{CSVT}, p_{T} > 70 GeV;Events", 13, 0., 13.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_jets70_search");
  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=1","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 46, 0.4, 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_search_1bt");
  draw_from_trees("met",search+"num_csvt_jets>=1","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 75, 125, 2000,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_1bt");
  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=2","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 46, 0.4, 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_search_2bt");
  draw_from_trees("met",search+"num_csvt_jets>=2","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_2bt");
  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=3","(weightppb*5000)",
  		  ";H_{T} [GeV];Events / 5 fb^{-1}", 46, 0.4, 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_search_3bt");
  draw_from_trees("met",search+"num_csvt_jets>=3","(weightppb*5000)",
  		  ";E_{T}^{miss} [GeV];Events / 5 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_3bt");
  return 0;
  
}

