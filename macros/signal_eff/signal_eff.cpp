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
  bool highMass = options.Contains("highMass") && (!options.Contains("!highMass"));

  // Book histograms
   TH1D * ht1bbbb_1500_100 = new TH1D("t1bbbb_1500_100" , title, nbinsx, xlow, xup);
   TH1D * ht1bbbb_1000_900 = new TH1D("t1bbbb_1000_900" , title, nbinsx, xlow, xup);
   TH1D * ht1tttt_1500_100 = new TH1D("t1tttt_1500_100" , title, nbinsx, xlow, xup);
   TH1D * ht1tttt_1200_800 = new TH1D("t1tttt_1200_800" , title, nbinsx, xlow, xup);

  // Format cuts
  TCut cut(other_cuts);
  cout << "Filling histograms for " << var.Data() << endl;
  t1bbbb_1500_100_ch->Project("t1bbbb_1500_100",var,cut*weights);
  t1bbbb_1000_900_ch->Project("t1bbbb_1000_900",var,cut*weights);
  t1tttt_1500_100_ch->Project("t1tttt_1500_100",var,cut*weights);
  t1tttt_1200_800_ch->Project("t1tttt_1200_800",var,cut*weights);
  
  //Double_t ttbar_e(0.), qcd_e(0.), znn_e(0.), wjets_e(0.), other_e(0.), single_top_e(0.), bg_tot_e(0.);
  Double_t t1bbbb_1500_100_e(0.), t1bbbb_1000_900_e(0.), t1tttt_1500_100_e(0.),t1tttt_1200_800_e(0.) ;
  // double ttbar_n(httbar->IntegralAndError(0,nbinsx+1, ttbar_e));
  // double qcd_n(hqcd->IntegralAndError(0,nbinsx+1, qcd_e));
  // double znn_n(hznn->IntegralAndError(0,nbinsx+1, znn_e));
  // double wjets_n(hwjets->IntegralAndError(0,nbinsx+1, wjets_e));
  // double other_n(hother->IntegralAndError(0,nbinsx+1, other_e));
  // double single_top_n(hsingle_top->IntegralAndError(0,nbinsx+1, single_top_e));
  // double bg_tot(hmc_exp->IntegralAndError(0,nbinsx+1, bg_tot_e));
  double t1bbbb_1500_100_n(ht1bbbb_1500_100->IntegralAndError(0,nbinsx+1, t1bbbb_1500_100_e));
  double t1bbbb_1000_900_n(ht1bbbb_1000_900->IntegralAndError(0,nbinsx+1, t1bbbb_1000_900_e));
  double t1tttt_1500_100_n(ht1tttt_1500_100->IntegralAndError(0,nbinsx+1, t1tttt_1500_100_e));
  double t1tttt_1200_800_n(ht1tttt_1200_800->IntegralAndError(0,nbinsx+1, t1tttt_1200_800_e));

  printf("Counts before cut: %s\n",var.Data());
  printf("&t1bbbb_1500_100&t1bbbb_1000_900&t1tttt_1500_100&t1tttt_1200_800 \\\\ \n");
  printf("%s & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f & %3.2f+-%3.2f  \\\\\n",
	 var.Data(),
	 t1bbbb_1500_100_n,t1bbbb_1500_100_e,
	 t1bbbb_1000_900_n,t1bbbb_1000_900_e,
	 t1tttt_1500_100_n,t1tttt_1500_100_e,
	 t1tttt_1200_800_n,t1tttt_1200_800_e);


  cout << "... DONE: filled histograms." << endl;

  ht1bbbb_1500_100->SetLineColor(2);
  ht1bbbb_1000_900->SetLineColor(3);
  ht1tttt_1500_100->SetLineColor(4);
  ht1tttt_1200_800->SetLineColor(5);
  ht1bbbb_1500_100->SetLineWidth(3);
  ht1bbbb_1000_900->SetLineWidth(3);
  ht1tttt_1500_100->SetLineWidth(3);
  ht1tttt_1200_800->SetLineWidth(3);

  // Setup legends
  TLegend * leg1 = new TLegend(0.6, 0.68, 0.94, 0.92);
  set_style(leg1,0.035);
  leg1->AddEntry(ht1bbbb_1500_100, "T1bbbb(1500,100)", "l");
  if (!highMass) leg1->AddEntry(ht1bbbb_1000_900, "T1bbbb(1000,900)", "l");
  leg1->AddEntry(ht1tttt_1500_100, "T1tttt(1500,100)", "l");
  if (!highMass) leg1->AddEntry(ht1tttt_1200_800, "T1tttt(1200,800)", "l");

  
  double ymax = ht1bbbb_1000_900->GetMaximum();
  if (highMass) ymax = ht1bbbb_1500_100->GetMaximum();
  double binwidth = (xup - xlow) / nbinsx;
  TString ytitle = Form("Events / %.2f", binwidth);
  ht1bbbb_1000_900->GetYaxis()->SetTitle(ytitle);
  ht1bbbb_1500_100->GetYaxis()->SetTitle(ytitle);
  if(plotLog) {
    ht1bbbb_1000_900->SetMaximum(11*ymax);
    ht1bbbb_1000_900->SetMinimum(0.1);
    ht1bbbb_1500_100->SetMaximum(15*ymax);
    ht1bbbb_1500_100->SetMinimum(0.1);
  }
  else {
    ht1bbbb_1000_900->SetMaximum(1.5*ymax);
    ht1bbbb_1500_100->SetMaximum(1.5*ymax);
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
  if (!highMass) ht1bbbb_1000_900->Draw("hist");
  ht1bbbb_1000_900->GetXaxis()->SetLabelSize(0.04);
  if (highMass) ht1bbbb_1500_100->Draw("hist");
  else ht1bbbb_1500_100->Draw("hist same");
  ht1tttt_1500_100->Draw("hist same");
  if (!highMass) ht1tttt_1200_800->Draw("hist same");

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
  latex->DrawLatex(0.19, 0.84, "#sqrt{s} = 13 TeV, L = 20 fb^{-1}");
  
  // Print
  cout << "MakePlots(): Printing..." << endl;

  c1->cd();
  if (plot_title.EqualTo("default")) plot_title=plotdir+var;
  gPad->Print(plotdir+plot_title+".pdf");

  TFile* outrootfile = TFile::Open(plotdir+plot_title+".root", "RECREATE");
  ht1bbbb_1500_100->Write();
  ht1bbbb_1000_900->Write();
  ht1tttt_1500_100->Write();
  ht1tttt_1200_800->Write();

  outrootfile->Close();

  // Clean up
  delete leg1;
  delete latex;
  delete c1;


  delete ht1bbbb_1500_100;
  delete ht1bbbb_1000_900;
  delete ht1tttt_1500_100;
  delete ht1tttt_1200_800;

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

  // ttbar_->Add("../../reduced_trees/TTJets*.root");
  // qcd_->Add("../../reduced_trees/QCD_Pt-*.root");
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
  TCut start("passesPVCut&&((ht40>350&&met>100)||(ht40>650)||(num_jets_pt50>1&&met>80))");
  TCut baseline(start+"ht50>400&&met>125&&num_jets_pt50>=3&&num_jets_pt70>=2");
  TCut search(baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0&&num_iso_tracks==0");
  TCut trigger("passesPVCut&&((ht40>350&&met>120)||(ht40>900)||(met>170)||( highest_csv>=0.7&&met>120))");  
  TCut highMass(trigger+"ht50>1000.&&met>200.&&jet2_pt>150.&&jet3_pt>100.");
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

  // No cuts
  cout << "No cuts" << endl;
  draw_from_trees("ht50","","(weightppb*20000)",
  		  ";H_{T} [GeV];Events / 20 fb^{-1}", 200, 0., 4000.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_log");
  draw_from_trees("mht","","(weightppb*20000)",
  		  ";H_{T}^{miss} [GeV];Events / 20 fb^{-1}", 150, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "mht_log");
  draw_from_trees("met","","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1500.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_log");
  draw_from_trees("ht50","","(weightppb*20000)",
  		  ";H_{T} [GeV];Events / 20 fb^{-1}", 200, 0., 4000.,
  		  "plotSig",-1,-1,
  		  "ht");
  draw_from_trees("met","","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1500.,
  		  "plotSig",-1,-1,
  		  "met");
  draw_from_trees("ht50","","(weightppb*20000)",
  		  ";H_{T} [GeV];Events / 20 fb^{-1}", 200, 0., 4000.,
  		  "highMass",-1,-1,
  		  "ht_highMass");
  draw_from_trees("met","","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 50, 0., 1500.,
  		  "highMass",-1,-1,
  		  "met_highMass");
  draw_from_trees("num_jets_pt50","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "njets50");
  draw_from_trees("num_jets_pt70","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 70 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "njets70");
  draw_from_trees("num_jets_pt50","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotLog",-1,-1,
  		  "njets50_log");
  draw_from_trees("num_jets_pt70","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 70 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotLog",-1,-1,
  		  "njets70_log");
  draw_from_trees("num_jets_pt20","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 20 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "",-1,-1,
  		  "njets20");
  draw_from_trees("num_jets_pt100","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 100 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "njets50");
  draw_from_trees("num_jets_pt150","","(weightppb*20000)",
  		  ";n_{jets} (p_{T} > 150 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "njets150");
 draw_from_trees("num_csvl_jets","","(weightppb*20000)",
  		  ";n_{CSVL} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "ncsvl50");
 draw_from_trees("num_csvm_jets","","(weightppb*20000)",
  		  ";n_{CSVM} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "ncsvm50");
 draw_from_trees("num_csvt_jets","","(weightppb*20000)",
  		  ";n_{CSVT} (p_{T} > 50 GeV);Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "ncsvt50");
draw_from_trees("jet1_pt","","(weightppb*20000)",
  		  ";p_{T}(jet 1) [GeV];Events / 20 fb^{-1}", 50, 0., 500.,
  		  "plotLog",-1,-1,
  		  "jet1_pt_log");
draw_from_trees("jet2_pt","num_jets_pt20>0","(weightppb*20000)",
  		  ";p_{T}(jet 2) [GeV];Events / 20 fb^{-1}", 50, 0., 500.,
  		  "plotLog",-1,-1,
  		  "jet2_pt_log");
draw_from_trees("jet3_pt","num_jets_pt20>1","(weightppb*20000)",
  		  ";p_{T}(jet 3) [GeV];Events / 20 fb^{-1}", 50, 0., 500.,
  		  "plotLog",-1,-1,
  		  "jet3_pt_log");
 draw_from_trees("highest_csv","","(weightppb*20000)",
		 ";Highest CSV;Events / 20 fb^{-1}", 100, 0., 1.,
		 "",0.244,0.679,
		 "1st_csv");
 draw_from_trees("second_highest_csv","","(weightppb*20000)",
		 ";2^{nd}-Highest CSV;Events / 20 fb^{-1}", 100, 0., 1.,
		 "",0.244,0.679,
		 "2nd_csv");
 draw_from_trees("third_highest_csv","","(weightppb*20000)",
		 ";3^{rd}-Highest CSV;Events / 20 fb^{-1}", 100, 0., 1.,
		 "",0.244,0.679,
		 "3rd_csv");
 draw_from_trees("fourth_highest_csv","","(weightppb*20000)",
		 ";4^{th}-Highest CSV;Events / 20 fb^{-1}", 100, 0., 1.,
		 "",0.244,0.679,
		 "4th_csv");
// trigger plateau
 cout << "cutflow begins" << endl;
  draw_from_trees("min_delta_phi_met_N",highMass,"(weightppb*20000)",
  		  ";#Delta#hat{#phi}_{N};Events / 20 fb^{-1}", 40, 0., 40.,
  		  "plotSig",4,-1,
  		  "min_delta_phi_met_N_cutflow");
  draw_from_trees("min_delta_phi_met_N",highMass,"(weightppb*20000)",
  		  ";#Delta#hat{#phi}_{N}^{min};Events / 20 fb^{-1}", 40, 0., 40.,
  		  "plotSig:plotLog",4,-1,
  		  "min_delta_phi_met_N_cutflow_log");
  draw_from_trees("num_reco_veto_electrons",highMass+"min_delta_phi_met_N>4","(weightppb*20000)",
  		  ";n_{e}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig",-1,1,
  		  "num_electrons_cutflow");
  draw_from_trees("num_reco_veto_muons",highMass+"min_delta_phi_met_N>4","(weightppb*20000)",
  		  ";n_{#mu}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig",-1,1,
  		  "num_muons_cutflow");
  draw_from_trees("num_reco_veto_electrons",highMass+"min_delta_phi_met_N>4&&num_reco_veto_muons==0","(weightppb*20000)",
  		  ";n_{e}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig:plotLog",-1,1,
  		  "num_electrons_cutflow_log");
  draw_from_trees("num_csvm_jets",highMass+"min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0","(weightppb*20000)",
  		  ";n_{CSVM}, p_{T} > 50 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig",-1,-1,
  		  "num_csvm_cutflow");
  cout << "b-jets" << endl;
  TCut base(highMass+"min_delta_phi_met_N>4&&num_reco_veto_muons==0&&num_reco_veto_electrons==0");
  draw_from_trees("met",base,"(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_no_b_req");
  draw_from_trees("met",base+"num_csvm_jets>=1","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_bm");
  draw_from_trees("met",base+"num_csvm_jets==1","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_1bm");
  draw_from_trees("met",base+"num_csvm_jets==2","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_2bm");
  draw_from_trees("met",base+"num_csvm_jets>=3","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_3bm");


  draw_from_trees("num_reco_veto_muons",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0","(weightppb*20000)",
  		  ";n_{#mu}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig:plotLog",-1,1,
  		  "num_muons_cutflow_log");
  draw_from_trees("num_iso_tracks",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","(weightppb*20000)",
  		  ";n_{isotk}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig",-1,1,
  		  "num_isotk_cutflow");
  draw_from_trees("num_iso_tracks",baseline+"min_delta_phi_met_N>4&&num_reco_veto_electrons==0&&num_reco_veto_muons==0","(weightppb*20000)",
  		  ";n_{isotk}, p_{T} > 10 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig:plotLog",-1,1,
  		  "num_isotk_cutflow_log");

  draw_from_trees("num_jets_pt70",search+"num_csvm_jets>0","(weightppb*20000)",
  		  ";n_{CSVT}, p_{T} > 70 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "num_jets70_1bm");
 draw_from_trees("num_csvt_jets",search,"(weightppb*20000)",
  		  ";n_{CSVT}, p_{T} > 50 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig",-1,-1,
  		  "num_csvt_search");
  draw_from_trees("num_csvt_jets",search,"(weightppb*20000)",
  		  ";n_{CSVT}, p_{T} > 50 GeV;Events / 20 fb^{-1}", 5, 0., 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "num_csvt_search_log");
  draw_from_trees("num_jets_pt70",search+"num_csvt_jets>=2","(weightppb*20000)",
  		  ";n_{CSVT}, p_{T} > 70 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "num_jets70_search");
  draw_from_trees("num_jets_pt100",search+"num_csvm_jets>=2","(weightppb*20000)",
  		  ";n_{CSVT}, p_{T} > 100 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "num_jets100_search");
  draw_from_trees("num_jets_pt150",search+"num_csvm_jets>=2","(weightppb*20000)",
  		  ";n_{CSVT}, p_{T} > 150 GeV;Events / 20 fb^{-1}", 13, 0., 13.,
  		  "plotSig",-1,-1,
  		  "num_jets150_search");
 draw_from_trees("ht50/1000.",search+"num_csvt_jets>=1","(weightppb*20000)",
  		  ";H_{T} [TeV];Events / 20 fb^{-1}", 46, 0.4, 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_search_1bt");
  draw_from_trees("met",search+"num_csvt_jets>=1","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125, 2000,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_1bt");
  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=2","(weightppb*20000)",
  		  ";H_{T} [TeV];Events / 20 fb^{-1}", 46, 0.4, 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_search_2bt");

  draw_from_trees("ht50/1000.",search+"num_csvt_jets>=3","(weightppb*20000)",
  		  ";H_{T} [TeV];Events / 20 fb^{-1}", 46, 0.4, 5.,
  		  "plotSig:plotLog",-1,-1,
  		  "ht_search_3bt");
  draw_from_trees("met",search+"num_csvt_jets>=3","(weightppb*20000)",
  		  ";E_{T}^{miss} [GeV];Events / 20 fb^{-1}", 75, 125., 2000.,
  		  "plotSig:plotLog",-1,-1,
  		  "met_search_3bt");
  return 0;
  
}

